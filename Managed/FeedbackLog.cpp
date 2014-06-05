#include "stdafx.h"
#include "FeedbackLog.h"


FeedbackLog::FeedbackLog(ManagedLib::ManagedLog^ p)
{
	_Managed = p;
}

void FeedbackLog::OnFlush(int pValue)
{
	_Managed->raiseValueChanged(pValue);
}