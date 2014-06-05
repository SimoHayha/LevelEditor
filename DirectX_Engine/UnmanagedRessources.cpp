#include "UnmanagedRessources.h"
#include "Dictionnary.h"


UnmanagedRessources::UnmanagedRessources(Feedback* feedback)
{
	_feedback = feedback;
}


UnmanagedRessources::~UnmanagedRessources(void)
{
}

bool
UnmanagedRessources::init(void)
{
	_dico = Dictionnary::getInstance();
	if (_dico == nullptr)
		return false;
	_dico->_feedback = _feedback;
	return true;
}

const std::string*
UnmanagedRessources::getNameModels(bool& recall)
{
	return _dico->getNameFbxModels(recall);
}

const std::string*
UnmanagedRessources::getNameScripts(bool& recall)
{
	return _dico->getNameScripts(recall);
}

const std::string*
UnmanagedRessources::getNameTextures(bool& recall)
{
	return _dico->getNameTextures(recall);
}

bool
UnmanagedRessources::reloadScript(const std::string& file)
{
	return _dico->reloadScript(file);
}

bool
UnmanagedRessources::addTexture(const std::string& file)
{
	if (_dico->getTexture(file) != nullptr)
		return true;
	return false;
}

bool
UnmanagedRessources::loadScript(const std::string& file)
{
	if (_dico->getScript(file) != nullptr)
		return true;
	return false;
}