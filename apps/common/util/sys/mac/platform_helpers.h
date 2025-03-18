//platform_helpers.h - mac

#ifndef PLATFORM_HELPERS_H
#define PLATFORM_HELPERS_H

#include <string>

#include "platform_defs.h"
#include "../../str/tstring.h"

#define HKEY_CURRENT_USER NULL
#define HKEY void*
#define DWORD int

namespace sys
{

inline std::wstring utf8ToWide(const std::string& str)
{
    int size_needed = 0;//MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo( size_needed, 0 );
    //MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

inline bool is64BitOS()
{
    return true;
}

inline DWORD readRegDword(HKEY hHive, const std::string& regPath, const std::string& valueName, DWORD defaultValue = -1, bool bSuppressWow64Redirect = false)
{
    return 0;
}

inline std::string readRegString(const std::string& regPath, const std::string& valueName)
{
    return "";
}

inline int writeRegString(const std::string& regPath, const std::string& valueName, const std::string& valueData)
{
    return 0;
}



}

#endif //PLATFORM_HELPERS_H
