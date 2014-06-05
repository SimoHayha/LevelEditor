#include "UnmanagedPerf.h"

UnmanagedPerf::UnmanagedPerf(Feedback * feedback)
{
	_feedback = feedback;
	_dataProvider = NVDataProvider::get();
	_dataProvider->setUnmanaged(this);
}

int	UnmanagedPerf::getPrimitives()
{
	return this->primitives;
}

float	UnmanagedPerf::getGpuIdle()
{
	return this->gpuIdle;
}

void	UnmanagedPerf::setPrimitives(int i)
{
	this->primitives = i;
}

void	UnmanagedPerf::setGpuIdle(float f)
{
	this->gpuIdle = f;
}