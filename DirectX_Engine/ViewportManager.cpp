#include "ViewportManager.h"


ViewportManager::ViewportManager(void) : _activeView(1)
{
}


ViewportManager::~ViewportManager(void)
{
}

void
ViewportManager::init(int width, int height, bool resetCamera)
{
	if (width != -1 && height != -1)
	{
		_width = width;
		_height = height;
	}
	if (resetCamera)
	{
		for (int i = 0 ; i < _MAX_VIEWS ; ++i)
			_views[i].camera = nullptr;
		_selectedView = 1;
	}
	initViewports();
}

void
ViewportManager::initViewports(void)
{
	float	lWidth;
	float	lHeight;

	if (_activeView == 1)
		_divWidth = _divHeight = _activeView;
	else if (_activeView == 2 || _activeView == 3)
	{
		_divWidth = 2;
		_divHeight = 1;
	}
	else if (_activeView == 4)
		_divWidth = _divHeight = 2;
	lWidth = (float)(_width / _divWidth);
	lHeight = (float)(_height / _divHeight);		
	for (size_t i = 0; i < _activeView; ++i)
	{
		if (_activeView == 3 && i == 1)
			lHeight = (float)(_height / 2);
		this->_views[i].viewport.Width = lWidth;
		this->_views[i].viewport.Height = lHeight;
		this->_views[i].viewport.MaxDepth = 1.0f;
		this->_views[i].viewport.MinDepth = 0.0f;
	}
	this->_views[0].viewport.TopLeftX = 0.0f;
	this->_views[0].viewport.TopLeftY = 0.0f;
	this->_views[1].viewport.TopLeftX = lWidth;
	this->_views[1].viewport.TopLeftY = 0.0f;
	this->_views[2].viewport.TopLeftX = lWidth;
	this->_views[2].viewport.TopLeftY = lHeight;
	this->_views[3].viewport.TopLeftX = 0.0f;
	this->_views[3].viewport.TopLeftY = lHeight;
}

void
ViewportManager::incrementView(int inc)
{
	_activeView += inc;
	if (_activeView > _MAX_VIEWS)
		_activeView = _MAX_VIEWS;
	if (_activeView < 1)
		_activeView = 1;
	initViewports();
}

void
ViewportManager::setMovementController(int index, MovementController* controller)
{
	if (index >= _MAX_VIEWS)
		return;
	for (View& it : _views)
	{
		if (it.camera == controller)
			it.camera = nullptr;
	}
	if (_views[index].camera != nullptr && _views[index].camera != controller)
		_views[index].camera->viewport = -1;
	_views[index].camera = controller;
}

size_t
ViewportManager::getNbView(void)
{
	return _activeView;
}

D3D11_VIEWPORT*
ViewportManager::getView(size_t index)
{
	if (index >= _activeView)
		return nullptr;
	return &_views[index].viewport;
}

MovementController*
ViewportManager::getCamera(size_t index)
{
	if (index >= _activeView)
		return nullptr;
	return _views[index].camera;
}

MovementController*
ViewportManager::getSelectedCamera(void)
{
	return _views[_selectedView].camera;
}

void
ViewportManager::getPosition(size_t index, DirectX::XMVECTOR& pos)
{
	DirectX::XMFLOAT3	v;
	DirectX::XMFLOAT3	zero(0.0f, 0.0f, 0.0f);

	if (_views[index].camera != nullptr)
	{
		_views[index].camera->getPos(v);
		pos = DirectX::XMLoadFloat3(&v);
	}
	else
		pos = DirectX::XMLoadFloat3(&zero);
}

void
ViewportManager::selectCamera(int x, int y)
{
	if (x < _width / _divWidth)
	{
		if (y < _height / _divHeight)
			_selectedView = 0;
		else
			_selectedView = 3;
	}
	else
	{
		if (_activeView != 3)
		{
			if (y < _height / _divHeight)
				_selectedView = 1;
			else
				_selectedView = 2;
		}
		else
		{
			if (y < _height / 2)
				_selectedView = 1;
			else
				_selectedView = 2;
		}
	}
}

D3D11_VIEWPORT *	ViewportManager::getSelectedView()
{
	return &this->_views[this->_selectedView].viewport;
}

void	ViewportManager::wireframe(int cam, bool wireframe)
{
	if (cam < 0 || cam > 4)
		return ;

	this->_views[cam].wireframe = wireframe;
}

bool	ViewportManager::isWireframe(int cam) const
{
	if (cam < 0 || cam > 4)
		return false;

	return this->_views[cam].wireframe;
}