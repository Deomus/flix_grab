//platform_defs.h - win32

#ifndef PLATFORM_DEFS_H
#define PLATFORM_DEFS_H

#include <shlwapi.h>
#include <shlobj.h>
#include <algorithm>

#include "../str/tstring.h"

#ifndef  _WIN64
#define OS_IS_32_BIT
#endif

#define fseeko64        _fseeki64
#define ftello64        _ftelli64

namespace sys
{

enum WinVer
{
    WINVER_UNKNOWN          = 0,
    WINVER_2000             = 50,
    WINVER_XP               = 51,
    WINVER_SERVER_2003      = 52,
    //WINVER_XP_64            = 52,
    WINVER_VISTA            = 60,
    //WINVER_SERVER_2008      = 60,
    //WINVER_SERVER_2008_R2   = 61,
    WINVER_WIN7             = 61,
    WINVER_WIN8             = 62,
    //WINVER_SERVER_2012      = 62,
};

}

#endif //PLATFORM_DEFS_H
