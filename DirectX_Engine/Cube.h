#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Entity.h" 

class Cube : public Entity
{
private :

	int		cubeSize;
	int		vertexCount;
	int		indexCount;
	ID3D11Buffer*	vertexBuffer;
	ID3D11Buffer*	indexBuffer;

	void	setVertices(float positionX, float positionY, float positionZ, VertexType* vertices,
						unsigned long* indices, int* index, float normalX, float normalY, float normalZ,
						float tu, float tv);

public:
	Cube();
	bool	init(ID3D11Device* device);
	int		getIndexCount(size_t = 0);
	size_t	getMeshCount(void);
	void	getWorldMatrix(DirectX::XMMATRIX& newMatrix, float dt = 0.15f, size_t index = 0);
	bool	initializeBuffers(ID3D11Device*);
};