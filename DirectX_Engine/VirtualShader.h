#pragma once

#include <DirectXMath.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <fstream>

#include "Entity.h"

class VirtualShader
{
protected:
	struct MatrixBufferType
	{
		DirectX::XMMATRIX	world;
		DirectX::XMMATRIX	view;
		DirectX::XMMATRIX	projection;
	};
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader*	pixelShader;
	ID3D11InputLayout*	layout;
	ID3D11Buffer*		matrixBuffer;

	void	OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);
public:
					VirtualShader(void);
	virtual			~VirtualShader(void);
	virtual bool	initialize(ID3D11Device* device, HWND hwnd) = 0;
	virtual void	shutdown(void) = 0;
	virtual bool	render(ID3D11DeviceContext*, int,
						   DirectX::XMMATRIX&, DirectX::XMMATRIX&, DirectX::XMMATRIX&,
						   ID3D11ShaderResourceView* = nullptr, shaderMaterial* = nullptr) = 0;

};
