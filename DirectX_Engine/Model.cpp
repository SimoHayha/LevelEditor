#include "Model.h"
#include "Log.hpp"
#include "ShaderManager.h"
#include "Dictionnary.h"

Model::Model(void) :
	model(nullptr), _initialized(false), animate(false), animateOneTime(false), animTimeCallback(0.0f)
{
	this->setCurrentAnimation(0);
	_type = Entity::E_MODEL;
	this->animAutoNext = false;
}

bool
Model::initialize(ID3D11Device* device, id::FBX::Model* fbxModel, Buffers* buffer, size_t meshIndex)
{
	FbxScene*	scene;

	this->device = device;
	if (fbxModel == nullptr)
		return true;
	model = fbxModel;
	_meshIndex = meshIndex;
	scene = fbxModel->getScene();
	_stride = sizeof(VertexType);
	_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	if (!animator.initAll(scene))
		return false;
	try
	{
		_buffers.push_back(buffer);
	}
	catch (std::bad_alloc)
	{
		return false;
	}

	if (!this->makeAabb(fbxModel->getMesh(meshIndex).vertice, fbxModel->getMesh(meshIndex).vertexCount))
		return false;

	this->_boundingBox.createBBBuffers(DirectX::XMFLOAT3(this->_boundingBox.getAabb().Center.x, this->_boundingBox.getAabb().Center.y, this->_boundingBox.getAabb().Center.z), this->_boundingBox.getAabb().Extents.x * 2, this->_boundingBox.getAabb().Extents.y, this->_boundingBox.getAabb().Extents.z * 2, device);
	return true;
}

void
Model::setShader(ShaderManager* shader)
{
	_shaders = shader;
}

bool
Model::refresh(ID3D11DeviceContext* context, float dt)
{
	if (!Entity::refresh(context, dt))
		return false;
	if (!this->animate && !this->animateOneTime)
		return true;

	//if (!this->animateOneTime)
	//	return true;

	D3D11_MAPPED_SUBRESOURCE	mappedResource;
	ID3D11Buffer*				buffer;
	VertexType*					dataPtr;
	FbxVector4*					newVertices;

	id::FBX::Mesh&	mesh = model->getMesh(_meshIndex);
	newVertices = new (std::nothrow) FbxVector4[mesh.node->GetMesh()->GetControlPointsCount()];
	if (newVertices == nullptr)
		return false;
	if (!animator.computeDeformations(mesh, newVertices))
	{
		delete[] newVertices;
		return false;
	}
	buffer = _buffers[0]->getVertexBuffer();
	if (FAILED(context->Map(buffer, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mappedResource)))
	{
		delete[] newVertices;
		return false;
	}
	dataPtr = (VertexType*)mappedResource.pData;

	for (size_t j = 0 ; j < mesh.vertexCount ; ++j)
		dataPtr[j].vertex = mesh.vertice[j].vertex;

	delete[] newVertices;

	if (false == this->makeAabb(dataPtr, mesh.node->GetMesh()->GetControlPointsCount()))
		return false;
	this->_boundingBox.createBBBuffers(DirectX::XMFLOAT3(this->_boundingBox.getAabb().Center.x, this->_boundingBox.getAabb().Center.y, this->_boundingBox.getAabb().Center.z), this->_boundingBox.getAabb().Extents.x * 2, this->_boundingBox.getAabb().Extents.y, this->_boundingBox.getAabb().Extents.z * 2, this->device);

	context->Unmap(buffer, 0);

	if (this->animateOneTime)
	{
		this->animateOneTime = false;
		this->_unmanaged->_feedback->OnCurrentTimeChanged(this->animator.getTime().GetSecondDouble());
		this->_unmanaged->_feedback->OnTotalTimeChanged(this->animator.getTotalTime().GetSecondDouble());
	}
	else
	{
		if (animator.incDt(dt))
		{
			if (this->isAnimationLoop())
				this->setCurrentAnimation(this->getCurrentAnimation());
			else if (this->isAnimationMoveNextOnEnd())
				this->setCurrentAnimation(this->getCurrentAnimation() + 1);
			else
			{
				this->animate = false;
				this->_unmanaged->_feedback->OnCurrentTimeChanged(this->animator.getTime().GetSecondDouble());
				this->_unmanaged->_feedback->OnTotalTimeChanged(this->animator.getTotalTime().GetSecondDouble());
				this->animTimeCallback = 0.0f;
				return true;
			}
		}

		this->animTimeCallback += dt;

		if (this->animTimeCallback >= 0.25f)
		{
			if (nullptr != this->_unmanaged)
			{
				this->_unmanaged->_feedback->OnCurrentTimeChanged(this->animator.getTime().GetSecondDouble());
				this->_unmanaged->_feedback->OnTotalTimeChanged(this->animator.getTotalTime().GetSecondDouble());
			}
			this->animTimeCallback = 0.0f;
		}
	}

	return true;
}

int
Model::getIndexCount(size_t index)
{
	return model->getMesh(index).indexCount;
}

size_t
Model::getMeshCount(void)
{
	return _buffers.size();
}

void
Model::getWorldMatrix(DirectX::XMMATRIX& newMatrix, float dt, size_t index)
{
	FbxAMatrix			matrix;
	DirectX::XMFLOAT4X4	tmp;
	DirectX::XMMATRIX	worldMatrix;
	id::FBX::Mesh&		mesh = model->getMesh(index);

	matrix.SetIdentity();
	animator.getGlobalMatrix(matrix, mesh.node);
		for (size_t i = 0 ; i < 4 ; ++i)
			for (size_t j = 0 ; j < 4 ; ++j)
				tmp(i, j) = static_cast<float>(matrix.Get(i, j));
	newMatrix = DirectX::XMLoadFloat4x4(&tmp);
	animator.incDt(dt);
}

bool
Model::render(ID3D11DeviceContext* context,
		      DirectX::XMMATRIX& viewMatrix,
			  DirectX::XMMATRIX& worldMatrix,
			  DirectX::XMMATRIX& projectionMatrix)
{
	id::FBX::Material*			mat;
	DirectX::XMMATRIX			transformMatrix;
	shaderMaterial*	shaderMat;
	ID3D11ShaderResourceView*	texture;
	size_t	meshCount = model->getMeshCount();

	//if (!mesh.hasDeformation)
	//{
	//	getWorldMatrix(worldMatrix, dt, _meshIndex);
	//	worldMatrix *= transformMatrix;
	//}
	id::FBX::Mesh&	mesh = model->getMesh(_meshIndex);
	FbxArray<id::FBX::Mesh::SubMesh*>&	subMeshes = mesh.getSubMeshArray();
	size_t	size = subMeshes.GetCount();
	for (size_t j = 0 ; j < size; ++j)
	{
		myShader = _shaders->getTextureShader();
		mat = subMeshes[j]->material;
		if (mat == nullptr)
		{
			shaderMat = &_defaultMat;
			texture = nullptr;
		}
		else
		{
			shaderMat = mat->getShaderMaterial();
			texture = mat->getTexture(id::FBX::EDIFFUSE);
		}
		Entity::render(context, 0, subMeshes[j]->indexOffset * sizeof(VertexType));
		if (myShader->render(context, subMeshes[j]->triangleCount * 3,
			worldMatrix, viewMatrix, projectionMatrix,
			texture, shaderMat) == false)
			return false;
	}
	return true;
}

id::FBX::Animator const *	Model::getAnimator() const
{
	return &this->animator;
}

void	Model::setAnimate(bool b)
{
	this->animate = b;
}

bool	Model::isAnimate() const
{
	return this->animate;
}

void	Model::setCurrentAnimation(int animation)
{
	this->currentAnimation = animation;

	this->animator.setCurrentAnimation(this->currentAnimation);
}

int	Model::getCurrentAnimation() const
{
	return this->currentAnimation;
}

double	Model::getCurrentAnimationTotalTime()
{
	return this->animator.getTotalTime().GetSecondDouble();
}

double	Model::getCurrentAnimationCurrentTime()
{
	return this->animator.getTime().GetSecondDouble();
}

void	Model::setCurrentAnimationCurrentTime(double d)
{
	this->animator.setCurrentAnimationCurrentTime(d);
	this->animateOneTime = true;
}

size_t
Model::getSubMeshCount(void)
{
	return  model->getMesh(_meshIndex).getSubMeshArray().GetCount();
}

shaderMaterial*
Model::getSubMeshMaterial(size_t index)
{
	return model->getMesh(_meshIndex).getSubMeshArray()[index]->material->getShaderMaterial();
}

std::string&
Model::getSubMeshName(size_t index)
{
	id::FBX::Material* mat;
	mat = model->getMesh(_meshIndex).getSubMeshArray()[index]->material;
	if (mat != nullptr)
		_defaultMatName = mat->getFbxMaterial()->GetName();
	else
		_defaultMatName.clear();
	return _defaultMatName;
}

std::string&
Model::getName(void)
{
	_name = model->getMesh(_meshIndex).node->GetName();
	return _name;
}

std::string&
Model::getTextureName(size_t index)
{
	id::FBX::Material* mat;
	mat = model->getMesh(_meshIndex).getSubMeshArray()[index]->material;
	if (mat != nullptr)
	{
		Texture* text = mat->getTextureContainer(id::FBX::EMat::EDIFFUSE);
		if (text != nullptr)
			_defaultMatName = text->name;
		else
			_defaultMatName.clear();
	}
	else
		_defaultMatName.clear();
	return _defaultMatName;
}

void
Model::setTexture(size_t index, const std::string name)
{
	Texture*	text = Dictionnary::getInstance()->getTexture(name);

	if (text != nullptr)
	{
		id::FBX::Material* mat;
		mat = model->getMesh(_meshIndex).getSubMeshArray()[index]->material;
		mat->setTextureContainer(id::FBX::EMat::EDIFFUSE, text);
	}
}

bool	Model::isAnimationLoop() const
{
	return this->animator.isLoop();
}

void	Model::setAnimationLoop(bool b)
{
	this->animator.setLoop(b);
}

bool	Model::isAnimationMoveNextOnEnd() const
{
	return this->animAutoNext;
}

void	Model::setAnimationMoveNextOnEnd(bool b)
{
	this->animAutoNext = b;
}

float	Model::getAnimationSpeed() const
{
	return this->animator.getSpeed();
}

void	Model::setAnimationSpeed(float f)
{
	this->animator.setSpeed(f);
	this->_unmanaged->_feedback->OnAnimationSpeedChanged();
}