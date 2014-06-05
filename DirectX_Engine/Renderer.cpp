#include "Renderer.h"

ID3D11DeviceContext*	Renderer::deviceContext;
ID3D11RasterizerState*	Renderer::rasterizerState;
ID3D11RasterizerState*	Renderer::rasterizerWireState;
ID3D11RasterizerState*	Renderer::rasterizerSolidNoCullState;

Renderer::Renderer(void) : 
	swapChain(nullptr), device(nullptr),
	renderTargetView(nullptr), depthStencilBuffer(nullptr), depthStencilState(nullptr),
	depthStencilView(nullptr), depthDisabledStencilState(nullptr),
	alphaEnableBlendingState(nullptr), alphaDisableBlendingState(nullptr)
{
}

Renderer::~Renderer(void)
{
	shutdown();
}

bool
Renderer::initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd,
					 bool fullscreen, float screenDepth, float screenNear)
{
	deviceContext = nullptr;
	rasterizerState = nullptr;
	rasterizerWireState = nullptr;
	rasterizerSolidNoCullState = nullptr;
	_screenWidth = screenWidth;
	_screenHeight = screenHeight;
	vsyncEnable = vsync;
	if (checkVideoCard() == false)
		return false;
	if (initSwapChain(hwnd, fullscreen) == false)
		return false;
	if (initDepthBuffer() == false)
		return false;
	if (initRasterizer() == false)
		return false;
	worldMatrix = DirectX::XMMatrixIdentity();
	ResizeProjection(screenWidth, screenHeight, screenDepth, screenNear);
	if (init2DStencil() == false)
		return false;
	if (initBlendStates() == false)
		return false;
	return true;
}

bool
Renderer::checkVideoCard(void)
{
	IDXGIFactory*					factory;
	IDXGIAdapter*					adapter;
	IDXGIOutput*					adapterOutput;
	DXGI_ADAPTER_DESC				adapterDesc;
	size_t							numModes, stringLenght;

	if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**) &factory)))
		return false;
	if (FAILED(factory->EnumAdapters(0, &adapter)))
	{
		factory->Release();
		return false;
	}
	if (FAILED(adapter->EnumOutputs(0, &adapterOutput)))
	{
		adapter->Release();
		factory->Release();
		return false;
	}
	if (FAILED(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED,
												 &numModes, nullptr)))
	{
		adapterOutput->Release();
		adapter->Release();
		factory->Release();
		return false;
	}
	auto displayModeList = std::unique_ptr<DXGI_MODE_DESC>(new (std::nothrow) DXGI_MODE_DESC[numModes]);
	if (FAILED(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED,
												 &numModes, displayModeList.get())))
	{
		displayModeList.reset();
		adapterOutput->Release();
		adapter->Release();
		factory->Release();
		return false;
	}
	for (size_t i = 0 ; i < numModes ; ++i)
		if (displayModeList.get()[i].Width == _screenWidth)
			if (displayModeList.get()[i].Height == _screenHeight)
			{
				_numerator = displayModeList.get()[i].RefreshRate.Numerator;
				_denominator = displayModeList.get()[i].RefreshRate.Denominator;
			}
	if (FAILED(adapter->GetDesc(&adapterDesc)))
	{
		displayModeList.reset();
		adapterOutput->Release();
		adapter->Release();
		factory->Release();
		return false;
	}
	videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);
	auto error = wcstombs_s(&stringLenght, videoCardDescription, 128, adapterDesc.Description, 128);
	displayModeList.reset();
	adapterOutput->Release();
	adapter->Release();
	factory->Release();
	if (error != 0)
			return false;
	return true;
}

bool
Renderer::initSwapChain(HWND hwnd, bool fullscreen)
{
	DXGI_SWAP_CHAIN_DESC			swapChainDesc;
	D3D_FEATURE_LEVEL				featureLevel = D3D_FEATURE_LEVEL_11_0;
	ID3D11Texture2D*				backBufferPtr;

	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = _screenWidth;
	swapChainDesc.BufferDesc.Height = _screenHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	if (vsyncEnable)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = _numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = _denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = !fullscreen;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;
	if(FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, &featureLevel,
											1, D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, nullptr,
											&deviceContext)))
		return false;
	if (FAILED(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr)))
		return false;
	if (FAILED(device->CreateRenderTargetView(backBufferPtr, nullptr, &renderTargetView)))
	{
		backBufferPtr->Release();
		return false;
	}
	backBufferPtr->Release();
	return true;
}

bool
Renderer::initDepthBuffer(void)
{
	D3D11_TEXTURE2D_DESC			depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC		depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC	depthStencilViewDesc;

	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	depthBufferDesc.Width = _screenWidth;
	depthBufferDesc.Height = _screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;
	if (FAILED(device->CreateTexture2D(&depthBufferDesc, nullptr, &depthStencilBuffer)))
		return false;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	if (FAILED(device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState)))
		return false;
	deviceContext->OMSetDepthStencilState(depthStencilState, 1);
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	if (FAILED(device->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &depthStencilView)))
		return false;
	deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
	return true;
}

bool
	Renderer::initRasterizer(void)
{
	D3D11_RASTERIZER_DESC			rasterDesc;

	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;
	if (FAILED(device->CreateRasterizerState(&rasterDesc, &rasterizerState)))
		return false;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
	if (FAILED(device->CreateRasterizerState(&rasterDesc, &rasterizerWireState)))
		return false;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	if (FAILED(device->CreateRasterizerState(&rasterDesc, &rasterizerSolidNoCullState)))
		return false;
	deviceContext->RSSetState(rasterizerWireState);
	return true;
}

void
Renderer::ResizeProjection(int screenWidth, int screenHeight, float screenDepth, float screenNear)
{
	float	fieldOfView = DirectX::XM_PI / 4.0f;
	float	screenAspect = (float)screenWidth / (float)screenHeight;

	projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);
	orthoMatrix = DirectX::XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight, screenNear, screenDepth);
}

bool
Renderer::init2DStencil(void)
{
	D3D11_DEPTH_STENCIL_DESC		depthDisabledStencilDesc;
	
	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));
	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	if(FAILED(device->CreateDepthStencilState(&depthDisabledStencilDesc, &depthDisabledStencilState)))
		return false;
	return true;
}

bool
	Renderer::initBlendStates(void)
{
	D3D11_BLEND_DESC				blendStateDescription;

	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;
	if(FAILED(device->CreateBlendState(&blendStateDescription, &alphaEnableBlendingState)))
		return false;
	blendStateDescription.RenderTarget[0].BlendEnable = FALSE;
	if(FAILED(device->CreateBlendState(&blendStateDescription, &alphaDisableBlendingState)))
		return false;
	return true;
}


void
Renderer::shutdown(void)
{
	if (alphaDisableBlendingState)
	{
		alphaDisableBlendingState->Release();
		alphaDisableBlendingState = nullptr;
	}
	if (alphaEnableBlendingState)
	{
		alphaEnableBlendingState->Release();
		alphaEnableBlendingState = nullptr;
	}
	if (depthDisabledStencilState)
	{
		depthDisabledStencilState->Release();
		depthDisabledStencilState = nullptr;
	}
	if(swapChain)
	{
		swapChain->SetFullscreenState(false, NULL);
	}
	if(rasterizerWireState)
	{
		rasterizerWireState->Release();
		rasterizerWireState = nullptr;
	}
	if(rasterizerState)
	{
		rasterizerState->Release();
		rasterizerState = nullptr;
	}
	if(depthStencilView)
	{
		depthStencilView->Release();
		depthStencilView = nullptr;
	}
	if(depthStencilState)
	{
		depthStencilState->Release();
		depthStencilState = nullptr;
	}
	if(depthStencilBuffer)
	{
		depthStencilBuffer->Release();
		depthStencilBuffer = nullptr;
	}
	if(renderTargetView)
	{
		renderTargetView->Release();
		renderTargetView = nullptr;
	}
	if(deviceContext)
	{
		deviceContext->Release();
		deviceContext = nullptr;
	}
	if(device)
	{
		device->Release();
		device = nullptr;
	}
	if(swapChain)
	{
		swapChain->Release();
		swapChain = nullptr;
	}
}

void
Renderer::beginScene(float red, float green, float blue, float alpha)
{
	float color[4];

	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	deviceContext->ClearRenderTargetView(renderTargetView, color);
	ClearDepth();
}

void
Renderer::endScene(void)
{
	if(vsyncEnable)
	{
		swapChain->Present(1, 0);
	}
	else
	{
		swapChain->Present(0, 0);
	}
}

ID3D11Device*
Renderer::getDevice(void) const
{
	return device;
}

ID3D11DeviceContext*
Renderer::getDeviceContext(void) const
{
	return deviceContext;
}

void
Renderer::getVideoCardInfo(char* cardName, int& memory) const
{
	strcpy_s(cardName, 128, videoCardDescription);
	memory = videoCardMemory;
}

void
Renderer::getProjectionMatrix(DirectX::XMMATRIX& matrix)
{
	matrix = projectionMatrix;
}

void
Renderer::getWorldMatrix(DirectX::XMMATRIX& matrix)
{
	matrix = worldMatrix;
}

void
Renderer::getOrthoMatrix(DirectX::XMMATRIX& matrix)
{
	matrix = orthoMatrix;
}

void
Renderer::TurnZBufferOn(void)
{
	deviceContext->OMSetDepthStencilState(depthStencilState, 1);
	return;
}

void
Renderer::TurnZBufferOff()
{
	deviceContext->OMSetDepthStencilState(depthDisabledStencilState, 1);
	return;
}

void
Renderer::TurnOnAlphaBlending(void)
{
	float blendFactor[4];
	
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;
	
	deviceContext->OMSetBlendState(alphaEnableBlendingState, blendFactor, 0xffffffff);
}

void
Renderer::TurnOffAlphaBlending(void)
{
	float blendFactor[4];
	
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;
	
	deviceContext->OMSetBlendState(alphaDisableBlendingState, blendFactor, 0xffffffff);
}

void
Renderer::SetRasterizer(D3D11_FILL_MODE fill, D3D11_CULL_MODE cull)
{
	if (fill == D3D11_FILL_SOLID)
	{
		if (cull == D3D11_CULL_BACK)
			deviceContext->RSSetState(rasterizerState);
		else
			deviceContext->RSSetState(rasterizerSolidNoCullState);
	}
	else
		deviceContext->RSSetState(rasterizerWireState);

}

int	Renderer::getScreenWidth() const
{
	return this->_screenWidth;
}

int	Renderer::getScreenHeight() const
{
	return this->_screenHeight;
}

bool Renderer::Resize(int height, int width)
{
	DXGI_MODE_DESC resize;

	_screenHeight = height;
	_screenWidth = width;
	resize.Height = height;
	resize.Width = width;
	resize.RefreshRate.Denominator = _denominator;
	resize.RefreshRate.Numerator = _numerator;
	resize.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	resize.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	resize.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChain->ResizeTarget(&resize);
	if (swapChain)
	{
		deviceContext->OMSetRenderTargets(0, 0, 0);
		renderTargetView->Release();
		HRESULT hr;
		hr = swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
		if (hr != S_OK)
			return false;
		ID3D11Texture2D* pBuffer;
		hr = swapChain->GetBuffer(0, __uuidof( ID3D11Texture2D), (void**) &pBuffer);
		if (hr != S_OK)
			return false;
		hr = device->CreateRenderTargetView(pBuffer, nullptr, &renderTargetView);
		if (hr != S_OK)
			return false;
		pBuffer->Release();
		deviceContext->OMSetRenderTargets(1, &renderTargetView, nullptr);
	}
	if(depthStencilView)
		depthStencilView->Release();
	return initDepthBuffer();
}

void
Renderer::ClearDepth(void)
{
	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}