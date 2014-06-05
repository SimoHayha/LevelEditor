#pragma once

#include <vcclr.h>
#include "ManagedLog.h"


class FeedbackLog : public UnmanagedLog::Feedback
{
public:
	FeedbackLog(ManagedLib::ManagedLog^ p);
	void OnFlush(int pValue);

private:
	gcroot<ManagedLib::ManagedLog^> _Managed;
};