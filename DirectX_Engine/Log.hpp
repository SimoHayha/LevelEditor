#pragma once


#include <iostream>
#include <sstream>

#define logCall(loglevel)	*(logger<loglevel>(__FILE__, __LINE__, __FUNCTION__))

class UnmanagedLog;

enum	log_level_t {
	LOG_NOTHING,
	LOG_CRITICAL,
	LOG_ERROR,
	LOG_WARNING,
	LOG_INFO,
	LOG_DEBUG
	};

namespace Elog
{
	enum	log_flush_t {
		endl,
		flush
		};
}

namespace log_implementation
{
	class	Log
	{
		static	Log*		__this;

				log_level_t	level;
				log_level_t	current;
				std::stringbuf	buffer;
				std::ostream	os;
				UnmanagedLog*	_unmanaged;
				
				Log(log_level_t lvl);
	public:
						~Log();

				Log&			operator<<(Elog::log_flush_t right);
				template <typename T>
				Log&			operator<<(const T& right);
				void			setCurrentLogLevel(log_level_t lvl);
				void			setLevel(log_level_t lvl);
				void			setUnmanaged(UnmanagedLog* unmanaged);
				std::stringbuf&	getBuffer(void);
				void			clearBuffer(void);
		static	Log*	get(log_level_t lvl = LOG_NOTHING);
	};
}

#include "Log.inl"
