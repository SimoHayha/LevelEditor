#pragma once

#include <vector>

#include "MovementController.h"

class ViewportManager
{
private:
	struct View
	{
		View()
		{
			this->camera = nullptr;
			this->wireframe = false;
		}

		MovementController*	camera;
		D3D11_VIEWPORT		viewport;
		bool				wireframe;
	};
	
	static const int	_MAX_VIEWS = 4;
	View				_views[_MAX_VIEWS];
	int					_width;
	int					_height;
	int					_divWidth;
	int					_divHeight;
	size_t				_activeView;
	size_t				_selectedView;
public:
	ViewportManager(void);
	~ViewportManager(void);

	void	init(int width = -1, int height = -1, bool resetCamera = true);
	void	initViewports(void);
	void	incrementView(int inc = 1);
	size_t	getNbView(void);
	void	setMovementController(int index, MovementController* controller);

	D3D11_VIEWPORT*		getView(size_t index);
	MovementController*	getCamera(size_t index);
	MovementController*	getSelectedCamera(void);
	void				getPosition(size_t index, DirectX::XMVECTOR& pos);
	void				selectCamera(int x, int y);
	D3D11_VIEWPORT *	getSelectedView();
	void				wireframe(int cam, bool wireframe);
	bool				isWireframe(int cam) const;
};

