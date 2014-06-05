#pragma once

#include <Windows.h>
#include <dxgi.h>
#include <d3d11.h>
#include <d3dcommon.h>
#include <DirectXMath.h>
#include <memory>

class	Renderer
{
public:
	Renderer(void);
	~Renderer(void);
	bool	initialize(int, int, bool, HWND, bool, float, float);
	void	shutdown(void);
	void	beginScene(float, float, float, float);
	void	endScene(void);

	ID3D11Device*			getDevice(void) const;
	ID3D11DeviceContext*	getDeviceContext(void) const;
	void					getVideoCardInfo(char*, int&) const;
	void					getProjectionMatrix(DirectX::XMMATRIX& matrix);
	void					getWorldMatrix(DirectX::XMMATRIX& matrix);
	void					getOrthoMatrix(DirectX::XMMATRIX& matrix);
	void					TurnZBufferOn(void);
	void					TurnZBufferOff(void);
	void					TurnOnAlphaBlending(void);
	void					TurnOffAlphaBlending(void);
	static void				SetRasterizer(D3D11_FILL_MODE fill, D3D11_CULL_MODE cull);
	D3D11_VIEWPORT *		getViewports();
	int						getScreenWidth() const;
	int						getScreenHeight() const;
	bool					Resize(int height, int width);
	void					ResizeProjection(int screenWidth, int screenHeight, float screenDepth, float screenNear);
	void					ClearDepth(void);

private:
	size_t	_numerator;
	size_t	_denominator;
	int		_screenWidth;
	int		_screenHeight;
	bool	vsyncEnable;
	int		videoCardMemory;
	char	videoCardDescription[128];

	IDXGISwapChain*				swapChain;
	ID3D11Device*				device;
	static ID3D11DeviceContext*		deviceContext;
	ID3D11RenderTargetView*		renderTargetView;
	ID3D11Texture2D*			depthStencilBuffer;
	ID3D11DepthStencilState*	depthStencilState;
	ID3D11DepthStencilView*		depthStencilView;
	static ID3D11RasterizerState*		rasterizerState;
	static ID3D11RasterizerState*		rasterizerWireState;
	static ID3D11RasterizerState*		rasterizerSolidNoCullState;
	ID3D11DepthStencilState*	depthDisabledStencilState;
	ID3D11BlendState*			alphaEnableBlendingState;
	ID3D11BlendState*			alphaDisableBlendingState;

	DirectX::XMMATRIX			projectionMatrix;
	DirectX::XMMATRIX			worldMatrix;
	DirectX::XMMATRIX			orthoMatrix;

	bool	checkVideoCard(void);
	bool	initSwapChain(HWND hwnd, bool fullscreen);
	bool	initDepthBuffer(void);
	bool	initViewport(void);
	bool	init2DStencil(void);
	bool	initBlendStates(void);
	bool	initRasterizer(void);
};