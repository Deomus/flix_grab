#pragma once

#if defined(WIN32) || defined(WIN64)
#include <windows.h>
#endif

#include <string>
#include <stdlib.h>
#include <vector>
#include <iterator>
#include <algorithm>
#include <locale>
#include <codecvt>
#include <stdio.h>
#include <cstddef>

#include "tstring.h"
#include "../platform_fork.h"

#include "../sys/DebugMemoryCheck.h"

//
typedef std::vector<std::string> StrVector;

#if defined(WIN32) || defined(WIN64)
#   define TO_TSTRING(x)   std::to_wstring(x)
#else
#   define TO_TSTRING(x)   std::to_string(x)
#endif

namespace str
{
#if defined(WIN32) || defined(WIN64)
inline std::string _Narrow_str(std::wstring _Str)
{    // convert wchar_t string to char string
    std::string _Ans;

    for (const wchar_t *_Ptr = _Str.c_str(); *_Ptr != 0; ++_Ptr)
        _Ans.push_back((char)_CSTD wctob(*_Ptr));
    return (_Ans);
}
#endif


inline bool isNumber(char c)
{
    return c <= 57 && c >= 48;
}


inline bool isHex(const std::string& sIn)
{
    bool result = true;
    for (size_t i = 0; i < sIn.size(); i++)
    {
        if (iswxdigit((wint_t)sIn[i]) == 0)
        {
            result = false;
            break;
        }
    }
    return result;
}


inline int tstringToInt(const tstring& sIn)
{
    return ttoi(sIn.c_str());
}


inline int stringToInt(const std::string& sIn)
{
    return atoi(sIn.c_str());
}


inline long long stringToLLong(const std::string& sIn)
{
#if defined(WIN32) || defined(WIN64)
    return _strtoi64(sIn.c_str(), NULL, 10);
#else
    return strtoll(sIn.c_str(), NULL, 10);
#endif
}


inline double stringToDouble(const std::string& sIn)
{
    return atof(sIn.c_str());;
}


inline unsigned long tstringToULong(const tstring& sIn)
{
#ifdef TCHAR_IS_W //UTF16 system
    return wcstoul(sIn.c_str(), NULL, 10);
#else
    return std::strtoul(sIn.c_str(), NULL, 10);
#endif
}


inline long long tstringToLLong(const tstring& sIn)
{
#ifdef TCHAR_IS_W //UTF16 system
    return _wcstoi64(sIn.c_str(), NULL, 10);
#else
    return strtoll(sIn.c_str(), NULL, 10);
#endif
}


inline unsigned long long tstringToULLong(const tstring& sIn)
{
#ifdef TCHAR_IS_W //UTF16 system
    return _wcstoui64(sIn.c_str(), NULL, 10);
#else
    return std::strtoull(sIn.c_str(), NULL, 10);
#endif
}


#if defined(WIN32) || defined(WIN64)
inline std::wstring longLongToTString(long long cchValue)
{
    const int nBufSize = 64;
    wchar_t szBuf[nBufSize];
    swprintf_s(szBuf, nBufSize, L"%lld", cchValue);
    return szBuf;
}
#endif


inline std::string longLongToString(long long cchValue)
{
    const int nBufSize = 64;
    char szBuf[nBufSize];
#if defined(WIN32) || defined(WIN64)
    sprintf_s(szBuf, nBufSize, "%lld", cchValue);
#else
    sprintf(szBuf, "%lld", cchValue);
#endif
    return szBuf;
}


inline std::string doubleToString(double cchValue)
{
    const int nBufSize = 32;
    char szBuf[nBufSize];
#if defined(WIN32) || defined(WIN64)
    sprintf_s(szBuf, nBufSize, "%g", cchValue);
#else
    sprintf(szBuf, "%g", cchValue);
#endif
    return szBuf;
}


inline std::string intToString(int nValue)
{
    const int nBufSize = 64;
    char szBuf[nBufSize];
#if defined(WIN32) || defined(WIN64)
    sprintf_s(szBuf, nBufSize, "%d", nValue);
#else
    sprintf(szBuf, "%d", nValue);
#endif
    return szBuf;
}


#if defined(WIN32) || defined(WIN64)
    #define sizetToString intToString
#else
    #define sizetToString longLongToString
#endif


inline std::wstring intToWString(int nIn)
{
    const size_t nBufSize = 64;
    wchar_t buf[nBufSize];
#if defined(WIN32) || defined(WIN64)
    _itow_s(nIn, buf, nBufSize, 10);
#endif
    return buf;
}


inline tstring intToTString(int nValue)
{
#if defined(WIN32) || defined(WIN64)
    return intToWString(nValue);
#else
    return intToString(nValue);
#endif
}


// The function doesn't convert from utf8 to utf16, use utfToWString() for that purpose.
inline tstring ansiToWString(const std::string& sIn)
{
#ifndef TCHAR_IS_W
    return sIn;
#endif
    tstring result(sIn.length(), L' ');
    std::copy(sIn.begin(), sIn.end(), result.begin());
    return result;
}


inline void ansiToWString(const std::string& sIn, std::vector<tchar> &vOut)
{
    vOut.reserve(sIn.size());
    std::copy(sIn.begin(), sIn.end(), std::back_inserter(vOut));
}


inline std::string wstringToAnsiTrimBytes(const std::wstring& sIn)
{
    std::string result;
    result.reserve(sIn.size() + 1);
    for (size_t i = 0; i < sIn.size(); i++)
        result += (char)sIn[i];
    result += "\0";
    return result;
}


inline void ansiToWStringEx(const std::string& sIn, std::vector<wchar_t> &vOut)
{
#if defined(WIN32) || defined(WIN64)
    vOut.resize(sIn.size() + 1);
    std::locale loc;
    std::use_facet<std::ctype<wchar_t>>(loc).widen(sIn.c_str(), sIn.c_str() + sIn.size(), &vOut[0]);
#endif
}


inline std::string wstringToAnsi(const std::wstring& sIn)
{
#if defined(WIN32) || defined(WIN64)
    return str::_Narrow_str(sIn);
#endif
    return "";
}


inline std::string tstringToAnsi(const tstring& sIn)
{
#ifndef TCHAR_IS_W
    return sIn;
#else
    return str::_Narrow_str(sIn);
#endif
}


// Originally by Mark Ransom's UTF16toUTF8() (stackoverflow)
inline std::string wstringToUtf(const std::wstring&  sIn)
{
    std::string out;
    unsigned int codepoint;
    bool completecode = false;
    for (std::wstring::const_iterator p = sIn.begin();  p != sIn.end();  ++p)
    {
        if (*p >= 0xd800 && *p <= 0xdbff)
        {
            codepoint = ((*p - 0xd800) << 10) + 0x10000;
            completecode = false;
        }
        else if (!completecode && *p >= 0xdc00 && *p <= 0xdfff)
        {
            codepoint |= *p - 0xdc00;
            completecode = true;
        }
        else
        {
            codepoint = *p;
            completecode = true;
        }

        if (completecode)
        {
            if (codepoint <= 0x7f)
            {
                out.push_back(codepoint);
            }
            else if (codepoint <= 0x7ff)
            {
                out.push_back(0xc0 | ((codepoint >> 6) & 0x1f));
                out.push_back(0x80 | (codepoint & 0x3f));
            }
            else if (codepoint <= 0xffff)
            {
                out.push_back(0xe0 | ((codepoint >> 12) & 0x0f));
                out.push_back(0x80 | ((codepoint >> 6) & 0x3f));
                out.push_back(0x80 | (codepoint & 0x3f));
            }
            else
            {
                out.push_back(0xf0 | ((codepoint >> 18) & 0x07));
                out.push_back(0x80 | ((codepoint >> 12) & 0x3f));
                out.push_back(0x80 | ((codepoint >> 6) & 0x3f));
                out.push_back(0x80 | (codepoint & 0x3f));
            }
        }
    }
    return out;
}


inline std::string tstringToUtf(const tstring& sIn)
{
#if defined(WIN32) || defined(WIN64) //UTF16
    return wstringToUtf(sIn);
#else
    return sIn;
#endif
}


#if defined(WIN32) || defined(WIN64)
inline int utfToWString(const std::string& sIn, std::vector<tchar>& vBuf)
{
    const size_t cchUTF8 = sIn.size() + 1;
    int cbUTF8 = static_cast<int>(cchUTF8);
    int cchUTF16 = MultiByteToWideChar(
        CP_UTF8,                // convert from UTF-8
        MB_ERR_INVALID_CHARS,   // error on invalid chars
        sIn.c_str(),            // source UTF-8 string
        cbUTF8,                 // total length of source UTF-8 string, in CHAR's (= bytes), including end-of-string \0
        NULL,                   // unused - no conversion done in this step
        0                       // request size of destination buffer, in WCHAR's
    );

    //ERROR_INSUFFICIENT_BUFFER
    //int n = GetLastError();

    vBuf.resize(cchUTF16 + 1);

    int result = MultiByteToWideChar(
        CP_UTF8,                // convert from UTF-8
        MB_ERR_INVALID_CHARS,   // error on invalid chars
        sIn.c_str(),            // source UTF-8 string
        cbUTF8,                 // total length of source UTF-8 string, in CHAR's (= bytes), including end-of-string \0
        &vBuf[0],               // destination buffer
        cchUTF16                // size of destination buffer, in WCHAR's
        );

    vBuf[cchUTF16] = 0;

    return result;
}
#endif


inline std::wstring utfToWString(const std::string& sIn)
{
    if (sIn.empty())
        return L"";
#if defined(WIN32) || defined(WIN64)
    const size_t cchUTF8 = sIn.size() + 1;
    int cbUTF8 = static_cast<int>(cchUTF8);
    int cchUTF16 = MultiByteToWideChar(
        CP_UTF8,                // convert from UTF-8
        MB_ERR_INVALID_CHARS,   // error on invalid chars
        sIn.c_str(),            // source UTF-8 string
        cbUTF8,                 // total length of source UTF-8 string, in CHAR's (= bytes), including end-of-string \0
        NULL,                   // unused - no conversion done in this step
        0                       // request size of destination buffer, in WCHAR's
    );
    //ERROR_INSUFFICIENT_BUFFER
    int uiRetCode = GetLastError();
    if (uiRetCode == 1113 && cchUTF16 == 0)
        cchUTF16 = sIn.size();

    std::vector<wchar_t> vBuf;
    vBuf.resize(cchUTF16);

    int result = MultiByteToWideChar(
        CP_UTF8,                // convert from UTF-8
        MB_ERR_INVALID_CHARS,   // error on invalid chars
        sIn.c_str(),            // source UTF-8 string
        cbUTF8,                 // total length of source UTF-8 string, in CHAR's (= bytes), including end-of-string \0
        &vBuf[0],               // destination buffer
        cchUTF16                // size of destination buffer, in WCHAR's
        );

    if (result == 0)
        0;//handle error

    return &vBuf[0];
#endif

    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> conversion;
    return conversion.from_bytes(sIn.c_str());
}


inline tstring utfToTString(const std::string& sIn)
{
#ifdef TCHAR_IS_W //UTF16
    return utfToWString(sIn);
#else
    return sIn;
#endif
}


inline int compareStringIgnoreCase(std::string sStr1, std::string sStr2)
{
    const char* szStr1 = sStr1.c_str();
    const char* szStr2 = sStr2.c_str();
    const size_t nLen1 = strlen(szStr1);
    const size_t nLen2 = strlen(szStr2);

    if (nLen1 != nLen2)
        return (nLen1 > nLen2) ? 1 : -1;

    std::locale loc;
    char chr1;
    char chr2;

    for (size_t i = 0; i < nLen1; ++i)
    {
        chr1 = std::toupper<char>(szStr1[i], loc);
        chr2 = std::toupper<char>(szStr2[i], loc);
        if (chr1 != chr2)
            return (chr1 > chr2) ? 1 : -1;
    }

    return 0;
}

#if defined(WIN32) || defined(WIN64)
inline int compareStringIgnoreCase(std::wstring sStr1, std::wstring sStr2)
{
    return compareStringIgnoreCase(str::tstringToUtf(sStr1), str::tstringToUtf(sStr2));
}
#endif


inline bool isBlanckSymbol(char chr)
{
    return (
        chr == ' '  ||
        chr == '\r' ||
        chr == '\t' ||
        chr == '\n' ||
        chr == '\v' ||
        chr == '\a' ||
        chr == '\b' ||
        chr == '\f');
}


inline std::string toHex(const unsigned char *pData, size_t size)
{
    std::string sRet;
    char buf[3] = {0};
    for (size_t i = 0; i < size; ++i)
    {
        if (pData[i] < 16)
        {
            itoa_safe(pData[i], (&buf[0] + 1), 3, 16);
            buf[0] = '0';
        }
        else
        {
            itoa_safe(pData[i], (&buf[0] + 1), 3, 16);
        }
        sRet.append(buf);
    }    
    return sRet;
}


/** \brief Safe copy function.
  * \param pSource a NULL-terminated string
  *
  * This function sets the last char to \0.
  */
// source - 
template <std::size_t D>
inline void safeCopy(char(&dest)[D], const char *pSource)
{ 
    strncpy(dest, pSource, D - 1);
    dest[D - 1] = '\0';
}

}
