#pragma once

#include "licensing.h"
#include <stdlib.h>
#include <stddef.h>
#include <string.h>                     /* memset(), memcpy() */
#include <assert.h>
#include <string>

#ifdef _WIN32

#define _WIN32_WINNT 0x0501
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#define _CRTDBG_MAP_ALLOC 

#include <crtdbg.h>
#include <WinSock2.h>
#include <windows.h>
#include <tchar.h>

#endif

#ifdef _UNICODE
	#define t_stricmp _wcsicmp
	#define t_isalnum iswalnum
	#define t_strlen wcslen
	#define t_atoi _wtoi
	#define t_strncpy wcsncpy
	#define T_STR(s) L##s
	#define t_snprintf _snwprintf

#ifndef _WIN32
	#define _T(s) L##s
	#define _tcslen wcslen
#endif

	typedef std::wstring string_t;
#else
	#define t_stricmp _stricmp
	#define t_isalnum isalnum
	#define t_strlen strlen
	#define t_atoi atoi
	#define t_strncpy strncpy
	#define T_STR(s) s
	#define t_snprintf _snprintf

#ifndef _WIN32
	#define _T(s) s
	#define _tcslen strlen
#endif

	typedef std::string string_t;
#endif

#ifndef _WIN32

#define _alloca alloca
#define _stricmp strcasecmp
#define _snprintf snprintf
#define _atoi64(s) strtoull(s, NULL, 10)

#endif
