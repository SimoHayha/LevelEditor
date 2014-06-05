#pragma once

#include <d3d11.h>
#include <DirectXCollision.h>
#include <DirectXMath.h>

#include "Buffers.h"
#include "Entity.h"

class VirtualShader;

class BoundingBox
{
private:
	DirectX::BoundingBox	mAabb;
	ID3D11Buffer *			mVertexBuffer;
	ID3D11Buffer *			mIndexBuffer;
	bool					mHasAabb;
	Buffers					mBuffer;

	static const int	NUM_VERTEXES = 8;

public:
	BoundingBox();
	~BoundingBox();

	void	createBBBuffers(DirectX::XMFLOAT3 centerPos, float sizeX, float sizeY, float sizeZ, ID3D11Device * device);
	void	RenderBBBuffers(ID3D11DeviceContext * deviceContext,
							DirectX::XMMATRIX& viewMatrix,
							DirectX::XMMATRIX& worldMatrix,
							DirectX::XMMATRIX& projectionMatrix,
							VirtualShader * shader,
							struct shaderMaterial * defaultMat);

	DirectX::BoundingBox &	getAabb();
	void					setAabb(DirectX::BoundingBox & aabb);

	bool	makeAabb(VertexType * vertices, int count);
	bool	makeEmptyAabb();
	bool	hasAabb();

	DirectX::XMFLOAT3	getCenter() const;
	DirectX::XMFLOAT3	getExtend() const;
};