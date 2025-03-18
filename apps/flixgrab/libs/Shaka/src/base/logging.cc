

#include "logging.h"

#ifdef _DEBUG
LogSeverity             LoggerSettings::min_severity_ = LOG_VERBOSE;
#else
LogSeverity             LoggerSettings::min_severity_ = LOG_INFO;
#endif

