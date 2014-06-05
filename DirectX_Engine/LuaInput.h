#pragma once

#include <lua.hpp>
#include <luabridge.h>

#include "Input.h"

class LuaInput
{
public:
	LuaInput(void);
	~LuaInput(void);

	int	mouseX;
	int	mouseY;
	int	wheel;
	int	up;
	int	down;
	int	left;
	int	right;
	int action;

	void	load(Input* input);

	void register_lua(lua_State*);
};
