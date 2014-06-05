#pragma once
#include "Entity.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include <cstdlib>
#include <iostream>
#include "VertexType.h"




class Field : public Entity
{
private :
	int				vertexCount;
	int				indexCount;

	ID3D11Buffer*	vertexBuffer;
	ID3D11Buffer*	indexBuffer;

	float			fVariability;
	float			fSpacing;
	unsigned long	unSteps;
	unsigned long	unSize;



	 
	std::vector< std::vector<float> > vectPoints;

		
	void		setVertices(float positionX, float positionY, float positionZ, int i, int j, VertexType* vertices,
						    unsigned long* indices, int* index, DirectX::XMVECTOR& normal, float tu, float tv);
	void		Generate(float fLeftBottom=1.0f, float fLeftTop=1.0f,
						 float fRightTop=1.0f, float fRightBottom=1.0f);
	float			randomize(void);
	float			DiamondStep(unsigned int unI, unsigned int unJ,
								unsigned int unHalfSpacing);
	float				SquareStep(unsigned int unI, unsigned int unJ,
							   unsigned int unHalfSpacing);
  void				initialize();
public:
	
	Field();
	bool			init(ID3D11Device* device);
	int				getIndexCount(size_t = 0);
	size_t			getMeshCount(void);
	void			getWorldMatrix(DirectX::XMMATRIX& newMatrix, float dt = 0.15f, size_t index = 0);
	bool		initializeBuffers(ID3D11Device*);
};