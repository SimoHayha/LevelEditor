#include <d3d11_1.h>

#include "main.h"
#include "SystemClass.h"
#include "UnmanagedEntity.h"


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

CUnmanaged::CUnmanaged(Feedback* feedback)
{
	_feedback = feedback;
}


HWND	CUnmanaged::initForWPF(HWND parent, size_t width, size_t height)
{
	 system = new SystemClass();
	   
	 system->setFeedback(_feedback);

	 HWND	ret = system->initialize(parent, width, height);

	 return ret;
}

void	CUnmanaged::run(HWND parent)
{
	system->run();
}

void	CUnmanaged::shutdown(HWND parent)
{
	system->shutdown();
}

void*	CUnmanaged::addEntity(int type, UnmanagedEntity* parent, const std::string& path, bool inEditor)
{
	Entity*	child;
	Entity*	father = nullptr;
	if (parent != nullptr)
		father = parent->getEntity();
	child = system->graphic->getEntityManager()->addEntity(static_cast<Entity::Type>(type), system->graphic->getHwnd(),
														   system->graphic->getRenderer()->getDevice(), father, path, inEditor);
	return child;
}

void*	CUnmanaged::addEntity(int type, void* parent, const std::string& path)
{
	Entity*	child;
	Entity*	father = reinterpret_cast<Entity*>(parent);
	if (parent == nullptr)
		return nullptr;
	child = system->graphic->getEntityManager()->addEntity(static_cast<Entity::Type>(type), system->graphic->getHwnd(),
														   system->graphic->getRenderer()->getDevice(), father, path);
	return child;
}

void*	CUnmanaged::addEntityToChildModel(int type, void* model, size_t index, const std::string& path)
{
	Entity*	child;
	Entity*	entityModel = reinterpret_cast<Entity*>(model);
	Entity*	father = entityModel->getChild(index);
	if (entityModel == nullptr)
		return nullptr;
	child = system->graphic->getEntityManager()->addEntity(static_cast<Entity::Type>(type), system->graphic->getHwnd(),
														   system->graphic->getRenderer()->getDevice(), father, path);
	return child;
}

void	CUnmanaged::removeEntity(UnmanagedEntity* entity)
{
	system->graphic->getEntityManager()->removeEntity(entity->getEntity());
}

void	CUnmanaged::resetScene(void)
{
	system->graphic->getEntityManager()->resetScene();
}

size_t	CUnmanaged::getEntityNb(void)
{
	return system->graphic->getEntityManager()->getEntityNb();
}

void	CUnmanaged::setEntity(UnmanagedEntity* entity, size_t index)
{
	entity->setEntity(index, system->graphic->getEntityManager()->getEntity(index));
	entity->getEntity()->setUnmanaged(entity);
}

void	CUnmanaged::setEntityEditor(UnmanagedEntity* entity, size_t index)
{
	entity->setEntity(index, system->graphic->getEntityManager()->getEntityEditor(index));
	entity->getEntity()->setUnmanaged(entity);
}

void	CUnmanaged::setChildEntity(UnmanagedEntity* entity, UnmanagedEntity* parent, size_t index)
{
	entity->setEntity(index, parent->getEntity()->getChild(index));
	entity->getEntity()->setUnmanaged(entity);
}

void	CUnmanaged::setCamera(UnmanagedEntity* entity, int viewport)
{
	system->graphic->setCamera(entity->getEntity(), viewport);
}

void	CUnmanaged::viewportChange(int diff)
{
	system->graphic->viewportChange(diff);
}

void	CUnmanaged::resize(int heigth, int width)
{
	system->ResizeWindow(heigth, width);
}

void	CUnmanaged::unselect()
{
	system->unselect();
}

void	CUnmanaged::actionChanged(std::string & action)
{
	system->actionMode(action);
}

void	CUnmanaged::wireframe(int cam, bool wireframe)
{
	system->wireframe(cam, wireframe);
}

void	CUnmanaged::loadVmf(std::string & filename)
{
	this->system->loadVmf(filename);
}