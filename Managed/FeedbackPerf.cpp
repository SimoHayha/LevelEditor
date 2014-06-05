#include "stdafx.h"
#include "FeedbackPerf.h"

using namespace ManagedLib;
FeedbackPerf::FeedbackPerf(ManagedPerf^ p)
{
	_Managed = p;
}

void FeedbackPerf::OnBasicUpdate(int primitives, float gpuIdle)
{
	_Managed->raiseSimpleUpdate(primitives, gpuIdle);
}