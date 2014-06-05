#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Windowsx.h>
#include <Shobjidl.h>
#include <memory>
#include <fbxsdk.h>
#include <string>

#include "BspFile.h"
#include "Input.h"
#include "Graphic.h"
#include "FBX_Manager.h"
#include "Log.hpp"
#include "main.h"

class VmfNode;

class SystemClass
{
public:

	std::unique_ptr<Graphic>	graphic;

	SystemClass(void);
	HWND	initialize(HWND parent, size_t width, size_t height);
	void	shutdown(void);
	void	run(void);
	bool	getNameFromWindow(std::string& filename);
	LRESULT CALLBACK messageHandler(HWND, UINT, WPARAM, LPARAM);
	void	setFeedback(CUnmanaged::Feedback* feedback);
	bool	ResizeWindow(int height, int width);
	void	unselect();
	void	actionMode(std::string & mode);
	void	wireframe(int cam, bool wireframe);
	void	loadVmf(std::string & path);

private:
	bool	Frame();
	bool	initializeWindows(size_t& width, size_t& height, HWND parent);
	void	shutdownWindows(void);
	void	applyMouseInput(int mouseX, int mouseY);
	void	applyKeyboardInput();
	void	stopCamera();

	/* Vmf stuff */

	LPRECT						winPosition;
	LPCWSTR						appName;
	HINSTANCE					hInstance;
	HWND						hwnd;
	size_t						posX;
	size_t						posY;
	size_t						_width;
	size_t						_height;
	bool						_hasParent;
	Input						input;
	id::FBX::Manager			fbxManager;
	bool						switchFill;
	log_implementation::Log*	_log;
	CUnmanaged::Feedback*		_feedback;
	Dictionnary*				_dico;
	int							oldX;
	int							oldY;
	bool						locked;
	bool						active;
	BspFile						file;
};