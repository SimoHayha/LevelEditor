#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "Entity.h"

class Grid : public Entity
{
	private:
		int		widthGrid;
		int		heightGrid;
		ID3D11Buffer*	vertexBuffer;
		ID3D11Buffer*	indexBuffer;

		static void	setVertices(float* positionX, float* positionY, int i, int j, VertexType* vertices,
						    unsigned long* indices, int* index, int addX, int addY);

	public:
		Grid();
		bool	init(ID3D11Device* device);
		void	destroy(void);
		int		getIndexCount(size_t = 0);
		size_t	getMeshCount(void);
		void	getWorldMatrix(DirectX::XMMATRIX& newMatrix, float dt = 0.15f, size_t index = 0);
		static bool	initializeBuffers(ID3D11Device*, size_t width, size_t height);
};