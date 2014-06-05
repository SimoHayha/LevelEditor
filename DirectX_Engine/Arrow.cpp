#include "Arrow.h"
#include "Dictionnary.h"
#include "VirtualShader.h"
#include "Renderer.h"

Arrow::Arrow(void) :
	vertexBuffer(nullptr),
	indexBuffer(nullptr)
{
	_type = Entity::E_ARROW;
}

bool	Arrow::init(ID3D11Device* device)
{
	Dictionnary*	dico;
	Buffers*		buffer;
	
	if ((dico = dico->getInstance()) == nullptr)
		return false;
	buffer = dico->getBuffer("__Arrow", 0);
	if (buffer == nullptr)
		return false;
	this->vertexCount = 13;
	this->indexCount = 66;
	_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	_stride = sizeof(VertexType);
	try
	{
		_defaultMatName = "defaultMatArrow";
		_name = "Arrow";
		_buffers.push_back(buffer);
	}
	catch (std::bad_alloc)
	{
		return false;
	}
	return (true);
}

int		Arrow::getIndexCount(size_t i)
{
	(void)i;
	return (this->indexCount);
}
	
size_t Arrow::getMeshCount(void)
{
	return 1;
}

bool	Arrow::initializeBuffers(ID3D11Device* device)
{
	Dictionnary*	dico;
	Buffers*		buffer;
	VertexType *	vertices;
	unsigned long*	indices;
	int				index = 0;

	int	vertexCount = 13;
	int indexCount = 66;

	if ((dico = Dictionnary::getInstance()) == nullptr)
		return false;
	buffer = new Buffers();
	if (buffer == nullptr)
		return false;
	vertices = new (std::nothrow) VertexType[vertexCount];
	if (vertices == nullptr)
	{
		delete buffer;
		return (false);
	}
	indices = new (std::nothrow) unsigned long[indexCount];
	if (indices == nullptr)
	{
		delete buffer;
		delete []vertices;
		return (false);
	}
	vertices[0].vertex =  DirectX::XMFLOAT4(0.0f, 0.07f, 0.07f, 1.0f);
	vertices[1].vertex =  DirectX::XMFLOAT4(0.0f, 0.07f, -0.07f, 1.0f);
	vertices[2].vertex =  DirectX::XMFLOAT4(0.0f, -0.07f, 0.07f, 1.0f);
	vertices[3].vertex =  DirectX::XMFLOAT4(0.0f, -0.07f, -0.07f, 1.0f);
	vertices[4].vertex =  DirectX::XMFLOAT4(0.65f, 0.07f, 0.07f, 1.0f);
	vertices[5].vertex =  DirectX::XMFLOAT4(0.65f, 0.07f, -0.07f, 1.0f);
	vertices[6].vertex =  DirectX::XMFLOAT4(0.65f, -0.07f, 0.07f, 1.0f);
	vertices[7].vertex =  DirectX::XMFLOAT4(0.65f, -0.07f, -0.07f, 1.0f);
	vertices[8].vertex =  DirectX::XMFLOAT4(0.65f, 0.1f, 0.1f, 1.0f);
	vertices[9].vertex =  DirectX::XMFLOAT4(0.65f, 0.1f, -0.1f, 1.0f);
	vertices[10].vertex = DirectX::XMFLOAT4(0.65f, -0.1f, 0.1f, 1.0f);
	vertices[11].vertex = DirectX::XMFLOAT4(0.65f, -0.1f, -0.1f, 1.0f);
	vertices[12].vertex = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 3;
	indices[3] = 0;
	indices[4] = 3;
	indices[5] = 2;
	indices[6] = 0;
	indices[7] = 1;
	indices[8] = 4;
	indices[9] = 1;
	indices[10] = 5;
	indices[11] = 4;
	indices[12] = 0;
	indices[13] = 4;
	indices[14] = 6;
	indices[15] = 0;
	indices[16] = 6;
	indices[17] = 2;
	indices[18] = 1;
	indices[19] = 5;
	indices[20] = 7;
	indices[21] = 1;
	indices[22] = 7;
	indices[23] = 3;
	indices[24] = 2;
	indices[25] = 6;
	indices[26] = 3;
	indices[27] = 3;
	indices[28] = 7;
	indices[29] = 6;
	indices[30] = 8;
	indices[31] = 9;
	indices[32] = 4;
	indices[33] = 9;
	indices[34] = 4;
	indices[35] = 5;
	indices[36] = 6;
	indices[37] = 7;
	indices[38] = 10;
	indices[39] = 7;
	indices[40] = 10;
	indices[41] = 11;
	indices[42] = 8;
	indices[43] = 4;
	indices[44] = 6;
	indices[45] = 8;
	indices[46] = 6;
	indices[47] = 10;
	indices[48] = 9;
	indices[49] = 5;
	indices[50] = 7;
	indices[51] = 9;
	indices[52] = 7;
	indices[53] = 11;
	indices[54] = 8;
	indices[55] = 9;
	indices[56] = 12;
	indices[57] = 10;
	indices[58] = 11;
	indices[59] = 12;
	indices[60] = 8;
	indices[61] = 10;
	indices[62] = 12;
	indices[63] = 9;
	indices[64] = 11;
	indices[65] = 12;
	if (!this->makeAabb(vertices, vertexCount))
		return false;
	this->_boundingBox.createBBBuffers(DirectX::XMFLOAT3(this->_boundingBox.getAabb().Center.x, this->_boundingBox.getAabb().Center.y, this->_boundingBox.getAabb().Center.z), this->_boundingBox.getAabb().Extents.x * 2, this->_boundingBox.getAabb().Extents.y, this->_boundingBox.getAabb().Extents.z * 2, device);
	if (!buffer->init(device, D3D11_USAGE_DEFAULT, sizeof(VertexType)* vertexCount, vertices,
					 sizeof(unsigned long) * indexCount, indices))
	{
		delete buffer;
		delete[] vertices;
		delete[] indices;
	}
	if (dico->getBuffer("__Arrow", 0, buffer) == nullptr)
	{
		delete []vertices;
		delete []indices;
		delete buffer;
		return false;
	}
	delete []vertices;
	delete []indices;
	return true;

}
void
Arrow::getWorldMatrix(DirectX::XMMATRIX& newMatrix, float dt, size_t index)
{
	(void)dt;
	(void)index;
	(void)newMatrix;
}

bool
Arrow::render(ID3D11DeviceContext* context,
		     DirectX::XMMATRIX& viewMatrix,
			 DirectX::XMMATRIX& worldMatrix,
			 DirectX::XMMATRIX& projectionMatrix)
{
	ID3D11ShaderResourceView*	text = nullptr;

	Entity::render(context, 0);
	Renderer::SetRasterizer(D3D11_FILL_SOLID, D3D11_CULL_NONE);
	if (_texture != nullptr)
		text = _texture->getTexture();
	if (myShader->render(context, getIndexCount(0), worldMatrix, viewMatrix, projectionMatrix, text, &_defaultMat) == false)
		return false;
	Renderer::SetRasterizer(D3D11_FILL_SOLID, D3D11_CULL_BACK);
	return true;
}

void
Arrow::getGlobalTransformMatrix(DirectX::XMMATRIX& transform)
{
	DirectX::XMMATRIX	tmp;
	DirectX::XMVECTOR	scale;
	DirectX::XMVECTOR	rot;
	DirectX::XMVECTOR	trans;

	this->getTransformMatrix(tmp);
	transform *= tmp;
	tmp = transform;
	if (this->_father)
		this->_father->getGlobalTransformMatrix(tmp);
	DirectX::XMMatrixDecompose(&scale, &rot, &trans, tmp);
	transform *= DirectX::XMMatrixTranslationFromVector(trans);
}