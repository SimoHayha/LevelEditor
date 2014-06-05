#include "stdafx.h"
#include "FeedbackRessources.h"


using namespace ManagedLib;
FeedbackRessources::FeedbackRessources(ManagedRessources^ p)
{
	_Managed = p;
}


FeedbackRessources::~FeedbackRessources(void)
{
}

void
FeedbackRessources::OnListChanged(int value)
{
	if (value == 0)
		_Managed->OnPropertyChanged("FbxModels");
	else if (value == 1)
		_Managed->OnPropertyChanged("Scripts");
	else if (value == 2)
		_Managed->OnPropertyChanged("Textures");
}