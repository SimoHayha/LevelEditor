#pragma once

#include <vcclr.h>
#include "..\DirectX_Engine\main.h"
#include "Managed.h"

using namespace ManagedLib;

class Feedback : public CUnmanaged::Feedback 
{
public:
	Feedback(Managed^ p);
	void OnClick(void);
private:
	gcroot<Managed^> _Managed;
};
