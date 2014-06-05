#include "NvPmApi.h"
#include "Log.hpp"
#include "Graphic.h"

float const Graphic::SCREEN_DEPTH = 100000.0f;
float const Graphic::SCREEN_NEAR = 0.1f;

Graphic::Graphic() : fbxManager(nullptr),
					 shaders(nullptr),
					 fillMode(D3D11_FILL_WIREFRAME),
					 isSelectionMode(false)
{
}

Graphic::~Graphic()
{
}

bool
Graphic::initialize(int width, int heigth, HWND hwnd)
{
	DirectX::XMMATRIX baseViewMatrix;

	if (fbxManager == nullptr)
		return false;
	_hwnd = hwnd;
	if (rdr.initialize(width, heigth, VSYNC_ENABLE, hwnd, false, SCREEN_DEPTH, SCREEN_NEAR) == false)
	{
		MessageBox(hwnd, L"Could not initialize Dx11", L"Error", MB_OK);
		return false;
	}
	_viewManager.init(width, heigth);
	dico = Dictionnary::getInstance();
	if (dico == nullptr)
		return false;
	dico->setDevice(rdr.getDevice(), hwnd);
	shaders = new (std::nothrow) ShaderManager;
	if(!shaders)
		return false;
	if (shaders->init(rdr.getDevice(), hwnd) == false)
	{
		MessageBox(hwnd, L"Could not initialize the ShaderManager object.", L"Error", MB_OK);
		return false;
	}
	if (!Grid::initializeBuffers(rdr.getDevice(), 100, 100))
		return false;
	//if (!Cube::initializeBuffers(rdr.getDevice()))
	//	return false;
	//if (!Sphere::initializeBuffers(rdr.getDevice()))
	//	return false;
	if (entityManager.initialize(this, hwnd) == false)
		return false;
	if (!this->clock.init())
		return (false);

	this->dataProvider = NVDataProvider::get();
	if (!this->dataProvider)
		return false;
	if (NVPM_OK != this->dataProvider->setupCounter(this->rdr.getDevice()))
		return false;

	return true;
}

Renderer*
Graphic::getRenderer(void)
{
	return &rdr;
}

ShaderManager*
Graphic::getShaders(void)
{
	return shaders;
}

void
Graphic::shutdown(void)
{
	if (shaders != nullptr)
	{
		shaders->shutdown();
		delete shaders;
	}
}

bool
Graphic::frame(void)
{
	DirectX::XMMATRIX	viewMatrix;
	DirectX::XMMATRIX	worldMatrix;
	DirectX::XMMATRIX	projectionMatrix;
	DirectX::XMMATRIX	orthoMatrix;
	DirectX::XMVECTOR	posCamera;
	const D3D11_VIEWPORT*	viewport;
	float				dt = clock.update();

	this->dataProvider->updateCounterValue(dt);

	//float	idle = this->dataProvider.getGpuIdle();
	//int		primitives = this->dataProvider.getPrimitiveCount();

	rdr.beginScene(0.15f, 0.15f, 0.15f, 1.0f);
	rdr.SetRasterizer(fillMode, D3D11_CULL_BACK);

	if (entityManager.refresh(dt, &rdr) ==  false)
		return false;

	for (size_t i = 0 ; i < _viewManager.getNbView() ; ++i)
	{
		viewport = _viewManager.getView(i);
		rdr.ResizeProjection(static_cast<int>(viewport->Width), static_cast<int>(viewport->Height), SCREEN_DEPTH, SCREEN_NEAR);
		if (_viewManager.isWireframe(i))
			this->rdr.SetRasterizer(D3D11_FILL_WIREFRAME, D3D11_CULL_BACK);
		else
			this->rdr.SetRasterizer(D3D11_FILL_SOLID, D3D11_CULL_BACK);
		this->rdr.getDeviceContext()->RSSetViewports(1, viewport);
		_viewManager.getPosition(i, posCamera);
		shaders->getTextureShader()->setCameraConstBuffer(rdr.getDeviceContext(), posCamera);
		if (!entityManager.setLights(shaders->getTextureShader(), rdr.getDeviceContext()))
			return false;
		if (entityManager.render(dt, &rdr, _viewManager.getCamera(i)) ==  false)
			return false;
	}
	rdr.endScene();
	return true;
}

void
Graphic::setFbxManager(id::FBX::Manager* manager)
{
	fbxManager = manager;
}

MovementController *
Graphic::getCamera(void)
{
	return _viewManager.getSelectedCamera();
}

void
Graphic::switchFillMode(void)
{
	if (fillMode == D3D11_FILL_SOLID)
		fillMode = D3D11_FILL_WIREFRAME;
	else
		fillMode = D3D11_FILL_SOLID;
}

EntityManager*
Graphic::getEntityManager(void)
{
	return &entityManager;
}

void	Graphic::selectCamera(int x, int y)
{
	_viewManager.selectCamera(x, y);
}

HWND
Graphic::getHwnd(void)
{
	return _hwnd;
}

void
Graphic::setCamera(Entity* camera, int viewport)
{
	MovementController*	cam = dynamic_cast<MovementController*>(camera);
	if (cam == nullptr)
		return;
	cam->viewport = viewport - 1;
	if (viewport > 0)
		_viewManager.setMovementController(viewport - 1, cam);
}

void
Graphic::viewportChange(int diff)
{
	_viewManager.incrementView(diff);
}

ViewportManager*
Graphic::getViewportManager(void)
{
	return &_viewManager;
}

void	Graphic::pick(int sx, int sy)
{
	D3D11_VIEWPORT *		viewport;
	MovementController *	cam;

	if (this->isSelectionMode)
		return ;

	viewport = this->_viewManager.getSelectedView();
	cam = this->_viewManager.getSelectedCamera();

	if (!viewport || !cam)
	{
		logCall(log_level_t::LOG_WARNING) << "Can't perform pick" << Elog::flush;
		return ;
	}

	this->isSelectionMode = true;
	this->entityManager.pick(sx, sy, this->rdr, viewport, cam);
}

void	Graphic::onMouseMoved(int sx, int sy)
{
	D3D11_VIEWPORT *		viewport;
	MovementController *	cam;

	viewport = this->_viewManager.getSelectedView();
	cam = this->_viewManager.getSelectedCamera();

	if (!viewport || !cam)
	{
		logCall(log_level_t::LOG_WARNING) << "Can't perform mouse over" << Elog::flush;
		return ;
	}

	this->entityManager.onMouseMoved(sx, sy, this->rdr, viewport, cam);
}

void	Graphic::onMouseUp()
{
	this->isSelectionMode = false;
	this->entityManager.onMouseUp();
}

void	Graphic::unselect()
{
	this->entityManager.unselect();
}

void	Graphic::actionMode(std::string & mode)
{
	this->entityManager.actionMode(mode);
}

void	Graphic::wireframe(int cam, bool wireframe)
{
	this->_viewManager.wireframe(cam, wireframe);
}