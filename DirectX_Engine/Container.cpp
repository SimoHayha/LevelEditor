#include "Container.h"

Container::Container()
{
	_type = Entity::E_CONTAINER;
	_device = nullptr;
}

Container::~Container()
{
}

bool	Container::initialize(ID3D11Device * device)
{
	try
	{
		_name = "container";
	}
	catch (std::bad_alloc)
	{
		return  false;
	}

	if (!this->makeEmptyAabb())
		return false;

	this->_boundingBox.createBBBuffers(DirectX::XMFLOAT3(this->_boundingBox.getAabb().Center.x, this->_boundingBox.getAabb().Center.y, this->_boundingBox.getAabb().Center.z), this->_boundingBox.getAabb().Extents.x * 2, this->_boundingBox.getAabb().Extents.y, this->_boundingBox.getAabb().Extents.z * 2, device);

	this->_device = device;

	return true;
}

int	Container::getIndexCount(size_t index)
{
	return 0;
}

size_t	Container::getMeshCount()
{
	return 1u;
}

void	Container::getWorldMatrix(DirectX::XMMATRIX &, float, size_t)
{
}

bool	Container::refresh(ID3D11DeviceContext* context, float dt)
{
	std::vector<DirectX::BoundingBox>	boxes;

	try
	{
		boxes.resize(this->_childs.size());
	}
	catch (std::bad_alloc)
	{
		return false;
	}

	unsigned int	i;

	for (i = 0u; i < this->_childs.size(); ++i)
	{
		DirectX::XMMATRIX	transform = DirectX::XMMatrixIdentity();

		this->_childs[i]->getTransformMatrix(transform);
		boxes[i] = DirectX::BoundingBox(this->_childs[i]->getAabb());
		boxes[i].Transform(boxes[i], transform);
	}

	DirectX::BoundingBox	finalBox;

	finalBox.Center.x = 0.0f;
	finalBox.Center.y = 0.0f;
	finalBox.Center.z = 0.0f;
	finalBox.Extents.x = 0.0f;
	finalBox.Extents.y = 0.0f;
	finalBox.Extents.z = 0.0f;

	for (auto it : boxes)
		DirectX::BoundingBox::CreateMerged(finalBox, finalBox, it);

	//this->_boundingBox.setAabb(finalBox);

	this->_boundingBox.createBBBuffers(DirectX::XMFLOAT3(finalBox.Center.x, finalBox.Center.y, finalBox.Center.z), finalBox.Extents.x * 2, finalBox.Extents.y, finalBox.Extents.z * 2, this->_device);

	return true;
}

bool	Container::render(ID3D11DeviceContext* context,
						  DirectX::XMMATRIX& viewMatrix,
						  DirectX::XMMATRIX& worldMatrix,
						  DirectX::XMMATRIX& projectionMatrix)
{
	return true;
}

bool	Container::appendChild(Entity * child)
{
	try
	{
		this->_childs.push_back(child);
	}
	catch (std::bad_alloc &)
	{
		return false;
	}
	child->setFather(this);

	this->mergeAabb(child->getAabb());

	return true;
}