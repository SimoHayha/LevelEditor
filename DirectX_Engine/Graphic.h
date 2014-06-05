#pragma once

#include <memory>

#include "Renderer.h"
#include "ShaderManager.h"
#include "ViewportManager.h"
#include "FBX_Manager.h"
#include "FBX_Model.h"
#include "Clock.h"
#include "EntityManager.h"
#include "Dictionnary.h"

#include "NvDataProvider.h"

class Graphic
{
private:

	HWND						_hwnd;
	ViewportManager				_viewManager;
	Renderer					rdr;
	id::FBX::Manager*			fbxManager;
	ShaderManager*				shaders;
	Clock						clock;
	EntityManager				entityManager;
	Dictionnary*				dico;
	D3D11_FILL_MODE				fillMode;
	bool						isSelectionMode;

	NVDataProvider *			dataProvider;

	void	setViewport(unsigned int which);

public:
	static	bool const	VSYNC_ENABLE = true;
	static	float const SCREEN_DEPTH;
	static	float const SCREEN_NEAR;

	Graphic(void);
	~Graphic(void);
	bool	initialize(int, int, HWND);
	void	shutdown(void);
	bool	frame(void);
	void	setFbxManager(id::FBX::Manager* manager);
	MovementController *	getCamera(void);
	Renderer*			getRenderer(void);
	HWND				getHwnd(void);
	ShaderManager*		getShaders(void);
	EntityManager*		getEntityManager(void);
	ViewportManager*	getViewportManager(void);
	bool				printText(DirectX::XMMATRIX& ortho, DirectX::XMMATRIX& world);
	void				switchFillMode(void);
	void				selectCamera(int x, int y);
	int					getSelectedCamera() const;
	void				setCamera(Entity* camera, int viewport);
	void				viewportChange(int diff);
	void				pick(int sx, int sy);
	void				onMouseMoved(int mouseX, int mouseY);
	void				onMouseUp();
	void				unselect();
	void				actionMode(std::string & mode);
	void				wireframe(int cam, bool wireframe);
};