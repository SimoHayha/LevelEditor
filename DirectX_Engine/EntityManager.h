#pragma once

#include <vector>
#include "Entity.h"
#include "Grid.h"
#include "Model.h"
#include "Light.h"
#include "Cube.h"
#include "FreeCamera.h"
#include "FpsCam.h"
#include "Dictionnary.h"
#include "Sphere.h"
#include "Field.h"
#include "GameTimer.h"
#include "Overlay.h"
#include "Container.h"
#include "Arrow.h"
#include "Linelist.h"

class Graphic;
class Camera;
class Renderer;
class TextureShader;


class EntityManager
{
private:
	Graphic *				graphic;
	std::vector<Entity*>	entity;
	std::vector<Entity*>	editorEntities;
	Dictionnary*			_dico;
	Entity *				selectedEntity;
	GameTimer				selectedEntityFeedbackTimer;
	Overlay					overlay;

	bool				initEntity(HWND hwnd, Entity* entity, ID3D11Device* device, const std::string& fbxModelPath);
	void				setSelectedEntity(Entity * entity);
	bool				overlayPicked(int sx, int sy, Renderer & renderer, D3D11_VIEWPORT * viewport, MovementController * cam);
	void				getRay(int sx, int sy, Renderer & renderer, D3D11_VIEWPORT * viewport,
							   MovementController * cam, Ray & ray, Entity& entity);
	void				removeEntityInVector(Entity* entity, std::vector<Entity*>& vec);
	void				getRecursiveLights(std::vector<Entity*>& entities, std::vector<Light*>& lights, int& count);
	void				recursivePick(int sx, int sy, Renderer & rdr, D3D11_VIEWPORT * viewport, MovementController * cam, Entity * entity, float & actualMin, Entity *& actualEntity);

public:
	EntityManager(void);
	~EntityManager(void);
	bool	initialize(Graphic* graphic, HWND hwnd);
	Entity*	addEntity(Entity::Type type, HWND hwnd, ID3D11Device* device = nullptr,
					  Entity* father = nullptr, const std::string& fbxModelPath = "", bool inEditor = false);
	void	removeEntity(Entity* entity);
	void	resetScene(void);
	std::vector<Entity*>&	getEntities(void);
	bool	refresh(float dt, Renderer* renderer);
	bool	render(float dt, Renderer* renderer, MovementController* camera);
	void	removeAll(Entity::Type type);
	bool	setLights(TextureShader* shader, ID3D11DeviceContext* context);
	size_t	getEntityNb(void);
	Entity*	getEntity(size_t index);
	Entity*	getEntityEditor(size_t index);
	void	pick(int sx, int sy, Renderer & renderer, D3D11_VIEWPORT * viewport, MovementController * cam);
	void	onMouseMoved(int sx, int sy, Renderer & rdr, D3D11_VIEWPORT * viewport, MovementController * cam);
	void	onMouseUp();
	void	unselect();
	void	actionMode(std::string & mode);
};