// tstring.h

#ifndef COMMON__TSTRING_H
#define COMMON__TSTRING_H

#ifndef UNUZD
#define UNUZD(a) (void)a
#endif

#include <string>
#include <sstream>

#include "../sys/DebugMemoryCheck.h"

#ifndef NPP
#define NPP std::string::npos
#endif

#if defined(WIN32) || defined(WIN64)
    #define TCHAR_IS_W
    #define tchar wchar_t
    #define LL(x) L ## x
    #define tostringstream std::wostringstream
#else
    #define tchar char
    #define LL(x) x
    #define tostringstream std::ostringstream
#endif

//
typedef std::basic_string<tchar>    tstring;

// Size helper
#if defined(WIN32) || defined(WIN64)
#else
    #define ARRAYSIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif

// CRT functions
#if defined(WIN32) || defined(WIN64)
    #define tslen(x) wcslen(x)
    #define tstrcpy wcscpy
#else
#   define tslen(x) strlen(x)
    #define tstrcpy strcpy
#endif

//
namespace str
{

template <typename T>
inline std::basic_string<T> makeTstring(const std::string& sIn)
{
    std::basic_string<T> result;

    if (sIn.empty())
        return result;

    if (sizeof(T) == sizeof(char))
    {
        std::basic_string<T>& sInputCasted = *(std::basic_string<T> *)(void *)&sIn;
        result.assign(sInputCasted.cbegin(), sInputCasted.cend());
    }
    else
    {
        result.resize(sIn.size() + 1);
        for (size_t i = 0; i < sIn.size(); i++)
            result[i] = (T)sIn[i];
        result[sIn.size()] = 0;
    }

    return result;
}

}

#endif //COMMON__TSTRING_H
