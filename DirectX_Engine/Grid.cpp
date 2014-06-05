#include "Grid.h"
#include "Dictionnary.h"
#include "VirtualShader.h"

Grid::Grid(void) :
	vertexBuffer(nullptr),
	indexBuffer(nullptr)
{
	_type = Entity::E_GRID;
}

bool	Grid::init(ID3D11Device* device)
{
	this->widthGrid = 100;
	this->heightGrid = 100;
	Dictionnary* dico;
	Buffers*	buffer;
	
	if ((dico = dico->getInstance()) == nullptr)
		return false;
	buffer = dico->getBuffer("__grid", 0);
	if (buffer == nullptr)
		return false;
	_stride = sizeof(VertexType);
	_topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
	try
	{
		_defaultMatName = "defaultMatGrid";
		_name = "grid";
		_buffers.push_back(buffer);
	}
	catch (std::bad_alloc)
	{
		return false;
	}
	return (true);
}

int		Grid::getIndexCount(size_t i)
{
	(void)i;
	return (widthGrid - 1) * (heightGrid - 1) * 8;
}

size_t
Grid::getMeshCount(void)
 {
	 return 1;
 }

void	Grid::setVertices(float* positionX, float* positionZ, int i, int j, VertexType* vertices,
						  unsigned long* indices, int* index, int addX, int addZ)
{
	*positionX = (float)(i + addX - 50);
	*positionZ = (float)(j + addZ - 50);
	vertices[*index].vertex = DirectX::XMFLOAT4(*positionX, 0.0f, *positionZ, 1.0f);
	vertices[*index].normal = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	indices[*index] = *index;
	++*index;
}

bool	Grid::initializeBuffers(ID3D11Device* device, size_t width, size_t height)
{
	Dictionnary*			dico;
	VertexType*				vertices;
	Buffers*				buffer;
	unsigned long*			indices;
	int						index;
	float					positionX;
	float					positionZ;
	int						vertexCount;
	int						indexCount;

	vertexCount = (width - 1) * (height - 1) * 8;
	indexCount = vertexCount;

	if ((dico = Dictionnary::getInstance()) == nullptr)
		return false;
	vertices = new VertexType[vertexCount];
	if (vertices == false)
		return (false);
	indices = new unsigned long[indexCount];
	if (indices == false)
	{
		delete []vertices;
		return (false);
	}
	buffer = new Buffers();
	if (buffer == nullptr)
	{
		delete []vertices;
		delete []indices;
		return (false);
	}
	index = 0;
	for(size_t j = 0; j < (height - 1); j++)
	{
		for(size_t i = 0; i < (width - 1); i++)
		{
			setVertices(&positionX, &positionZ, i, j, vertices, indices, &index, 0, 1);
			setVertices(&positionX, &positionZ, i, j, vertices, indices, &index, 1, 1);
			setVertices(&positionX, &positionZ, i, j, vertices, indices, &index, 1, 1);
			setVertices(&positionX, &positionZ, i, j, vertices, indices, &index, 1, 0);
			setVertices(&positionX, &positionZ, i, j, vertices, indices, &index, 1, 0);
			setVertices(&positionX, &positionZ, i, j, vertices, indices, &index, 0, 0);
			setVertices(&positionX, &positionZ, i, j, vertices, indices, &index, 0, 0);
			setVertices(&positionX, &positionZ, i, j, vertices, indices, &index, 0, 1);			
		}
	}

	if (!buffer->init(device, D3D11_USAGE_DEFAULT, sizeof(VertexType) * vertexCount,
					  vertices, sizeof(unsigned long) * indexCount, indices))
	{
		delete []vertices;
		delete []indices;
		delete buffer;
		return false;
	}
	if (dico->getBuffer("__grid", 0, buffer) == nullptr)
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
Grid::getWorldMatrix(DirectX::XMMATRIX& newMatrix, float dt, size_t index)
{
	(void)dt;
	(void)index;
	(void)newMatrix;
}