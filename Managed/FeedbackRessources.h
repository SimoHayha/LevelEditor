#pragma once
#include <vcclr.h>
#include "..\DirectX_Engine\UnmanagedRessources.h"
#include "ManagedRessources.h"

using namespace ManagedLib;

class FeedbackRessources : public UnmanagedRessources::Feedback
{
public:
	FeedbackRessources(ManagedRessources^ p);
	~FeedbackRessources(void);
	void OnListChanged(int pValue);

private:
	gcroot<ManagedRessources^> _Managed;
};
