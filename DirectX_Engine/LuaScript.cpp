#include "LuaScript.h"

LuaInput LuaScript::_input;


LuaScript::LuaScript(void) : _state(nullptr)
{
}


LuaScript::~LuaScript(void)
{
	if (_state != nullptr)
		lua_close(_state);
}


bool
LuaScript::init(const std::string& filename)
{
	_state = luaL_newstate();
	if (_state == nullptr)
		return nullptr;
	luaL_openlibs(_state);
	_entity.register_lua(_state);
	_input.register_lua(_state);
	if (luaL_dofile(_state, filename.c_str()) != LUA_OK)
	{
		lua_close(_state);
		_state = nullptr;
		return false;
	}
	lua_getglobal(_state, "Update");
	if (!lua_isfunction(_state,-1))
	{
		lua_pop(_state, 1);
		return false;
	}
	lua_pop(_state, 1);
	return true;
}

bool
LuaScript::reload(const std::string& filename)
{
	if (_state != nullptr)
	{
		lua_close(_state);
		_state = nullptr;
	}
	return init(filename);
}

bool
LuaScript::call(float dt)
{
	if (_state == nullptr)
		return true;
	lua_getglobal(_state, "Update");
	if (!lua_isfunction(_state,-1))
	{
		lua_pop(_state, 1);
		return false;
	}
	else
	{
		lua_pushnumber(_state, dt);
		if (lua_pcall(_state, 1, 0, 0) != LUA_OK)
		{
			char const *	s = lua_tostring(_state, -1);
		
			return false;
		}
	}
	return true;
}

void
LuaScript::loadFromEntity(Entity* entity)
{
	_entity.load(entity);
}

void
LuaScript::storeInEntity(Entity* entity)
{
	_entity.store(entity);
}

void
LuaScript::loadFromInput(Input* input)
{
	_input.load(input);
}