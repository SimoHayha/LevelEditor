#pragma once

#ifdef UNMANAGED_EXPORTS
#define UNMANAGED_API __declspec(dllexport)
#else
#define UNMANAGED_API __declspec(dllimport)
#endif

#include <string>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

class SystemClass;
class UnmanagedEntity;

class UNMANAGED_API CUnmanaged
{
	SystemClass* system;
public:
	class UNMANAGED_API Feedback
	{
	public:
		virtual void OnClick(void) = 0;
	};

	CUnmanaged (Feedback* feedback);

	HWND	initForWPF(HWND parent, size_t width, size_t height);
	void	run(HWND parent);
	void	shutdown(HWND parent);
	void*	addEntity(int type, UnmanagedEntity* parent = nullptr, const std::string& path = "", bool inEditor = false);
	void*	addEntity(int type, void* parent, const std::string& path = "");
	void*	addEntityToChildModel(int type, void* model, size_t index, const std::string& path = "");
	void	removeEntity(UnmanagedEntity* entity);
	void	resetScene(void);
	size_t	getEntityNb(void);
	SystemClass*	getSystem(void);
	void			setEntity(UnmanagedEntity* entity, size_t index);
	void			setEntityEditor(UnmanagedEntity* entity, size_t index);
	void			setChildEntity(UnmanagedEntity* entity, UnmanagedEntity* parent, size_t index);
	void			setCamera(UnmanagedEntity* entity, int viewport);
	void			viewportChange(int diff);
	void			resize(int heigth, int width);
	void			unselect();
	void			actionChanged(std::string & action);
	void			wireframe(int cam, bool wireframe);
	void			loadVmf(std::string & path);

private:
	Feedback*	_feedback;
};