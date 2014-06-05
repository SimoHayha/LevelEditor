#include "Stdafx.h"

#include "ManagedPerf.h"
#include "FeedbackPerf.h"

namespace ManagedLib
{
	ManagedPerf::ManagedPerf()
	{
		_feedback = new FeedbackPerf(this);
		_unmanaged = new UnmanagedPerf(_feedback);
	}

	ManagedPerf::!ManagedPerf()
	{
		delete _feedback;
		delete _unmanaged;
	}

	ManagedPerf::~ManagedPerf()
	{
	}

	void	ManagedPerf::raiseSimpleUpdate(int primitives, float gpuIdle)
	{
		this->Primitives = primitives;
		this->GpuIdle = gpuIdle;
		if (_valueChanged != nullptr)
			_valueChanged->Invoke(0);
	}

	UnmanagedPerf *	ManagedPerf::getUnmanaged()
	{
		return _unmanaged;
	}
}