#include "Dictionnary.h"
#include "Log.hpp"

Dictionnary* Dictionnary::_this = nullptr;
id::FBX::Manager* Dictionnary::_fbxManager = nullptr;

Dictionnary::Dictionnary(void) : _feedback(nullptr)
{
}

Dictionnary::Dictionnary(Dictionnary const& right)
{
	(void)right;
}

Dictionnary::~Dictionnary(void)
{
	this->_destroy();
	this->kill();
}

Dictionnary*
Dictionnary::getInstance(void)
{
	if (_this == nullptr)
	{
		_this = new (std::nothrow) Dictionnary();
		if (_this == nullptr)
			return nullptr;
		_fbxManager = new (std::nothrow) id::FBX::Manager();
		if (_fbxManager == nullptr || !_fbxManager->init())
			kill();
	}
	return _this;
}

bool
Dictionnary::kill(void)
{
	if (_this != nullptr)
	{
		delete _this;
		_this = nullptr;
	}
	if (_fbxManager != nullptr)
	{
		delete _fbxManager;
		_fbxManager = nullptr;
	}
	return true;
}

bool
Dictionnary::_destroy(void)
{
	std::map<std::string, Texture*>::iterator				endText = _mapTextures.end();
	std::map<std::string, std::vector<Buffers*>*>::iterator	endBuf = _mapBuffers.end();
	std::map<std::string, LuaScript*>::iterator				endScripts = _mapScripts.end();

	for (std::map<std::string, Texture*>::iterator it = _mapTextures.begin(); it != endText; it++)
		delete it->second;
	for (std::map<std::string, std::vector<Buffers*>*>::iterator it = _mapBuffers.begin(); it != endBuf; it++)
	{
		std::vector<Buffers*>::iterator	endSubBuf = it->second->end();
		for (std::vector<Buffers*>::iterator itBuf = it->second->begin() ; itBuf != endSubBuf ; ++itBuf)
			delete *itBuf;
		delete it->second;
	}
	for (std::map<std::string, LuaScript*>::iterator it = _mapScripts.begin(); it != endScripts; it++)
		delete it->second;
	return true;
}

Dictionnary&
Dictionnary::operator=(const Dictionnary& right)
{
	return *this;
	(void)right;
}

bool
Dictionnary::setWorkingDirectory(const std::string& wd)
{
	try
	{
		_workingDirectory = wd;
	}
	catch (std::bad_alloc b)
	{
		b.what();
		return false;
	}
	return true;
}

id::FBX::Model*
Dictionnary::getFbxModel(const std::string& path)
{
	id::FBX::Model*	model;

	std::map<std::string, id::FBX::Model*>::iterator it =_mapFbxModel.find(path);
	if (it != _mapFbxModel.end())
		return it->second;
	logCall(log_level_t::LOG_INFO) << "Loading model " << path << Elog::flush;
	if (!_fbxManager->initScene(path.c_str()))
		return nullptr;
	model = new (std::nothrow) id::FBX::Model;
	if (model == nullptr)
		return nullptr;
	if (!model->init(_fbxManager->getSceneWithName(path)))
		return nullptr;
	try
	{
		_mapFbxModel.insert(std::pair<std::string, id::FBX::Model*>(path, model));
	}
	catch (std::bad_alloc)
	{
		return false;
	}
	if (_feedback != nullptr)
		_feedback->OnListChanged(0);
	return model;
}

Texture*
Dictionnary::getTexture(FbxFileTexture* file)
{
	return getTexture(file->GetFileName());
}

Texture*
Dictionnary::getTexture(const std::string& filename)
{
	Texture*	texture;

	if (filename == "")
		return nullptr;
	std::map<std::string, Texture*>::iterator it =_mapTextures.find(filename);
	if (it != _mapTextures.end())
		return it->second;
	logCall(log_level_t::LOG_INFO) << "Loading texture " << filename << Elog::flush;
	texture = loadTexture(filename);
	if (texture == nullptr)
		return nullptr;
	try
	{
		texture->name = filename;
		_mapTextures.insert(std::pair<std::string, Texture*>(filename, texture));
	}
	catch (std::bad_alloc e)
	{
		delete texture;
		e.what();
		return nullptr;
	}
	if (_feedback != nullptr)
		_feedback->OnListChanged(2);
	return texture;
}

Buffers*
Dictionnary::getBuffer(const std::string name, size_t index,
					   Buffers* customBuffer)
{
	std::map<std::string, std::vector<Buffers*>*>::iterator it =_mapBuffers.find(name);
	if (it != _mapBuffers.end())
	{
		if (index >= it->second->size())
			return nullptr;
		return (*it->second)[index];
	}
	id::FBX::Model* model = nullptr;
	size_t numMesh = 1;
	std::vector<Buffers*>*	vector;
	if (customBuffer == nullptr)
	{
		model = getFbxModel(name);
		numMesh = model->getMeshCount();
	}
	try
	{
		vector = new std::vector<Buffers*>();
		_mapBuffers.insert(std::pair<std::string, std::vector<Buffers*>*>(name, vector));
		if (customBuffer == nullptr)
			for (size_t i = 0 ; i < numMesh ; ++i)
				vector->push_back(new Buffers());
		else
			vector->push_back(customBuffer);
	}
	catch (std::bad_alloc)
	{
		return nullptr;
	}
	if (customBuffer == nullptr)
	{
		for (size_t i = 0 ; i < numMesh ; ++i)
		{
			id::FBX::Mesh&		mesh = model->getMesh(i);
			if (!(*vector)[i]->init(_device, D3D11_USAGE_DYNAMIC, sizeof(VertexType) * mesh.vertexCount,
									mesh.vertice, sizeof(unsigned long) * mesh.indexCount, mesh.indice))
				return nullptr;
		}
		if (index >= vector->size())
			return nullptr;
		customBuffer = (*vector)[index];
	}
	return customBuffer;
}

LuaScript*
Dictionnary::getScript(const std::string& name)
{
	LuaScript*	script;
	bool		initOk = true;
	
	if (name == "")
		return nullptr;
	std::map<std::string, LuaScript*>::iterator it =_mapScripts.find(name);
	if (it != _mapScripts.end())
		return it->second;
	logCall(log_level_t::LOG_DEBUG) << "Loading script " << name << Elog::flush;
	script = new (std::nothrow) LuaScript;
	if (script == nullptr)
		return nullptr;
	if (!script->init(name))
		return nullptr;
	try
	{
		_mapScripts.insert(std::pair<std::string, LuaScript*>(name, script));
	}
	catch (std::bad_alloc e)
	{
		delete script;
		e.what();
		return nullptr;
	}
	if (_feedback != nullptr)
		_feedback->OnListChanged(1);
	return script;
}

Texture*
Dictionnary::loadTexture(const std::string& filename)
{
	FbxString	na = filename.c_str();
	BSTR		unicodestr = SysAllocStringLen(nullptr, na.GetLen());
	Texture*	texture;

	texture = new (std::nothrow) Texture;
	if (texture == nullptr)
		return nullptr;
	::MultiByteToWideChar(CP_ACP, 0, na.Buffer(), na.GetLen(), unicodestr, na.GetLen());
	if (texture->initialize(_device, unicodestr) == false)
	{
		::SysFreeString(unicodestr);
		delete texture;
		MessageBox(_hwnd, L"Could not initialize texture, the program will continue without them.", L"Error", MB_OK);
		return nullptr;
	}
	::SysFreeString(unicodestr);
	return texture;
}

void
Dictionnary::setDevice(ID3D11Device* device, HWND hwnd)
{
	_device = device;
	_hwnd = hwnd;
}

const std::string*
Dictionnary::getNameFbxModels(bool& recall)
{
	static auto	it = _mapFbxModel.begin();
	static bool reset = true;
	const std::string*	ret = nullptr;

	if (reset)
		it = _mapFbxModel.begin();
	if (it == _mapFbxModel.end())
	{
		recall = false;
		reset = true;
		return nullptr;
	}
	else
	{
		ret = &it->first;
		++it;
		recall = true;
		reset = false;
		return ret;
	}
}

const std::string*
Dictionnary::getNameScripts(bool& recall)
{
	static auto	it = _mapScripts.begin();
	static bool reset = true;
	const std::string*	ret = nullptr;

	if (reset)
		it = _mapScripts.begin();
	if (it == _mapScripts.end())
	{
		recall = false;
		reset = true;
		return nullptr;
	}
	else
	{
		ret = &it->first;
		++it;
		recall = true;
		reset = false;
		return ret;
	}
}

const std::string*
Dictionnary::getNameTextures(bool& recall)
{
	static auto	it = _mapTextures.begin();
	static bool reset = true;
	const std::string*	ret = nullptr;

	if (reset)
		it = _mapTextures.begin();
	if (it == _mapTextures.end())
	{
		recall = false;
		reset = true;
		return nullptr;
	}
	else
	{
		ret = &it->first;
		++it;
		recall = true;
		reset = false;
		return ret;
	}
}

bool
Dictionnary::reloadScript(const std::string& file)
{
	if (file == "")
		return false;
	std::map<std::string, LuaScript*>::iterator it =_mapScripts.find(file);
	if (it != _mapScripts.end())
		return it->second->reload(file);
	else
		return false;
}