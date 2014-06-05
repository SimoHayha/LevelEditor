#include <memory>

#include "VirtualShader.h"
#include "BoundingBox.h"

BoundingBox::BoundingBox() :
	mVertexBuffer(nullptr),
	mIndexBuffer(nullptr),
	mHasAabb(false)
{
}

BoundingBox::~BoundingBox()
{
}

void	BoundingBox::createBBBuffers(DirectX::XMFLOAT3 centerPos,
									 float sizeX,
									 float sizeY,
									 float sizeZ,
									 ID3D11Device * device)
{
	VertexType *			vertices;

	if (this->mIndexBuffer)
	{
		this->mIndexBuffer->Release();
		this->mIndexBuffer = nullptr;
	}
	if (this->mVertexBuffer)
	{
		this->mVertexBuffer->Release();
		this->mVertexBuffer = nullptr;
	}

	vertices = new VertexType[NUM_VERTEXES];

	// Front face
	vertices[0].vertex = DirectX::XMFLOAT4(centerPos.x - sizeX / 2, centerPos.y + sizeY, centerPos.z - sizeZ / 2, 1.0f);
	vertices[1].vertex = DirectX::XMFLOAT4(centerPos.x + sizeX / 2, centerPos.y + sizeY, centerPos.z - sizeZ / 2, 1.0f);
	vertices[2].vertex = DirectX::XMFLOAT4(centerPos.x - sizeX / 2, centerPos.y - sizeY,  centerPos.z - sizeZ / 2, 1.0f);
	vertices[3].vertex = DirectX::XMFLOAT4(centerPos.x + sizeX / 2, centerPos.y - sizeY,  centerPos.z - sizeZ / 2, 1.0f);
	// Back face
	vertices[4].vertex = DirectX::XMFLOAT4(centerPos.x - sizeX / 2, centerPos.y + sizeY, centerPos.z + sizeZ / 2, 1.0f);
	vertices[5].vertex = DirectX::XMFLOAT4(centerPos.x + sizeX / 2, centerPos.y + sizeY, centerPos.z + sizeZ / 2, 1.0f);
	vertices[6].vertex = DirectX::XMFLOAT4(centerPos.x - sizeX / 2, centerPos.y - sizeY, centerPos.z + sizeZ / 2, 1.0f);
	vertices[7].vertex = DirectX::XMFLOAT4(centerPos.x + sizeX / 2, centerPos.y - sizeY, centerPos.z + sizeZ / 2, 1.0f);

	unsigned long indices[] = {
                // front
                0, 1,
                0, 2,
                1, 3,
                2, 3,
                // left
                0, 4,
                2, 6,
                4, 6,
                // far
                4, 5,
                5, 7,
                6, 7,
                // right
                5, 1,
                7, 3
        };

	this->mBuffer.destroy();

	if (!this->mBuffer.init(device, D3D11_USAGE_DEFAULT, sizeof(VertexType) * NUM_VERTEXES, vertices, sizeof(unsigned long) * 24, indices))
		return ;

	delete [] vertices;
	vertices = 0;

	this->mHasAabb = true;
}

void	BoundingBox::RenderBBBuffers(ID3D11DeviceContext * deviceContext,
									 DirectX::XMMATRIX& viewMatrix,
									 DirectX::XMMATRIX& worldMatrix,
									 DirectX::XMMATRIX& projectionMatrix,
									 VirtualShader * shader,
									 shaderMaterial * defaultMat)
{
	unsigned int stride, offset;
	
	stride = sizeof(VertexType); 
	offset = 0;
	
	this->mBuffer.render(deviceContext, stride, D3D11_PRIMITIVE_TOPOLOGY_LINELIST, offset);

	shader->render(deviceContext, 24, viewMatrix, worldMatrix, projectionMatrix, nullptr, defaultMat);
}

DirectX::BoundingBox &
BoundingBox::getAabb()
{
	return this->mAabb;
}

bool	BoundingBox::makeAabb(VertexType * vertices, int count)
{
	using namespace DirectX;

	XMFLOAT3 *	points = nullptr;

	try
	{
		points = new XMFLOAT3[count];
	}
	catch (std::bad_alloc &)
	{
		return false;
	}

	for (auto i = 0; i < count; ++i)
	{
		points[i].x = vertices[i].vertex.x;
		points[i].y = vertices[i].vertex.y;
		points[i].z = vertices[i].vertex.z;
	}

	DirectX::BoundingBox::CreateFromPoints(this->mAabb, count, points, sizeof(XMFLOAT3));

	delete [] points;

	return true;
}

bool	BoundingBox::makeEmptyAabb()
{
	DirectX::BoundingBox(this->mAabb);

	return true;
}

bool	BoundingBox::hasAabb()
{
	return this->mHasAabb;
}

DirectX::XMFLOAT3	BoundingBox::getCenter() const
{
	if (this->mHasAabb)
		return this->mAabb.Center;
	return DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
}

DirectX::XMFLOAT3	BoundingBox::getExtend() const
{
	if (this->mHasAabb)
		return this->mAabb.Extents;
	return DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
}

void	BoundingBox::setAabb(DirectX::BoundingBox & aabb)
{
	this->mAabb = aabb;
}