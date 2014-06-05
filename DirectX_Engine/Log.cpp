#include "Log.hpp"
#include "UnmanagedLog.h"

log_implementation::Log* log_implementation::Log::__this = NULL;

log_implementation::Log::Log(log_level_t lvl) : level(lvl), current(LOG_NOTHING), os(&buffer)
{
}

log_implementation::Log::~Log()
{
}

log_implementation::Log&
log_implementation::Log::operator<<(Elog::log_flush_t right)
{
	if (current <= level)
		_unmanaged->_feedback->OnFlush(0);
	else
		buffer.str("");
	return *this;
}

log_implementation::Log*
log_implementation::Log::get(log_level_t lvl)
{
	if (__this == NULL)
	{
		__this = new (std::nothrow) Log(lvl);
		if (__this == NULL)
			return NULL;
	}
	return __this;
}

void
log_implementation::Log::setCurrentLogLevel(log_level_t lvl)
{
	current = lvl;
}

void
log_implementation::Log::setLevel(log_level_t lvl)
{
	level = lvl;
}

void
log_implementation::Log::setUnmanaged(UnmanagedLog* unmanaged)
{
	_unmanaged = unmanaged;
}

std::stringbuf&
log_implementation::Log::getBuffer(void)
{
	return buffer;
}

void
	log_implementation::Log::clearBuffer(void)
{
	buffer.str("");
}