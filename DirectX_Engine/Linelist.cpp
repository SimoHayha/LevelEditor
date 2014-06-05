#include "Linelist.h"

Linelist::Linelist()
{
	_type = Entity::E_LINELIST;
	_name = "linelist";
}

Linelist::~Linelist()
{
}

int	Linelist::getIndexCount(size_t)
{
	return this->nbIndices;
}

size_t	Linelist::getMeshCount()
{
	return 1u;
}

void	Linelist::getWorldMatrix(DirectX::XMMATRIX &, float, size_t)
{

}

bool	Linelist::initializeBuffers(ID3D11Device * device)
{
	_topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
	_stride = sizeof(VertexType);

	if (!this->buffer.init(device, D3D11_USAGE_DEFAULT, sizeof(VertexType) * this->nbVertices, this->vertices, sizeof(unsigned long) * this->nbIndices, this->indices))
		return false;

	this->_buffers.push_back(&this->buffer);

	return true;
}

void	Linelist::setVertex(VertexType * vertices, int count)
{
	this->vertices = vertices;
	this->nbVertices = count;
}

void	Linelist::setIndices(unsigned long * indices, int count)
{
	this->indices = indices;
	this->nbIndices = count;
}