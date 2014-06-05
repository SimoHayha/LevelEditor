#pragma once

#include <d3d11.h>

class Buffers
{
public:
	Buffers(void);
	virtual ~Buffers(void);

	bool	init(ID3D11Device* device, D3D11_USAGE vertexBufferUsage, UINT vertexByteWidth,
				 void* vertexSysMem, UINT indexByteWidth, void* indexSysMem);
	void	destroy(void);
	void	render(ID3D11DeviceContext* deviceContext, size_t stride,
				   D3D11_PRIMITIVE_TOPOLOGY topology, size_t offset);

	ID3D11Buffer*	getVertexBuffer(void);
	ID3D11Buffer*	getIndexBuffer(void);

private:
	ID3D11Buffer*	_vertexBuffer;
	ID3D11Buffer*	_indexBuffer;
};

