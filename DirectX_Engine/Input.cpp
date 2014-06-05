#include "Input.h"

void
Input::initialize(void)
{
	keys.resize(256);
	for (size_t i = 0 ; i < 256 ; ++i)
		keys[i] = false;
	mouseButton.resize(3);
	for (size_t i = 0 ; i < 3 ; ++i)
		mouseButton[i] = false;
}

void
Input::keyDown(size_t key)
{
	keys[key] = true;
}

void
Input::keyUp(size_t key)
{
	keys[key] = false;
}

bool
Input::isKeyDown(size_t key) const
{
	return keys[key];
}

bool
Input::isMouseButtonDown(size_t button)
{
	return mouseButton[button];
}

void
Input::mouseMove(int x, int y)
{
	mousePosX = x;
	mousePosY = y;
}

void
Input::mouseButtonDown(size_t button)
{
	mouseButton[button] = true;
}

void
Input::mouseButtonUp(size_t button)
{
	mouseButton[button] = false;
}

int
Input::getMousePosX(void)
{
	return mousePosX;
}

int
Input::getMousePosY(void)
{
	return mousePosY;
}

void	Input::setWheel(short w)
{
	this->wheel = w;
}

short	Input::getWheel()
{
	/*short ret = this->wheel;

	if (this->wheel != 0)
		this->wheel = 0;

	return ret;*/
	short	ret;

	if (this->wheel != 0)
		ret = this->wheel;
	else
		ret = 0;
	
	this->wheel = 0;

	return ret;
}