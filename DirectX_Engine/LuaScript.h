#pragma once

#include "LuaEntity.h"
#include "LuaInput.h"

class LuaScript
{
private:
	lua_State*	_state;
	LuaEntity	_entity;

	static LuaInput	_input;

public:
	LuaScript(void);
	virtual ~LuaScript(void);

	bool	init(const std::string& filename);
	bool	call(float dt);
	void	loadFromEntity(Entity* entity);
	void	storeInEntity(Entity* entity);
	bool	reload(const std::string& filename);

	static void loadFromInput(Input* input);
};

