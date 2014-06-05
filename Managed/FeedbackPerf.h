#pragma once

#include <vcclr.h>

#include "../DirectX_Engine/UnmanagedPerf.h"
#include "ManagedPerf.h"

using namespace ManagedLib;

class FeedbackPerf : public UnmanagedPerf::Feedback
{
public:
	FeedbackPerf(ManagedPerf^ p);

	void OnBasicUpdate(int primitives, float gpuUsage);

private:
	gcroot<ManagedPerf^> _Managed;
};