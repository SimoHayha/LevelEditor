#include "Clock.h"

Clock::Clock(void)
{
}

bool	Clock::init(void)
{
	this->startTime = GetTickCount();
	this->lastTime = this->startTime;
	this->lastFPS = this->startTime;
	this->elapsedTime = 0.0f;
	this->fps = 0;
	this->fpsCount = 0;
	return (true);
}

float	Clock::update(void)
{
	this->elapsedTime = (float)(GetTickCount() - lastTime);
	this->lastTime = GetTickCount();

	this->fpsCount++;
	if (GetTickCount() >= this->startTime + 1000)
	{
		this->fps = this->fpsCount;
		this->fpsCount = 0;

		this->startTime = GetTickCount();
	}
	return ((float)(this->elapsedTime) / 1000.0f);
}

size_t	Clock::getTimeSinceBegin(void)
{
	return (GetTickCount() - this->lastTime);
}

void	Clock::reset(void)
{
	this->startTime = GetTickCount();
	this->lastTime = this->startTime;
}

float	Clock::getElapsedTime(void)
{
	return (this->elapsedTime);
}

int		Clock::getFPS(void)
{
	return (this->fps);
}

