#pragma once

#include <lua.hpp>
#include <luabridge.h>

#include "Entity.h"

class LuaEntity
{
public:
	LuaEntity(void);
	~LuaEntity(void);

	float scaleX, scaleY, scaleZ;
	float rotX, rotY, rotZ;
	float transX, transY, transZ;

	void	load(Entity* entity);
	void	store(Entity* entity);

	void register_lua(lua_State*);
};

