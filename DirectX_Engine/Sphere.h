#pragma once
#include "Entity.h"
#include <d3d11.h>
#include <DirectXMath.h>

class Sphere : public Entity
{
private :
	//struct VertexType
	//{
	//	DirectX::XMVECTOR	postion;
	//	DirectX::XMVECTOR	color;
	//	DirectX::XMVECTOR	normal;		
	//};
	int		vertexCount;
	int		indexCount;

	ID3D11Buffer*	vertexBuffer;
	ID3D11Buffer*	indexBuffer;

	FLOAT			deltaRingAngle;
	FLOAT			deltaSegAngle;

	void	setVertices(float positionX, float positionY, float positionZ, VertexType* vertices,
						WORD* index, float normalX, float normalY, float normalZ, float tu, float tv);

public:
	Sphere();
	bool init(ID3D11Device* device);
	int		getIndexCount(size_t = 0);
	size_t	getMeshCount(void);
	void	getWorldMatrix(DirectX::XMMATRIX& newMatrix, float dt = 0.15f, size_t index = 0);
	bool	initializeBuffers(ID3D11Device*);


};