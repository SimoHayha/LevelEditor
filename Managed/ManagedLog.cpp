#include "stdafx.h"
#include "ManagedLog.h"
#include "FeedbackLog.h"


namespace ManagedLib
{
	ManagedLog::ManagedLog()
	{
		_feedback = new FeedbackLog(this);
		_unmanaged = new UnmanagedLog(_feedback);
	}

	ManagedLog::!ManagedLog()
	{
		delete _unmanaged;
		delete _feedback;
	}

	ManagedLog::~ManagedLog()
	{
	}

	void
	ManagedLog::raiseValueChanged(int i)
	{
		if (_valueChanged != nullptr)
			_valueChanged->Invoke(i);
	}
}
