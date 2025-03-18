//platform_helpers.h

#pragma once

#if defined(WIN32) || defined(WIN64)
	#include "util/sys/win32/platform_helpers.h"
#else
	#include "util/sys/mac/platform_helpers.h"
#endif
