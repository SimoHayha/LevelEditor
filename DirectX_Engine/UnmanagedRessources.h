#pragma once

#ifdef UNMANAGED_EXPORTS
#define UNMANAGED_API __declspec(dllexport)
#else
#define UNMANAGED_API __declspec(dllimport)
#endif

#include <string>

class Dictionnary;

class UNMANAGED_API UnmanagedRessources
{
public:
	class UNMANAGED_API Feedback
	{
	public:
		virtual void OnListChanged(int value) = 0;
	};

	UnmanagedRessources(Feedback* feedback);
	~UnmanagedRessources(void);

	bool				init(void);
	const std::string*	getNameScripts(bool& recall);
	const std::string*	getNameTextures(bool& recall);
	const std::string*	getNameModels(bool& recall);

	bool	reloadScript(const std::string& file);
	bool	addTexture(const std::string& file);
	bool	loadScript(const std::string& file);
	Feedback*	_feedback;
private:
	Dictionnary*	_dico;
};