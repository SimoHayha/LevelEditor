#include "UnmanagedLog.h"
#include "Log.hpp"

UnmanagedLog::UnmanagedLog(Feedback* feedback)
{
	_feedback = feedback;
	_log = log_implementation::Log::get();
	_log->setUnmanaged(this);
	_log->setLevel(log_level_t::LOG_DEBUG);
}

void
UnmanagedLog::getValue(char* buffer)
{
	log_implementation::Log*	log = log_implementation::Log::get();
	std::stringbuf&				buf = log->getBuffer();
	std::string					str = buf.str();
	size_t						size = str.size();

	strcpy_s(buffer, size + 1, str.c_str());
	buffer[size] = '\0';
	log->clearBuffer();
}

size_t
UnmanagedLog::getLen(void)
{
	return log_implementation::Log::get()->getBuffer().str().size();
}