#include "Entity.h"
#include "ShaderManager.h"
#include "LuaScript.h"
#include "VirtualShader.h"
#include "Dictionnary.h"
#include "Texture.h"

Entity::Entity(void) : _script(nullptr),
	myShader(nullptr),
	_unmanaged(nullptr),
	_father(nullptr),
	_texture(nullptr)
{
	_transform.rotX = 0.0f;
	_transform.rotY = 0.0f;
	_transform.rotZ = 0.0f;
	_transform.scaleX = 1.0f;
	_transform.scaleY = 1.0f;
	_transform.scaleZ = 1.0f;
	_transform.transX = 0.0f;
	_transform.transY = 0.0f;
	_transform.transZ = 0.0f;

	_right   = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
	_up      = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	_forward = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);

	_selected = false;

	this->feedback = Feedback::E_FEEDBACK_EMPTY;

	ZeroMemory(&_defaultMat, sizeof(_defaultMat));
	_defaultMat.shinness = 10;
	_defaultMat.diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	_defaultMat.color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	_scriptRunning = false;
}

Entity::~Entity()
{
}

void
Entity::setShader(ShaderManager* shader)
{
	myShader = shader->getTextureShader();
}

VirtualShader*
Entity::getShader(void)
{
	return myShader;
}

bool
Entity::render(ID3D11DeviceContext* context,
		     DirectX::XMMATRIX& viewMatrix,
			 DirectX::XMMATRIX& worldMatrix,
			 DirectX::XMMATRIX& projectionMatrix)
{
	ID3D11ShaderResourceView*	text = nullptr;

	Entity::render(context, 0);
	if (_texture != nullptr)
		text = _texture->getTexture();
	if (myShader->render(context, getIndexCount(0), worldMatrix, viewMatrix, projectionMatrix, text, &_defaultMat) == false)
		return false;
	return true;
}

void
Entity::render(ID3D11DeviceContext* context, size_t index, size_t offset)
{
	_buffers[index]->render(context, _stride, _topology, offset);
}

int&
Entity::getType(void)
{
	return _type;
}

//void
//Entity::translate(float x, float y, float z)
//{
//	_transform.transX = x;
//	_transform.transY = y;
//	_transform.transZ = z;
//}
//
//void
//Entity::scale(float x, float y, float z)
//{
//	_transform.scaleX = x;
//	_transform.scaleY = y;
//	_transform.scaleZ = z;
//}
//
//void
//Entity::rotate(float x, float y, float z)
//{
//	_transform.rotX = x;
//	_transform.rotY = y;
//	_transform.rotZ = z;
//}

bool
Entity::refresh(ID3D11DeviceContext* context, float dt)
{
	if (_script != nullptr && _scriptRunning)
	{
		_script->loadFromEntity(this);
		if (!_script->call(dt))
			return false;
		_script->storeInEntity(this);
	}
	return true;
}

bool
Entity::appendChild(Entity * child)
{
	try
	{
		this->_childs.push_back(child);
	}
	catch (std::bad_alloc &)
	{
		return false;
	}
	child->_father = this;
	return true;
}

bool
Entity::childRefresh(ID3D11DeviceContext * context, float dt, Entity * & bind)
{
	if (!this->refresh(context, dt))
		return false;

	if (this->_selected)
		bind = this;

	for (auto it : this->_childs)
	{
		if (!it->childRefresh(context, dt, bind))
			return false;
	}

	return true;
}

void
Entity::childRender(ID3D11DeviceContext * context, float dt,
							DirectX::XMMATRIX& viewMatrix,
							DirectX::XMMATRIX& worldMatrix,
					   		DirectX::XMMATRIX& projectionMatrix,
							std::vector<DirectX::XMMATRIX*>& parentMatrices)
{
	DirectX::XMMATRIX	localWorld;
	DirectX::XMMATRIX	world;

	getTransformMatrix(localWorld);
	parentMatrices.push_back(&localWorld);
	for (auto it : this->_childs)
	{
		it->childRender(context, dt, viewMatrix, worldMatrix, projectionMatrix, parentMatrices);
	}
	world = worldMatrix;
	int len = parentMatrices.size();
	for (auto i = len - 1 ; i >= 0 ; --i)
		world *= *(parentMatrices[i]);
	parentMatrices.pop_back();
	this->render(context, viewMatrix, world, projectionMatrix);
	if (this->isSelected())
	{
		this->onProcessFeedback();
		this->_boundingBox.RenderBBBuffers(context, world, viewMatrix, projectionMatrix, this->myShader, &this->_defaultMat);
	}
}

void
Entity::setTransX(float f)
{
	if (this->_transform.transX == f)
		return ;

	this->feedback |= Entity::Feedback::E_FEEDBACK_TRANSX;
	if (f > static_cast<float>(MAX_TRANS_X))
		f = static_cast<float>(MAX_TRANS_X);
	if (f < static_cast<float>(MIN_TRANS_X))
		f = static_cast<float>(MIN_TRANS_X);
	this->_transform.transX = f;
}

void
Entity::setTransY(float f)
{
	if (this->_transform.transY == f)
		return ;

	this->feedback |= Entity::Feedback::E_FEEDBACK_TRANSY;
	if (f > static_cast<float>(MAX_TRANS_Y))
		f = static_cast<float>(MAX_TRANS_Y);
	if (f < static_cast<float>(MIN_TRANS_Y))
		f = static_cast<float>(MIN_TRANS_Y);
	this->_transform.transY = f;
}

void
Entity::setTransZ(float f)
{
	if (this->_transform.transZ == f)
		return ;

	this->feedback |= Entity::Feedback::E_FEEDBACK_TRANSZ;
	if (f > static_cast<float>(MAX_TRANS_Z))
		f = static_cast<float>(MAX_TRANS_Z);
	if (f < static_cast<float>(MIN_TRANS_Z))
		f = static_cast<float>(MIN_TRANS_Z);
	this->_transform.transZ = f;
}

void
Entity::setRotateX(float f)
{
	if (this->_transform.rotX == f)
		return ;

	this->feedback |= Entity::Feedback::E_FEEDBACK_ROTX;
	while (f >= DirectX::XM_2PI)
		f -= DirectX::XM_2PI;
	while (f < 0)
		f += DirectX::XM_2PI;
	this->_transform.rotX = f;
}

void
Entity::setRotateY(float f)
{
	if (this->_transform.rotY == f)
		return ;

	this->feedback |= Entity::Feedback::E_FEEDBACK_ROTY;
	while (f >= DirectX::XM_2PI)
		f -= DirectX::XM_2PI;
	while (f < 0)
		f += DirectX::XM_2PI;
	this->_transform.rotY = f;
}

void
Entity::setRotateZ(float f)
{
	if (this->_transform.rotZ == f)
		return ;

	this->feedback |= Entity::Feedback::E_FEEDBACK_ROTZ;
	while (f >= DirectX::XM_2PI)
		f -= DirectX::XM_2PI;
	while (f < 0)
		f += DirectX::XM_2PI;
	this->_transform.rotZ = f;
}

void
Entity::setScaleX(float f)
{
	if (this->_transform.scaleX == f)
		return ;

	this->feedback |= Entity::Feedback::E_FEEDBACK_SCALEX;
	if (f > static_cast<float>(MAX_SCALE_X))
		f = static_cast<float>(MAX_SCALE_X);
	if (f < static_cast<float>(MIN_SCALE_X))
		f = static_cast<float>(MIN_SCALE_X);
	this->_transform.scaleX = f;
}

void
Entity::setScaleY(float f)
{
	if (this->_transform.scaleY == f)
		return ;

	this->feedback |= Entity::Feedback::E_FEEDBACK_SCALEY;
	if (f > static_cast<float>(MAX_SCALE_Y))
		f = static_cast<float>(MAX_SCALE_Y);
	if (f < static_cast<float>(MIN_SCALE_Y))
		f = static_cast<float>(MIN_SCALE_Y);
	this->_transform.scaleY = f;
}

void
Entity::setScaleZ(float f)
{
	if (this->_transform.scaleZ == f)
		return ;

	this->feedback |= Entity::Feedback::E_FEEDBACK_SCALEZ;
	if (f > static_cast<float>(MAX_SCALE_Z))
		f = static_cast<float>(MAX_SCALE_Z);
	if (f < static_cast<float>(MIN_SCALE_Z))
		f = static_cast<float>(MIN_SCALE_Z);
	this->_transform.scaleZ = f;
}

size_t
Entity::getChildCount(void)
{
	return _childs.size();
}

Entity*	
Entity::getChild(size_t index)
{
	return _childs[index];
}

std::string&
Entity::getFilePath(void)
{
	return _filePath;
}

bool
Entity::setFilePath(const std::string& path)
{
	try
	{
		_filePath = path;
	}
	catch (std::bad_alloc)
	{
		return false;
	}
	return true;
}

void
Entity::setUnmanaged(UnmanagedEntity * unmanaged)
{
	this->_unmanaged = unmanaged;
}

void
Entity::setScript(LuaScript* script)
{
	_script = script;
}

void
Entity::getTransformMatrix(DirectX::XMMATRIX& transform)
{
	DirectX::XMVECTOR	quat = DirectX::XMQuaternionRotationRollPitchYaw(_transform.rotX, _transform.rotY, _transform.rotZ);

	transform = DirectX::XMMatrixScaling(_transform.scaleX, _transform.scaleY, _transform.scaleZ);
	transform *= DirectX::XMMatrixRotationQuaternion(quat);
	transform *= DirectX::XMMatrixTranslation(_transform.transX, _transform.transY, _transform.transZ);
}

void
Entity::getGlobalTransformMatrix(DirectX::XMMATRIX& transform)
{
	DirectX::XMMATRIX	tmp;

	this->getTransformMatrix(tmp);

	transform *= tmp;

	if (this->_father)
		this->_father->getGlobalTransformMatrix(transform);
}

size_t
Entity::getSubMeshCount(void)
{
	return 1;
}

shaderMaterial*
Entity::getSubMeshMaterial(size_t index)
{
	return &_defaultMat;
}

std::string&
Entity::getSubMeshName(size_t)
{
	return _defaultMatName;
}

std::string&
Entity::getName(void)
{
	return _name;
}

bool	Entity::makeAabb(VertexType * vertices, int count)
{
	if (!this->_boundingBox.makeAabb(vertices, count))
		return false;
	return true;
}

bool	Entity::makeEmptyAabb()
{
	if (!this->_boundingBox.makeEmptyAabb())
		return false;
	return true;
}

bool	Entity::mergeAabb(DirectX::BoundingBox & that)
{
	DirectX::BoundingBox	newAabb;

	DirectX::BoundingBox::CreateMerged(newAabb, this->getAabb(), that);

	this->_boundingBox.setAabb(newAabb);

	return false;
}

DirectX::BoundingBox &	Entity::getAabb()
{
	return this->_boundingBox.getAabb();
}

bool	Entity::hasAabb()
{
	return this->_boundingBox.hasAabb();
}

void	Entity::onFeedback()
{
	this->_unmanaged->_feedback->OnTransformationUpdate();
}

std::string&	Entity::getTextureName(size_t index)
{
	return _textureName;
}

void	Entity::setTexture(size_t, const std::string name)
{
	_texture = Dictionnary::getInstance()->getTexture(name);
}

std::string&	Entity::getScriptName(void)
{
	return _scriptName;
}

void	Entity::setScript(const std::string name)
{
	_scriptName = name;
	_script = Dictionnary::getInstance()->getScript(name);
}

std::vector<Entity*>& Entity::getChilds(void)
{
	return _childs;
}

void	Entity::getRight(DirectX::XMVECTOR & right)
{
	DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationRollPitchYaw(_transform.rotX, _transform.rotY, _transform.rotZ);

	DirectX::XMStoreFloat3(&_right, DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f)), rotation));
	right = DirectX::XMLoadFloat3(&_right);
	DirectX::XMVector3Normalize(right);
}

void	Entity::getUp(DirectX::XMVECTOR & up)
{
	DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationRollPitchYaw(_transform.rotX, _transform.rotY, _transform.rotZ);

	DirectX::XMStoreFloat3(&_up, DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)), rotation));
	up = DirectX::XMLoadFloat3(&_up);
	DirectX::XMVector3Normalize(up);
}

void	Entity::getForward(DirectX::XMVECTOR & forward)
{
	DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationRollPitchYaw(_transform.rotX, _transform.rotY, _transform.rotZ);

	DirectX::XMStoreFloat3(&_forward, DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f)), rotation));
	forward = DirectX::XMLoadFloat3(&_forward);
	DirectX::XMVector3Normalize(forward);
}

UnmanagedEntity::Transform &	Entity::getTransform()
{
	return this->_transform;
}

float	Entity::getTransX() const
{
	return this->_transform.transX;
}

float	Entity::getTransY() const
{
	return this->_transform.transY;
}

float	Entity::getTransZ() const
{
	return this->_transform.transZ;
}

float	Entity::getRotX() const
{
	return this->_transform.rotX;
}

float	Entity::getRotY() const
{
	return this->_transform.rotY;
}

float	Entity::getRotZ() const
{
	return this->_transform.rotZ;
}

float	Entity::getScaleX() const
{
	return this->_transform.scaleX;
}

float	Entity::getScaleY() const
{
	return this->_transform.scaleY;
}

float	Entity::getScaleZ() const
{
	return this->_transform.scaleZ;
}

void	Entity::setName(const std::string & s)
{
	this->_name = s;
}

void	Entity::onProcessFeedback()
{
	if (this->feedback & Entity::Feedback::E_FEEDBACK_TRANSX)
		this->_unmanaged->_feedback->OnTransXChanged();
	if (this->feedback & Entity::Feedback::E_FEEDBACK_TRANSY)
		this->_unmanaged->_feedback->OnTransYChanged();
	if (this->feedback & Entity::Feedback::E_FEEDBACK_TRANSZ)
		this->_unmanaged->_feedback->OnTransZChanged();
	if (this->feedback & Entity::Feedback::E_FEEDBACK_ROTX)
		this->_unmanaged->_feedback->OnRotXChanged();
	if (this->feedback & Entity::Feedback::E_FEEDBACK_ROTY)
		this->_unmanaged->_feedback->OnRotYChanged();
	if (this->feedback & Entity::Feedback::E_FEEDBACK_ROTZ)
		this->_unmanaged->_feedback->OnRotZChanged();
	if (this->feedback & Entity::Feedback::E_FEEDBACK_SCALEX)
		this->_unmanaged->_feedback->OnScaleXChanged();
	if (this->feedback & Entity::Feedback::E_FEEDBACK_SCALEY)
		this->_unmanaged->_feedback->OnScaleYChanged();
	if (this->feedback & Entity::Feedback::E_FEEDBACK_SCALEZ)
		this->_unmanaged->_feedback->OnScaleZChanged();
	if (this->feedback & Entity::Feedback::E_FEEDBACK_SCRIPT_RUNNING)
		this->_unmanaged->_feedback->OnScriptRunningChanged();

	this->feedback = Entity::Feedback::E_FEEDBACK_EMPTY;
}

Entity *	Entity::getFather() const
{
	return this->_father;
}

DirectX::XMFLOAT3	Entity::getBoundingBoxCenter() const
{
	return this->_boundingBox.getCenter();
}

DirectX::XMFLOAT3	Entity::getBoundingBoxExtend() const
{
	return this->_boundingBox.getExtend();
}

bool	Entity::isSelected() const
{
	return this->_selected;
}

void	Entity::setSelected(bool b)
{
	this->_selected = b;

	if (this->_unmanaged)
		this->_unmanaged->_feedback->OnSelectedChanged();
}

bool	Entity::isScriptRunning() const
{
	return this->_scriptRunning;
}

void	Entity::setScriptRunning(bool b)
{
	if (b != this->_scriptRunning)
		this->_scriptRunning = b;

	this->feedback |= Entity::Feedback::E_FEEDBACK_SCRIPT_RUNNING;
}

void	Entity::setFather(Entity * father)
{
	this->_father = father;
}