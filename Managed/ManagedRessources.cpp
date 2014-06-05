#include "stdafx.h"
#include "ManagedRessources.h"
#include "Managed.h"
#include "FeedbackRessources.h"



namespace ManagedLib
{
	ManagedRessources::ManagedRessources()
	{
		_feedback = new FeedbackRessources(this);
		_unmanaged = new UnmanagedRessources(_feedback);
		_unmanaged->init(); //verif
	}

	ManagedRessources::!ManagedRessources()
	{
		delete _unmanaged;
		delete _feedback;
	}

	ManagedRessources::~ManagedRessources()
	{
	}

	bool
	ManagedRessources::reloadScript(String^ path)
	{
		std::string fileName = "";

		if (path == nullptr)
			return false;
		Managed::marshalString(path, fileName);
		_unmanaged->reloadScript(fileName);
		return true;
	}

	bool
	ManagedRessources::addTexture(String^ path)
	{
		std::string fileName = "";

		if (path == nullptr)
			return false;
		Managed::marshalString(path, fileName);
		_unmanaged->addTexture(fileName);
		return true;
	}

	bool
	ManagedRessources::loadScript(String^ path)
	{
		std::string fileName = "";

		if (path == nullptr)
			return false;
		Managed::marshalString(path, fileName);
		_unmanaged->loadScript(fileName);
		return true;
	}

	/*void
	ManagedRessources::raiseValueChanged(int i)
	{
		if (_valueChanged != nullptr)
			_valueChanged->Invoke(i);
	}*/
}
