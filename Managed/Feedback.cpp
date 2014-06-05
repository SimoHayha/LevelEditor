#include "stdafx.h"
#include "Feedback.h"

using namespace ManagedLib;
Feedback::Feedback(Managed^ p)
{
	_Managed = p;
}

void Feedback::OnClick(void)
{
	_Managed->raiseClick();
}