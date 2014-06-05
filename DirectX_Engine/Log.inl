template <typename T>
log_implementation::Log&
log_implementation::Log::operator<<(T const& right)
{
	if (current <= level)
		os << right;
	return *this;
}

template <log_level_t level>
log_implementation::Log*
logger(const std::string& file, int line, const std::string& func)
{
	log_implementation::Log*	logger = log_implementation::Log::get();
	std::string					temp;

	temp = file;
	std::reverse(temp.begin(), temp.end());
	size_t pos = temp.find_first_of("\\/");
	if (pos != std::string::npos)
		temp.erase(temp.begin() + pos, temp.end());
	std::reverse(temp.begin(), temp.end());
	logger->setCurrentLogLevel(level);
	*logger << "[" << temp << ":" << line << ":" << func <<"]";
	if (level == LOG_CRITICAL)
		*logger << "_CRITICAL_\t";
	if (level == LOG_ERROR)
		*logger << "_ERROR_   \t";
	if (level == LOG_WARNING)
		*logger << "_WARNING_ \t";
	if (level == LOG_INFO)
		*logger << "_INFO_    \t";
	if (level == LOG_DEBUG)
		*logger << "_DEBUG_   \t";
	return logger;
}
