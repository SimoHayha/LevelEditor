#include "Buffers.h"


Buffers::Buffers(void) : _vertexBuffer(nullptr), _indexBuffer(nullptr)
{
}


Buffers::~Buffers(void)
{
	destroy();
}

bool
Buffers::init(ID3D11Device* device, D3D11_USAGE vertexBufferUsage, UINT vertexByteWidth,
			  void* vertexSysMem, UINT indexByteWidth, void* indexSysMem)
{
	D3D11_BUFFER_DESC					vertexBufferDesc;
	D3D11_BUFFER_DESC					indexBufferDesc;
	D3D11_SUBRESOURCE_DATA				vertexData;
	D3D11_SUBRESOURCE_DATA				indexData;

	vertexBufferDesc.Usage = vertexBufferUsage;
	if (vertexBufferUsage == D3D11_USAGE_DYNAMIC)
		vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	else
		vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	vertexBufferDesc.ByteWidth = vertexByteWidth;
	vertexData.pSysMem = vertexSysMem;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	indexBufferDesc.ByteWidth = indexByteWidth;
	indexData.pSysMem = indexSysMem;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;
	if(FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &_vertexBuffer)))
		return false;
	if(FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &_indexBuffer)))
		return false;
	return true;
}

void
Buffers::destroy(void)
{
	if(_indexBuffer != nullptr)
	{
		_indexBuffer->Release();
		_indexBuffer = nullptr;
	}
	if(_vertexBuffer != nullptr)
	{
		_vertexBuffer->Release();
		_vertexBuffer = nullptr;
	}
}

void
Buffers::render(ID3D11DeviceContext* deviceContext, size_t stride,
				D3D_PRIMITIVE_TOPOLOGY topology, size_t offset)
{
	deviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(topology);
}

ID3D11Buffer*
Buffers::getVertexBuffer(void)
{
	return _vertexBuffer;
}

ID3D11Buffer*
Buffers::getIndexBuffer(void)
{
	return _indexBuffer;
}