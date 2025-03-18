#ifndef STR_PARSE_H
#define STR_PARSE_H

#include <string>
#include <list>
#include <algorithm>
#include <vector>
#include <locale>
#include <stdarg.h>
#include <regex>

#include "tstring.h"
#include "../platform_fork.h"

//Just a helper macro
#define ADD_WS(x)   patt.emplace_back(LL(x))
#define ADD_S(x)    patt.emplace_back(x)

namespace str
{

template<typename T>
inline size_t findLastNotOf(const std::basic_string<T> & sIn, const T charToTrim)
{
    size_t nPos = sIn.size();
    while (nPos-- > 0)
    {
        if (sIn[nPos] != charToTrim)
            break;
    }
    return nPos;
}


inline std::string to_lower(const std::string & sIn)
{
    std::string result = sIn;
    std::transform(sIn.begin(), sIn.end(), result.begin(), tolower);
    return result;
}


#if defined(WIN32) || defined(WIN64)
inline tstring to_lower(const tstring & sIn)
{
    tstring result = sIn;
    std::transform(sIn.begin(), sIn.end(), result.begin(), tolower);
    return result;
}
#endif


inline std::string to_upper(const std::string & sIn)
{
    std::string result = sIn;
    std::transform(sIn.begin(), sIn.end(), result.begin(), toupper);
    return result;
}


inline bool stringToBool(const std::string & sIn)
{
    std::string sLower = to_lower(sIn);
    return (sLower == "1" || sLower == "true" || sLower == "yes" || sLower == "ok");
}


inline bool tstringToBool(const tstring & sIn)
{
    tstring sLower = to_lower(sIn);
    return (sLower == LL("1") || sLower == LL("true") || sLower == LL("yes") || sLower == LL("ok"));
}


inline std::string & trim_left(std::string & sIn)
{
    if (sIn.empty())
        return sIn;
    size_t nPos = sIn.find_first_not_of(' ');
    sIn.erase(0, nPos);
    return sIn;
}


inline std::wstring & trim_left(std::wstring & sIn)
{
    if (sIn.empty())
        return sIn;
    size_t nPos = sIn.find_first_not_of(L' ');
    sIn.erase(0, nPos);
    return sIn;
}


template<typename T>
inline void trim_left(std::basic_string<T>& sIn, const T& charsToTrim)
{
    if (sIn.empty())
        return;
    size_t nPos = sIn.find_first_not_of(charsToTrim);
    sIn.erase(0, nPos);
}

    
template<typename T>
inline std::basic_string<T> trim_left_copy(const std::basic_string<T>& sIn, const T& charsToTrim)
{
    std::basic_string<T> sResult = sIn;
    trim_left(sResult, charsToTrim);
    return sResult;
}


inline std::wstring trim_left_copy(const std::wstring & sIn)
{
    if (sIn.empty())
        return sIn;
    return sIn.substr(sIn.find_first_not_of(L' '));
}


inline std::string trim_left_copy(const std::string & sIn)
{
    if (sIn.empty())
        return sIn;
    return sIn.substr(sIn.find_first_not_of(' '));
}


inline std::string & trim_right(std::string & sIn)
{
    if (sIn.empty())
        return sIn;
    size_t nPos = sIn.find_last_not_of(' ');
    sIn.erase(nPos + 1);
    return sIn;
}


inline std::wstring &  trim_right(std::wstring & sIn)
{
    if (sIn.empty())
        return sIn;
    size_t nPos = sIn.find_last_not_of(L' ');
    sIn.erase(nPos + 1);
    return sIn;
}


template<typename T>
inline void trim_right(std::basic_string<T> & sIn, const T& charToTrim)
{
    if (sIn.empty())
        return;
    size_t nPos = sIn.find_last_not_of(charToTrim);
    sIn.erase(nPos + 1);
}


template<typename T>
inline void trim_right(std::basic_string<T> & sIn, const T *charsToTrim)
{
    if (sIn.empty() || charsToTrim == nullptr)
        return;
    size_t nPos = sIn.find_last_not_of(charsToTrim);
    sIn.erase(nPos + 1);
}


template<typename T>
inline void trimRight(std::basic_string<T> & sIn, const T charToTrim)
{
    if (sIn.empty())
        return;
    size_t nPos = findLastNotOf(sIn, charToTrim);
    sIn.erase(nPos + 1);
}


inline std::string trim_right_copy(const std::string & sIn)
{
    if (sIn.empty())
        return sIn;
    return sIn.substr(0, sIn.find_last_not_of(' ') + 1);
}


inline std::wstring trim_right_copy(const std::wstring & sIn)
{
    if (sIn.empty())
        return sIn;
    return sIn.substr(0, sIn.find_last_not_of(L' ') + 1);
}


#if defined(WIN32) || defined(WIN64)
inline int compare_ci(const std::wstring strVal1, const wchar_t *pszVal2)
{
    return ::_wcsicmp(strVal1.c_str(), pszVal2);
}
#endif


inline int compare_ci(const std::string strVal1, const char *pszVal2)
{
    return STRCASECMP(strVal1.c_str(), pszVal2);
}


template<typename charT>
struct caseInsensitiveComparer
{
    caseInsensitiveComparer(const std::locale& loc) : loc_(loc) {}
    bool operator()(charT ch1, charT ch2)
    {
        return std::tolower(ch1, loc_) == std::tolower(ch2, loc_);
    }
private:
    const std::locale& loc_;
};


template<typename T>
inline bool endsWith(const std::basic_string<T> & sIn, const std::basic_string<T> & sWhat)
{
    size_t sizePos = sIn.find(sWhat);
    if (sizePos == -1)
        return false;
    else
        return sizePos == sIn.size() - sWhat.size();
}


template<typename T>
size_t find_ci(const std::basic_string<T> & sIn, const std::basic_string<T> & sWhat, const std::locale & loc = std::locale())
{
    typename std::basic_string<T>::const_iterator it = std::search(sIn.begin(), sIn.end(),
        sWhat.begin(), sWhat.end(), caseInsensitiveComparer<T>(loc));
    if (it != sIn.end())
        return it - sIn.begin();
    else
        return -1;
}


inline size_t find_ci(std::string::const_iterator itInStart, std::string::const_iterator itInEnd, const std::string & sWhat, const std::locale &loc = std::locale())
{
    std::string::const_iterator it = std::search(itInStart, itInEnd, 
        sWhat.begin(), sWhat.end(), caseInsensitiveComparer<typename std::string::value_type>(loc));
    if (it != itInEnd)
        return it - itInStart;
    else
        return -1;
}


inline std::wstring replace(const std::wstring & sIn, const std::wstring & sWhat, const std::wstring & sWith)
{
    std::wstring result = sIn;
    size_t nPos = 0;
    size_t cchWhat = sWhat.size();
    size_t cchWith = sWith.size();

    while (true)
    {
        nPos = result.find(sWhat, nPos);// + cchWhat - cchWith + 1);
        if (nPos == -1)
            break;
        result.erase(nPos, sWhat.size());
        result.insert(nPos, sWith);
        nPos += cchWith - cchWhat + 1;
    }

    return std::move(result);
}


inline std::string replace(const std::string & sIn, const std::string & sWhat, const std::string & sWith)
{
    std::string result = sIn;
    size_t nPos = 0;
    size_t cchWhat = sWhat.size();
    size_t cchWith = sWith.size();

    while (true)
    {
        nPos = result.find(sWhat, nPos);// + cchWhat - cchWith + 1);
        if (nPos == -1)
            break;
        result.erase(nPos, sWhat.size());
        result.insert(nPos, sWith);
        nPos += cchWith - cchWhat + 1;
    }

    return std::move(result);
}


inline std::wstring & replace(std::wstring & sIn, const std::wstring & sWhat, const std::wstring & sWith)
{   
    size_t nPos = 0;
    size_t cchWhat = sWhat.size();
    size_t cchWith = sWith.size();

    while (true)
    {
        nPos = sIn.find(sWhat, nPos);// + cchWhat - cchWith + 1);
        if (nPos == -1)
            break;
        sIn.erase(nPos, sWhat.size());
        sIn.insert(nPos, sWith);
        nPos += cchWith - cchWhat + 1;
    }

    return sIn;
}


inline std::string & replace(std::string & sIn, const std::string & sWhat, const std::string & sWith)
{   
    size_t nPos = 0;
    size_t cchWhat = sWhat.size();
    size_t cchWith = sWith.size();

    while (true)
    {
        nPos = sIn.find(sWhat, nPos);// + cchWhat - cchWith + 1);
        if (nPos == -1)
            break;
        sIn.erase(nPos, sWhat.size());
        sIn.insert(nPos, sWith);
        nPos += cchWith - cchWhat + 1;
    }

    return sIn;
}


//template <typename T> //char_traits<tchar>, allocator<tchar>>
//std::basic_stringstream<T> toString(const T &val)
//{
//    std::basic_stringstream<T> out;
//    out << val;
//    return out.str();
//}


inline std::string formatStr(const std::string & sFormat, const std::string & sArg)
{
    std::vector<char> vBuf;
    vBuf.resize(sFormat.size() /*%s*/- 2 + sArg.size() + /*'\0'*/1);
#if defined(WIN32) || defined(WIN64) //UTF16 system
#pragma warning( push )
#pragma warning ( disable : 4996 )
    sprintf((char *)&vBuf[0], sFormat.c_str(), sArg.c_str());
#pragma warning( pop )
#else
    sprintf((char *)&vBuf[0], sFormat.c_str(), sArg.c_str());
#endif
    return &vBuf[0];
}


inline std::string formatStr(size_t nBuffSize, char *fmt, ...)
{
    if (fmt == nullptr)
        return "";

    std::vector<char> buf(nBuffSize);
    buf[nBuffSize - 1] = 0;
    va_list vl;
    va_start(vl, fmt);
#if defined(WIN32) || defined(WIN64)
    vsnprintf_s(buf.data(), nBuffSize, nBuffSize, fmt, vl);
#else
    vsnprintf(buf.data(), nBuffSize, fmt, vl);
#endif
    return std::string(buf.data(), buf.size());
}


// Use {0} to specify the insertion point
inline tstring formatNumber(const tstring & sFormat, const size_t number)
{
    const size_t nMaxDigitsInNumber = 20; //15 for double, 20 for unsigned long long
    std::vector<tchar> vBuf;
    vBuf.resize(sFormat.size() /*{0}*/- 3 + nMaxDigitsInNumber + /*'\0'*/1);
#if defined(WIN32) || defined(WIN64) //UTF16 system
#pragma warning( push )
#pragma warning ( disable : 4996 )
    tstring sFormatStr;
    if (sizeof(number) > 4)
        sFormatStr = str::replace(sFormat, LL("{0}"), LL("%L"));
    else
        sFormatStr = str::replace(sFormat, LL("{0}"), LL("%d"));
    swprintf((tchar *)&vBuf[0], sFormatStr.c_str(), number);
#pragma warning( pop )
#else
    sprintf((tchar *)&vBuf[0], sFormat.c_str(), number);
#endif
    return &vBuf[0];
}


// Use {0} to specify the insertion point
template <typename T>
inline tstring formatNumber(const tstring & sFormat, const T &number)
{
    const size_t nMaxDigitsInNumber = 20; //15 for double, 20 for unsigned long long
    std::vector<tchar> vBuf;
    vBuf.resize(sFormat.size() /*{0}*/- 3 + nMaxDigitsInNumber + /*'\0'*/1);
#if defined(WIN32) || defined(WIN64) //UTF16 system
#pragma warning( push )
#pragma warning ( disable : 4996 )
    tstring sFormatStr;
    if (sizeof(number) > 4)
        sFormatStr = str::replace(sFormat, LL("{0}"), LL("%L"));
    else
        sFormatStr = str::replace(sFormat, LL("{0}"), LL("%d"));
    swprintf((tchar *)&vBuf[0], sFormatStr.c_str(), number);
#pragma warning( pop )
#else
    sprintf((tchar *)&vBuf[0], sFormat.c_str(), number);
#endif
    return &vBuf[0];
}


inline std::wstring removeChars(std::wstring & sIn, const std::wstring & sChars)
{
    for (std::wstring::const_iterator it = sIn.begin(); it != sIn.end();)
    {
        if (sChars.find(*it) != -1)
            sIn.erase(it, it + 1);
        else
            it++;
    }

    return sIn;
}


inline std::string removeChars(std::string & sIn, const std::string & sChars)
{
    for (std::string::const_iterator it = sIn.begin(); it != sIn.end();)
    {
        if (sChars.find(*it) != -1)
            sIn.erase(it, it + 1);
        else
            it++;
    }

    return sIn;
}


inline tstring removeChars_copy(const tstring & sIn, const tstring & sChars)
{
    tstring result;

    for (tstring::const_iterator it = sIn.begin(); it != sIn.end(); it++)
    {
        if (sChars.find(*it) == -1)
            result += *it;
    }

    return result;
}


template <typename T>
inline void tokenize(const std::basic_string<T>& sIn, const std::basic_string<T>& sSeparator, std::vector<std::basic_string<T>>& vec_parts)
{
    if (sSeparator.empty())
        return;

    size_t cchStart = 0;
    size_t cchEnd = sIn.find(sSeparator, cchStart);

    vec_parts.clear();

    if (sIn.empty())
        return;

    if (cchEnd == -1)
    {
        vec_parts.push_back(sIn);
    }
    else
    {
        size_t cchSrcLen        = sIn.size();
        size_t cchSeparatorLen  = sSeparator.size();

        do
        {
            vec_parts.push_back(sIn.substr(cchStart, cchEnd - cchStart));

            cchStart = cchEnd + cchSeparatorLen;
            cchEnd = sIn.find(sSeparator, cchStart);

            if (cchEnd == -1)
                vec_parts.push_back(sIn.substr(cchStart, cchSrcLen - cchStart));
        }
        while (cchEnd != -1);
    }
}


inline const char *rstrstr(const char *sSrc, size_t sizeSrcLen, const char *s2)
{
    size_t  s1len = sizeSrcLen;
    size_t  s2len = strlen(s2);
    const char *s;

    if (s2len > s1len)
        return NULL;
    for (s = sSrc + s1len - s2len; s >= sSrc; --s)
        if (strncmp(s, s2, s2len) == 0)
            return s;
    return nullptr;
}


template<typename T>
inline std::basic_string<T> makeSafeFileName(const std::basic_string<T> &sVal)
{
    std::basic_string<T> sRet = sVal;
    str::replace(sRet, "<", "-");
    str::replace(sRet, ">", "-");
    str::replace(sRet, ":", "-");
    str::replace(sRet, "\"", "");
    str::replace(sRet, "/", "-");
    str::replace(sRet, "\\", "-");
    str::replace(sRet, "|", "-");
    str::replace(sRet, "?", "-");
    str::replace(sRet, "*", "-");    
    
    str::replace(sRet, "...", ".");
    str::replace(sRet, "..", ".");
    
    
    str::trim_left(sRet);
    return sRet;
}


template<>
inline std::basic_string<wchar_t> makeSafeFileName(const std::basic_string<wchar_t> &sVal)
{
    std::basic_string<wchar_t> sRet = sVal;
    str::replace(sRet, L"<", L"-");
    str::replace(sRet, L">", L"-");
    str::replace(sRet, L":", L"-");
    str::replace(sRet, L"\"", L"");
    str::replace(sRet, L"/", L"-");
    str::replace(sRet, L"\\", L"-");
    str::replace(sRet, L"|", L"-");
    str::replace(sRet, L"?", L"-");
    str::replace(sRet, L"*", L"-");

    str::replace(sRet, L"...", L".");
    str::replace(sRet, L"..", L".");


    str::trim_left(sRet);
    return sRet;
}


int inline compareVersions(const std::string & sVer1, const std::string & sVer2, bool& bOk)
{
    bOk = true;
    const std::regex pattern("^(\\d+)\\.(\\d+)\\.(\\d+)\\.(\\d+)$");
    std::smatch m1;
    std::smatch m2;
    std::regex_search(sVer1, m1, pattern);
    std::regex_search(sVer2, m2, pattern);

    if ((m1.size() == 0) || (m2.size() == 0) || (m1.size() != 5) || (m2.size() != 5))
    {
        bOk = false;
        return 0;
    }

    int nComparisonResult = 0;
    for (unsigned int i = 1; i < m1.size() - 1; ++i)
    {
        int numb1(0);
        std::istringstream(m1[i]) >> numb1;

        int numb2(0);
        std::istringstream(m2[i]) >> numb2;

        int nDiff = numb2 - numb1;
        if (nDiff == 0)
            continue;

        if (nDiff > 0)
        {
            nComparisonResult = 1;
            break;
        }
        else
        {
            nComparisonResult = -1;
            break;
        }
    }
    return nComparisonResult;
}

inline int parseDurationString(const std::string& sIn) {
    std::string sHours;
    std::string sMin;
    std::string sSec;
    std::string sDuration = str::trim_left_copy(sIn);
    str::trim_right(sDuration);

    int duration = 0;
    if (!sDuration.empty()) {
        size_t posLeft = sDuration.find(":");
        size_t posRight = sDuration.find_last_of(":");

        if (posLeft != -1 && posRight != -1) {
            if (posLeft == posRight) {
                sMin = sDuration.substr(0, posLeft);
                sSec = sDuration.substr(sMin.size() + 1);
            }
            else {
                sHours = sDuration.substr(0, posLeft);
                sMin = sDuration.substr(posLeft + 1, 2);
                sSec = sDuration.substr(posRight + 1);
            }
        }
    }

    if (sHours.size())
        duration += atoi(sHours.c_str()) * 3600;

    duration += atoi(sMin.c_str()) * 60;
    duration += atoi(sSec.c_str());

    return duration;
}

}

#endif //STR_PARSE_H
