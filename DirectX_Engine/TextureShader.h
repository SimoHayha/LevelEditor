#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include "VirtualShader.h"
#include "Entity.h"
#include "Light.h"

class TextureShader : public VirtualShader
{
	ID3D11SamplerState*	sampleState;
	ID3D11Buffer*		materialBuffer;
	ID3D11Buffer*		cameraBuffer;
	ID3D11Buffer*		lightBuffer;

	bool	InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void	ShutdownShader(void);
	bool	SetShaderParameters(ID3D11DeviceContext*,
								DirectX::XMMATRIX&, DirectX::XMMATRIX&, DirectX::XMMATRIX&,
								ID3D11ShaderResourceView*, shaderMaterial* mat = nullptr);
	void	RenderShader(ID3D11DeviceContext*, int);

public:
			TextureShader(void);
	bool	initialize(ID3D11Device*, HWND);
	void	shutdown(void);
	bool	render(ID3D11DeviceContext*, int,
				   DirectX::XMMATRIX&, DirectX::XMMATRIX&, DirectX::XMMATRIX&,
				   ID3D11ShaderResourceView*, shaderMaterial*);
	bool	setCameraConstBuffer(ID3D11DeviceContext* deviceContext, DirectX::XMVECTOR&	vector);
	bool	setLightConstBuffer(ID3D11DeviceContext* deviceContext, std::vector<Light*>& lightEntities);
};