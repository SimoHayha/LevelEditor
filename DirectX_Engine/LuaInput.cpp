#include "LuaInput.h"


LuaInput::LuaInput(void)
{
}


LuaInput::~LuaInput(void)
{
}

void
LuaInput::load(Input* input)
{
	mouseX = input->getMousePosX();
	mouseY = input->getMousePosY();
	wheel = input->getWheel();
	up = input->isKeyDown('W');
	down = input->isKeyDown('S');
	right = input->isKeyDown('A');
	left = input->isKeyDown('D');
	action = input->isKeyDown('E');
}

void
LuaInput::register_lua(lua_State* L) {
    luabridge::getGlobalNamespace(L)
        .beginNamespace("Input")
				.addVariable("mouseX", &mouseX)
                .addVariable("mouseY", &mouseY)
                .addVariable("up", &up)
                .addVariable("down", &down)
                .addVariable("right", &right)
                .addVariable("left", &left)
				.addVariable("action", &action)
        .endNamespace();
}