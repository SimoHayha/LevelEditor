#pragma once

#ifdef UNMANAGED_EXPORTS
#define UNMANAGED_API __declspec(dllexport)
#else
#define UNMANAGED_API __declspec(dllimport)
#endif

#include "NvDataProvider.h"

class UNMANAGED_API UnmanagedPerf
{
public:
	NVDataProvider *	_dataProvider;

	class UNMANAGED_API Feedback
	{
	public:
		virtual void OnBasicUpdate(int primitives, float gpuIdle) = 0;
	};

	UnmanagedPerf(Feedback* feedback);

	Feedback*	_feedback;

	int		getPrimitives();
	float	getGpuIdle();

	void	setPrimitives(int);
	void	setGpuIdle(float);

private:
	int		primitives;
	float	gpuIdle;
};