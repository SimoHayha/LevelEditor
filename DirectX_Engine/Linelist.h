#pragma once

#include <vector>

#include "Entity.h"

class Linelist : public Entity
{
private:
	/*std::vector<DirectX::XMFLOAT3>	points;
	std::vector<unsigned short>		indices;*/
	Buffers							buffer;
	VertexType *					vertices;
	unsigned long *					indices;
	int								nbVertices;
	int								nbIndices;

public:
	Linelist();
	virtual ~Linelist();

	bool	init(ID3D11Device * device);
	
	virtual int		getIndexCount(size_t = 0);
	virtual size_t	getMeshCount();
	virtual void	getWorldMatrix(DirectX::XMMATRIX &,
								   float dt,
								   size_t index = 0);
	virtual bool	initializeBuffers(ID3D11Device * device);

	/*bool			pushVertex(DirectX::XMFLOAT3 point);
	bool			pushIndice(unsigned short indice);*/

	void			setVertex(VertexType * vertices, int count);
	void			setIndices(unsigned long * indices, int count);
};