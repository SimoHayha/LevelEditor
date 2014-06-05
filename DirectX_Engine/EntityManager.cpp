#include "EntityManager.h"
#include "Graphic.h"
#include "RayCaster.h"
#include "Renderer.h"
#include "Log.hpp"

EntityManager::EntityManager(void) :
	selectedEntity(nullptr)
{
}


EntityManager::~EntityManager(void)
{
	for (size_t i = 0; i < entity.size(); ++i)
		delete entity[i];
	for (size_t i = 0; i < editorEntities.size(); ++i)
		delete editorEntities[i];
}

bool
EntityManager::initialize(Graphic*	graphic, HWND hwnd)
{
	ID3D11Device*	device = graphic->getRenderer()->getDevice();

	_dico = Dictionnary::getInstance();
	if (_dico == nullptr)
		return false;
	this->graphic = graphic;
	if (false == this->overlay.initialize(graphic->getRenderer()->getDevice(), graphic->getShaders()))
		return false;

	if (false == this->addEntity(Entity::E_GRID, hwnd, device))
		return false;
	if (false == this->addEntity(Entity::E_CUBE, hwnd, device))
		return false;

	return true;
}

Entity*
EntityManager::addEntity(Entity::Type type, HWND hwnd, ID3D11Device* device,
						 Entity* father, const std::string& fbxModelPath, bool inEditor)
{
	Entity*	newEntity;

	try
	{
		if (type == Entity::E_MODEL)
			newEntity = new Container;
		else if (type == Entity::E_GRID)
			newEntity = new Grid;
		else if (type == Entity::E_LIGHT)
			newEntity = new Light;
		else if (type == Entity::E_CUBE)
			newEntity = new Cube;
		else if (type == Entity::E_SPHERE)
			newEntity = new Sphere;
		else if (type == Entity::E_CAMERA)
			newEntity = new FpsCam;
		else if (type == Entity::E_FIELD)
			newEntity = new Field;
		else if (type == Entity::E_CONTAINER)
			newEntity = new Container;
		else if (type == Entity::E_LINELIST)
			newEntity = new Linelist;
	}
	catch (std::bad_alloc b)
	{
		b.what();
		return false;
	}
	if (!this->initEntity(hwnd, newEntity, device, fbxModelPath))
	{
		delete newEntity;
		return false;
	}
	if (nullptr == father)
	{
		try
		{
			if (!inEditor)
				this->entity.push_back(newEntity);
			else
				this->editorEntities.push_back(newEntity);
		}
		catch (std::bad_alloc &)
		{
			delete newEntity;
			return false;
		}
	}
	else
	{
		if (!father->appendChild(newEntity))
		{
			delete newEntity;
			return false;
		}
	}
	newEntity->setShader(this->graphic->getShaders());
	return newEntity;
}

bool
EntityManager::initEntity(HWND hwnd, Entity* entity, ID3D11Device* device, const std::string& fbxModelPath)
{
	if (entity->getType() == Entity::E_GRID)
	{
		Grid*	grid = dynamic_cast<Grid*>(entity);
		if (grid->init(device) == false)
		{
			MessageBox(hwnd, L"Could not initialize the grid object.", L"Error", MB_OK);
			return false;
		}
	}
	else if (entity->getType() == Entity::E_CUBE)
	{
		Cube*	cube = dynamic_cast<Cube*>(entity);
		if (!cube->initializeBuffers(device))
			return false;
		if (cube->init(device) == false)
		{
			MessageBox(hwnd, L"Could not initialize the cube object.", L"Error", MB_OK);
			return false;
		}
	}
	else if (entity->getType() == Entity::E_SPHERE)
	{
		Sphere*	sphere = dynamic_cast<Sphere*>(entity);
		if (!sphere->initializeBuffers(device))
			return false;
		if (sphere->init(device) == false)
		{
			MessageBox(hwnd, L"Could not initialize the sphere object.", L"Error", MB_OK);
			return false;
		}
	}
	else if (entity->getType() == Entity::E_LIGHT)
	{
		Light*	light = dynamic_cast<Light*>(entity);
		if (!light->initialize())
		{
			MessageBox(hwnd, L"Could not initialize the light object.", L"Error", MB_OK);
			return false;
		}
	}
	else if (entity->getType() == Entity::E_FIELD)
	{
		Field*	field = dynamic_cast<Field*>(entity);
		if (!field->initializeBuffers(device))
			return false;
		if (!field->init(device))
		{
			MessageBox(hwnd, L"Could not initialize the light object.", L"Error", MB_OK);
			return false;
		}
	}
	else if (entity->getType() == Entity::E_CONTAINER)
	{
		Container *	container = dynamic_cast<Container *>(entity);

		if (!container->initialize(device))
			return false;
		if (fbxModelPath != "")
		{
			Model*			newEntity;
			id::FBX::Model* fbxModel;
			Buffers*		buffer;

			fbxModel = _dico->getFbxModel(fbxModelPath);
			if (fbxModel == nullptr)
				return false;
			size_t	numMesh = fbxModel->getMeshCount();
			for (size_t i = 0 ; i < numMesh ; ++i)
			{
				newEntity = new (std::nothrow) Model();
				if (newEntity == nullptr)
					return false;
				if (!container->appendChild(newEntity))
					return false;
				buffer = _dico->getBuffer(fbxModelPath, i);
				if (buffer == nullptr)
					return false;
				if(newEntity->initialize(device, fbxModel, buffer, i) == false)
				{
					MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
					return false;
				}
				newEntity->setShader(this->graphic->getShaders());
			}
			container->setName(fbxModelPath);
		}
	}
	return entity->setFilePath(fbxModelPath);
}

void
EntityManager::removeEntity(Entity* entity)
{
	if (this->overlay.getFather() == entity)
		this->overlay.unselect();

	if (this->selectedEntity == entity)
		this->selectedEntity = nullptr;

	if (entity->getFather())
		removeEntityInVector(entity, entity->getFather()->getChilds());
	else
		removeEntityInVector(entity, this->entity);
}

void
EntityManager::removeEntityInVector(Entity* entity, std::vector<Entity*>& vec)
{
	for (auto it = vec.begin() ; it != vec.end() ; ++it)
		if (*it == entity)
		{
			delete *it;
			vec.erase(it);
			break;
		}
}

std::vector<Entity*>&
EntityManager::getEntities(void)
{
	return entity;
}

bool
EntityManager::refresh(float dt, Renderer* renderer)
{
	ID3D11DeviceContext*	deviceContext = renderer->getDeviceContext();
	Entity *				bind = nullptr;

	this->selectedEntityFeedbackTimer.tick();

	for (size_t i = 0; i < entity.size(); ++i)
	{
		if (entity[i]->childRefresh(deviceContext, dt, bind) == false)
			return false;
	}

	for (size_t i = 0; i < editorEntities.size(); ++i)
	{
		if (editorEntities[i]->childRefresh(deviceContext, dt, bind) == false)
			return false;
	}

	if (bind != this->selectedEntity)
		this->setSelectedEntity(bind);

	if (this->selectedEntityFeedbackTimer.deltaTime() * 1000.0f > 0.25f)
	{
		if (this->selectedEntity)
			this->selectedEntity->onFeedback();
		this->selectedEntityFeedbackTimer.stop();
		this->selectedEntityFeedbackTimer.start();
	}

	return true;
}

bool
EntityManager::render(float dt, Renderer* renderer, MovementController* camera)
{
	DirectX::XMMATRIX		viewMatrix;
	DirectX::XMMATRIX		worldMatrix;
	DirectX::XMMATRIX		projectionMatrix;
	ID3D11DeviceContext*	deviceContext = renderer->getDeviceContext();
	std::vector<DirectX::XMMATRIX*>	parent;

	if (camera == nullptr)
		return true;

	for (size_t i = 0; i < entity.size(); ++i)
	{
			parent.clear();
			camera->getViewMatrix(viewMatrix);
			renderer->getWorldMatrix(worldMatrix);
			renderer->getProjectionMatrix(projectionMatrix);
			entity[i]->childRender(deviceContext, dt, viewMatrix, worldMatrix, projectionMatrix, parent);
	}

	for (size_t i = 0; i < editorEntities.size(); ++i)
	{
			parent.clear();
			camera->getViewMatrix(viewMatrix);
			renderer->getWorldMatrix(worldMatrix);
			renderer->getProjectionMatrix(projectionMatrix);
			editorEntities[i]->childRender(deviceContext, dt, viewMatrix, worldMatrix, projectionMatrix, parent);
	}
	renderer->ClearDepth();
	if (this->overlay.canRender())
		this->overlay.childRender(deviceContext, dt, viewMatrix, worldMatrix, projectionMatrix, parent);
	return true;
}

bool
EntityManager::setLights(TextureShader* shader, ID3D11DeviceContext* context)
{
	const int	LIGHT_COUNT = 8;
	int			count;

	std::vector<Light*>	lights;
	try
	{
		lights.reserve(LIGHT_COUNT);
	}
	catch (std::bad_alloc)
	{
		return false;
	}
	count = LIGHT_COUNT;
	getRecursiveLights(entity, lights, count);
	shader->setLightConstBuffer(context, lights);
	return true;
}

void
EntityManager::getRecursiveLights(std::vector<Entity*>& entities, std::vector<Light*>& lights, int& count)
{
	for (Entity* it : entities)
	{
		if (it->getType() == Entity::E_LIGHT)
		{
			lights.push_back(dynamic_cast<Light*>(it));
			--count;
		}
		else
			getRecursiveLights(it->getChilds(), lights, count);
		if (count == 0)
			return;
	}
}

size_t
EntityManager::getEntityNb(void)
{
	return entity.size();
}

Entity*
EntityManager::getEntity(size_t index)
{
	return entity[index];
}

Entity*
EntityManager::getEntityEditor(size_t index)
{
	return editorEntities[index];
}

void
EntityManager::resetScene(void)
{
	for (size_t i = 0; i < entity.size(); ++i)
		delete entity[i];
	entity.clear();
}

void	EntityManager::recursivePick(int sx, int sy, Renderer & rdr, D3D11_VIEWPORT * viewport, MovementController * cam, Entity * entity, float & actualMin, Entity *& actualEntity)
{
	using namespace DirectX;

	XMMATRIX	P;
	XMMATRIX	V;
	XMMATRIX	invView;
	XMMATRIX	invWorld;
	XMFLOAT3	pos;

	rdr.getProjectionMatrix(P);

	float vx = (+2.0f * sx / viewport->Width  - 1.0f) / P.r[0].m128_f32[0];
	float vy = (-2.0f * sy / viewport->Height + 1.0f) / P.r[1].m128_f32[1];

	cam->getViewMatrix(V);
	invView = XMMatrixInverse(&XMMatrixDeterminant(V), V);

	Ray	ray = RayCaster::getRay(vx, vy, invView, invWorld, *entity);

	if (RayCaster::intersect(ray, *entity, actualMin))
		actualEntity = entity;

	for (auto it : entity->getChilds())
		this->recursivePick(sx, sy, rdr, viewport, cam, it, actualMin, actualEntity);
}

void	EntityManager::pick(int sx, int sy, Renderer & rdr, D3D11_VIEWPORT * viewport, MovementController * cam)
{

	if (this->overlayPicked(sx, sy, rdr, viewport, cam))
		return ;

	float		actualMin = FLT_MAX;
	Entity *	actualEntity = nullptr;

	for (auto it : entity)
		this->recursivePick(sx, sy, rdr, viewport, cam, it, actualMin, actualEntity);

	this->setSelectedEntity(actualEntity);
}

void	EntityManager::setSelectedEntity(Entity * entity)
{
 	this->selectedEntityFeedbackTimer.stop();
	if (this->selectedEntity)
	{
		this->selectedEntity->setSelected(false);
		this->overlay.unselect();
	}

	this->selectedEntity = entity;
	if (this->selectedEntity)
	{
		this->selectedEntity->setSelected(true);
	}

	this->overlay.bind(this->selectedEntity);
}

bool	EntityManager::overlayPicked(int sx, int sy, Renderer & rdr, D3D11_VIEWPORT * viewport, MovementController * cam)
{
	using namespace DirectX;

	if (this->overlay.isAxisSelected())
		return true;
	
	float	actualMin = FLT_MAX;
	Arrow &	xAxis = this->overlay.getXAxis();
	Arrow &	yAxis = this->overlay.getYAxis();
	Arrow &	zAxis = this->overlay.getZAxis();

	Ray	ray;
	
	/* Intersect x ? */
	getRay(sx, sy, rdr, viewport, cam, ray, xAxis);
	if (RayCaster::intersect(ray, xAxis, actualMin))
	{
		this->overlay.setSelectedAxis(Overlay::SelectedAxis::E_XAXIS);
		return true;
	}
	/* Intersect y ? */
	getRay(sx, sy, rdr, viewport, cam, ray, yAxis);
	if (RayCaster::intersect(ray, yAxis, actualMin))
	{
		this->overlay.setSelectedAxis(Overlay::SelectedAxis::E_YAXIS);
		return true;
	}
	/* Intersect z ? */
	getRay(sx, sy, rdr, viewport, cam, ray, zAxis);
	if (RayCaster::intersect(ray, zAxis, actualMin))
	{
		this->overlay.setSelectedAxis(Overlay::SelectedAxis::E_ZAXIS);
		return true;
	}
	return false;
}

void	EntityManager::getRay(int sx, int sy, Renderer & renderer, D3D11_VIEWPORT * viewport,
							  MovementController * cam, Ray & ray, Entity& entity)
{
	using namespace DirectX;

	
	XMMATRIX	P;
	XMMATRIX	V;
	XMMATRIX	invView;
	XMMATRIX	invWorld;
	XMMATRIX	transform;
	XMFLOAT3	pos;
	float		actualMin = FLT_MAX;

	renderer.getProjectionMatrix(P);

	float vx = (+2.0f * sx / viewport->Width  - 1.0f) / P.r[0].m128_f32[0];
	float vy = (-2.0f * sy / viewport->Height + 1.0f) / P.r[1].m128_f32[1];

	cam->getViewMatrix(V);
	invView = XMMatrixInverse(&XMMatrixDeterminant(V), V);

	ray = RayCaster::getRay(vx, vy, invView, invWorld, entity);
}

void	EntityManager::onMouseMoved(int sx, int sy, Renderer & rdr, D3D11_VIEWPORT * viewport, MovementController * cam)
{
	if (this->selectedEntity == nullptr || !this->overlay.isAxisSelected())
		return ;

	using namespace DirectX;

	XMMATRIX	P;
	XMMATRIX	V;
	XMMATRIX	W;
	Ray	ray;
	XMFLOAT3	temp;
	XMVECTOR	origin;
	DirectX::XMVECTOR	scale;
	DirectX::XMVECTOR	rot;
	DirectX::XMVECTOR	pos;

	rdr.getWorldMatrix(W);
	rdr.getProjectionMatrix(P);
	cam->getPos(temp);
	cam->getViewMatrix(V);
	origin = DirectX::XMLoadFloat3(&temp);
	this->overlay.getFather()->getGlobalTransformMatrix(W);
	DirectX::XMMatrixDecompose(&scale, &rot, &pos, W);
	rdr.getWorldMatrix(W);
	W *= DirectX::XMMatrixTranslation(pos.m128_f32[0], pos.m128_f32[1], pos.m128_f32[2]);
	
	RayCaster::getRay(sx, sy, P, V, W, origin, viewport, ray);
	this->overlay.onMouseMoved(ray);
}

void	EntityManager::onMouseUp()
{
	this->overlay.onMouseUp();
}

void	EntityManager::unselect()
{
	this->setSelectedEntity(nullptr);
}

void	EntityManager::actionMode(std::string & mode)
{
	this->overlay.setActionMode(mode);
}