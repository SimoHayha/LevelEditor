#pragma once

#ifdef UNMANAGED_EXPORTS
#define UNMANAGED_API __declspec(dllexport)
#else
#define UNMANAGED_API __declspec(dllimport)
#endif

#include <string>
#include "Log.hpp"

class UNMANAGED_API UnmanagedLog
{
public:
	log_implementation::Log*	_log;

	class UNMANAGED_API Feedback
	{
	public:
		virtual void OnFlush(int value) = 0;
	};

	UnmanagedLog(Feedback* feedback);

	void	getValue(char* buf);
	size_t	getLen(void);
	Feedback*	_feedback;
private:
	
};