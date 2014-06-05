#pragma once

#include <iostream>
#include <map>
#include <string>
#include <fbxsdk.h>

#include "FBX_Manager.h"
#include "FBX_Material.h"
#include "FBX_Model.h"
#include "Buffers.h"
#include "LuaScript.h"
#include "UnmanagedRessources.h"

class	Dictionnary
{
private:
	static Dictionnary*									_this;
	static id::FBX::Manager*							_fbxManager;
	std::map<std::string, Texture*>						_mapTextures;
	std::map<std::string, id::FBX::Model*>				_mapFbxModel;
	std::map<std::string, std::vector<Buffers*>*>		_mapBuffers;
	std::map<std::string, LuaScript*>					_mapScripts;
	std::string											_workingDirectory;
	ID3D11Device*										_device;
	HWND												_hwnd;
private:
	bool	_destroy(void);
	Dictionnary(void);
	Dictionnary(Dictionnary const& right);
	Dictionnary&	operator=(const Dictionnary& right);
	virtual	~Dictionnary(void);

	Texture*			loadTexture(const std::string& filename);

public:
	UnmanagedRessources::Feedback*	_feedback;
	static Dictionnary*	getInstance(void);
	static bool			kill(void);
	bool				setWorkingDirectory(const std::string& wd);
	void				setDevice(ID3D11Device* device, HWND hwnd);
	
	Texture*			getTexture(FbxFileTexture* file);
	Texture*			getTexture(const std::string& file);
	id::FBX::Model*		getFbxModel(const std::string& path);
	Buffers*			getBuffer(const std::string name, size_t index,
								  Buffers* customBuffer = nullptr);
	LuaScript*			getScript(const std::string& name);

	const std::string*		getNameScripts(bool& recall);
	const std::string*		getNameTextures(bool& recall);
	const std::string*		getNameFbxModels(bool& recall);

	bool	reloadScript(const std::string& file);
};