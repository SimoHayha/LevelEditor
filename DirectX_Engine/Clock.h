#pragma once

#include "Windows.h"
#include <windows.h>
#include <mmsystem.h>

class Clock
{	
	public:
		Clock();
		bool	init();
		float	update();
		size_t	getTimeSinceBegin();
		void	reset();
		float	getElapsedTime();
		int		getFPS();

	private:
		size_t	lastTime;
		size_t	startTime;
		float	elapsedTime;
		int		fps;
		int		fpsCount;
		size_t	lastFPS;
		size_t	actuaFPS;
};