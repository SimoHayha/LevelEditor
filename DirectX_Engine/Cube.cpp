#include "Cube.h"
#include "Dictionnary.h"
#include "VirtualShader.h"

Cube::Cube(void) :
	vertexBuffer(nullptr),
	indexBuffer(nullptr)
{
	_type = Entity::E_CUBE;
}

bool	Cube::init(ID3D11Device* device)
{
	Dictionnary*	dico;
	Buffers*		buffer;
	
	if ((dico = dico->getInstance()) == nullptr)
		return false;
	buffer = dico->getBuffer("__cube", 0);
	if (buffer == nullptr)
		return false;
	this->cubeSize = 10;
	this->vertexCount = 36;
	this->indexCount = 36;
	_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	_stride = sizeof(VertexType);
	try
	{
		_defaultMatName = "defaultMatCube";
		_name = "cube";
		_buffers.push_back(buffer);
	}
	catch (std::bad_alloc)
	{
		return false;
	}
	return (true);
}

int		Cube::getIndexCount(size_t i)
{
	(void)i;
	return (this->indexCount);
}
	
size_t Cube::getMeshCount(void)
{
	return 1;
}
void	Cube::setVertices(float positionX, float positionY, float positionZ, VertexType* vertices,
						  unsigned long* indices, int* index, float normalX, float normalY, float normalZ,
						  float tu, float tv)
{
	vertices[*index].vertex = DirectX::XMFLOAT4(positionX, positionY, positionZ, 1.0f);
	vertices[*index].normal = DirectX::XMFLOAT4(normalX, normalY, normalZ, 1.0f);
	vertices[*index].UV = DirectX::XMFLOAT2(tu, tv);
	indices[*index] = *index;
	++*index;
}

bool	Cube::initializeBuffers(ID3D11Device* device)
{
	Dictionnary*	dico;
	Buffers*		buffer;
	VertexType *	vertices;
	unsigned long*	indices;
	int				index = 0;

	int	vertexCount = 36;
	int indexCount = 36;

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
	if(vertexCount != 0)
	{
		setVertices(-1.0f, 1.0f, -1.0f, vertices, indices, &index, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
		setVertices(1.0f, 1.0f, -1.0f, vertices, indices, &index, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
		setVertices(-1.0f, -1.0f, -1.0f, vertices, indices, &index, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
		setVertices(-1.0f, -1.0f, -1.0f, vertices, indices, &index, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
		setVertices(1.0f, 1.0f, -1.0f, vertices, indices, &index, 0.0f, 0.0f,-1.0f, 1.0f, 0.0f);
		setVertices(1.0f, -1.0f, -1.0f, vertices, indices, &index, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);
		setVertices(1.0f, 1.0f, -1.0f, vertices, indices, &index, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		setVertices(1.0f, 1.0f, 1.0f, vertices, indices, &index, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
		setVertices(1.0f, -1.0f, -1.0f, vertices, indices, &index, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		setVertices(1.0f, -1.0f, -1.0f, vertices, indices, &index, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		setVertices(1.0f, 1.0f, 1.0f, vertices, indices, &index, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
		setVertices(1.0f, -1.0f, 1.0f, vertices, indices, &index, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
		setVertices(1.0f, 1.0f, 1.0f, vertices, indices, &index, 0.0f, 0.0f,1.0f, 0.0f, 0.0f);
		setVertices(-1.0, 1.0, 1.0f, vertices, indices, &index, 0.0f, 0.0f,1.0f, 1.0f, 0.0f);
		setVertices(1.0, -1.0, 1.0f, vertices, indices, &index, 0.0f, 0.0f,1.0f, 0.0f, 1.0f);
		setVertices(1.0, -1.0, 1.0f, vertices, indices, &index, 0.0f, 0.0f,1.0f, 0.0f, 1.0f);
		setVertices(-1.0, 1.0, 1.0f, vertices, indices, &index, 0.0f, 0.0f,1.0f, 1.0f, 0.0f);
		setVertices(-1.0, -1.0, 1.0f, vertices, indices, &index, 0.0f, 0.0f,1.0f, 1.0f, 1.0f);
		setVertices(-1.0f, 1.0f, 1.0f, vertices, indices, &index,-1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		setVertices(-1.0f, 1.0f, -1.0f, vertices, indices, &index,-1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
		setVertices(-1.0f, -1.0f, 1.0f, vertices, indices, &index,-1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		setVertices(-1.0f, -1.0f, 1.0f, vertices, indices, &index,-1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		setVertices(-1.0f, 1.0f, -1.0f, vertices, indices, &index,-1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
		setVertices(-1.0f, -1.0f, -1.0f, vertices, indices, &index,-1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
		setVertices(-1.0f, 1.0f, 1.0f, vertices, indices, &index, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
		setVertices(1.0f, 1.0f, 1.0f, vertices, indices, &index, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
		setVertices(-1.0f, 1.0f, -1.0f, vertices, indices, &index, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
		setVertices(-1.0f, 1.0f, -1.0f, vertices, indices, &index, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
		setVertices(1.0f, 1.0f, 1.0f, vertices, indices, &index, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
		setVertices(1.0f, 1.0f, -1.0f, vertices, indices, &index, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);
		setVertices(-1.0f, -1.0f, -1.0f, vertices, indices, &index, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f);
		setVertices(1.0f, -1.0f, -1.0f, vertices, indices, &index, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);
		setVertices(-1.0f, -1.0f, 1.0f, vertices, indices, &index, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f);
		setVertices(-1.0f, -1.0f, 1.0f, vertices, indices, &index, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f);
		setVertices( 1.0f, -1.0f, -1.0f, vertices, indices, &index, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);
		setVertices(1.0f, -1.0f, 1.0f, vertices, indices, &index, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f);
	}

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
	if (dico->getBuffer("__cube", 0, buffer) == nullptr)
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
Cube::getWorldMatrix(DirectX::XMMATRIX& newMatrix, float dt, size_t index)
{
	(void)dt;
	(void)index;
	(void)newMatrix;
}