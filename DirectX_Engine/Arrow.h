#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Entity.h" 

class Arrow : public Entity
{
private :

	int				vertexCount;
	int				indexCount;
	ID3D11Buffer*	vertexBuffer;
	ID3D11Buffer*	indexBuffer;

public:
	Arrow();
	virtual bool	render(ID3D11DeviceContext* context,
						   DirectX::XMMATRIX& viewMatrix,
						   DirectX::XMMATRIX& worldMatrix,
						   DirectX::XMMATRIX& projectionMatrix);
	bool	init(ID3D11Device* device);
	int		getIndexCount(size_t = 0);
	size_t	getMeshCount(void);
	void	getWorldMatrix(DirectX::XMMATRIX& newMatrix, float dt = 0.15f, size_t index = 0);
	bool	initializeBuffers(ID3D11Device*);
	virtual void	getGlobalTransformMatrix(DirectX::XMMATRIX& transform);
};