#ifndef COMMON__STR_MANIP_H
#define COMMON__STR_MANIP_H

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

//
typedef std::vector<std::string> StrVector;

#ifndef U2T
#   if defined(WIN32) || defined(WIN64)
#       define U2T str::utfToWString
#   else
#       define U2T
#   endif
#endif


namespace str {
#if defined(WIN32) || defined(WIN64)
/*!
 * \fn  inline std::string _Narrow_str(std::wstring _Str)
 * \brief   Convert wchar_t string to char string.
 * \param   _Str    The string.
 * \return  A std::string.
 */
inline std::string _Narrow_str(std::wstring _Str) {    // convert wchar_t string to char string
    std::string _Ans;

    for (const wchar_t *_Ptr = _Str.c_str(); *_Ptr != 0; ++_Ptr)
        _Ans.push_back((char)_CSTD wctob(*_Ptr));
    return (_Ans);
}
#endif


/*!
 * \fn  inline bool isNumber(char c)
 * \brief   Query if 'c' is number.
 * \param   c   The character.
 * \return  true if number, false if not.
 */
inline bool isNumber(char c) {
    return c <= 57 && c >= 48;
}


/*!
 * \fn  inline bool isHex(const std::string& in)
 * \brief   Query if 'in' is hexadecimal.
 * \param   in The in.
 * \return  true if hexadecimal, false if not.
 */
inline bool isHex(const std::string& in) {
    bool result = true;
    for (size_t i = 0; i < in.size(); i++) {
        if (iswxdigit((wint_t)in[i]) == 0) {
            result = false;
            break;
        }
    }
    return result;
}


/*!
 * \fn  inline int stringToInt(const std::string& in)
 * \brief   Convert string to int.
 * \param   in The in.
 * \return  An int.
 */
inline int stringToInt(const std::string& in) {
    return atoi(in.c_str());
}


/*!
 * \fn  inline int stringToUInt(const std::string& in)
 * \brief   Convert string to unsigned int.
 * \param   in The in.
 * \return  An int.
 */
inline unsigned int stringToUInt(const std::string& in) {
    return strtoul(in.c_str(), nullptr, 10);
}


/*!
 * \fn  inline long long stringToLLong(const std::string& in)
 * \brief   Convert string to long long.
 * \param   in The in.
 * \return  A long.
 */
inline long long stringToLLong(const std::string& in) {
#if defined(WIN32) || defined(WIN64)
    return _strtoi64(in.c_str(), NULL, 10);
#else
    return strtoll(in.c_str(), NULL, 10);
#endif
}


/*!
* \fn  inline unsigned long long stringToULLong(const std::string& in)
* \brief   Convert string to unsigned long long.
* \param   in The in.
* \return  A unsigned long long.
*/
inline unsigned long long stringToULLong(const std::string& in) {
#if defined(WIN32) || defined(WIN64)
    return _strtoui64(in.c_str(), NULL, 10);
#else
    return strtoull(in.c_str(), NULL, 10);
#endif
}


/*!
 * \fn  inline double stringToDouble(const std::string& in)
 * \brief   Convert string to double.
 * \param   in The in.
 * \return  A double.
 */
inline double stringToDouble(const std::string& in) {
    return atof(in.c_str());;
}


/*!
 * \fn  inline std::string longLongToString(long long cchValue)
 * \brief   Convert long long to string.
 * \param   cchValue    The cch value.
 * \return  A std::string.
 */
inline std::string longLongToString(long long cchValue) {
    const int nBufSize = 64;
    char szBuf[nBufSize];
#if defined(WIN32) || defined(WIN64)
    sprintf_s(szBuf, nBufSize, "%lld", cchValue);
#else
    sprintf(szBuf, "%lld", cchValue);
#endif
    return szBuf;
}


/*!
 * \fn  inline std::string doubleToString(double cchValue)
 * \brief   Convert double to string.
 * \param   cchValue    The in.
 * \return  A std::string.
 */
inline std::string doubleToString(double cchValue) {
    const int nBufSize = 32;
    char szBuf[nBufSize];
#if defined(WIN32) || defined(WIN64)
    sprintf_s(szBuf, nBufSize, "%g", cchValue);
#else
    sprintf(szBuf, "%g", cchValue);
#endif
    return szBuf;
}


/*!
 * \fn  inline std::string intToString(int nValue)
 * \brief   Convert int to string.
 * \param   nValue  The in.
 * \return  A std::string.
 */
inline std::string intToString(int nValue) {
    const int nBufSize = 64;
    char szBuf[nBufSize];
#if defined(WIN32) || defined(WIN64)
    sprintf_s(szBuf, nBufSize, "%d", nValue);
#else
    sprintf(szBuf, "%d", nValue);
#endif
    return szBuf;
}


/*!
* \fn  inline std::string intToString(int nValue)
* \brief   Convert int to string.
* \param   nValue  The in.
* \return  A std::string.
*/
inline std::string uintToString(unsigned int nValue) {
    const int nBufSize = 64;
    char szBuf[nBufSize];
#if defined(WIN32) || defined(WIN64)
    sprintf_s(szBuf, nBufSize, "%u", nValue);
#else
    sprintf(szBuf, "%u", nValue);
#endif
    return szBuf;
}


#if defined(WIN32) || defined(WIN64)
#define sizetToString intToString
#else
#define sizetToString longLongToString
#endif


/*!
 * \fn  inline std::wstring intToWString(int nIn)
 * \brief   Convert int to wstring.
 * \param   nIn The in.
 * \return  A std::wstring.
 */
inline std::wstring intToWString(int nIn) {
    const size_t nBufSize = 64;
    wchar_t buf[nBufSize];
#if defined(WIN32) || defined(WIN64)
    _itow_s(nIn, buf, nBufSize, 10);
#endif
    return buf;
}


/*!
 * \fn  inline std::string wstringToAnsiTrimBytes(const std::wstring& in)
 * \brief   Convert wstring to ANSI trim bytes.
 * \param   in The in.
 * \return  A std::string.
 */
inline std::string wstringToAnsiTrimBytes(const std::wstring& in) {
    std::string result;
    result.reserve(in.size() + 1);
    for (size_t i = 0; i < in.size(); i++)
        result += (char)in[i];
    result += "\0";
    return result;
}


/*!
 * \fn  inline void ansiToWStringEx(const std::string& in, std::vector<wchar_t> &vOut)
 * \brief   Convert ANSI to extended wstring.
 * \param   in             The in.
 * \param [in,out]  vOut    The out.
 */
inline void ansiToWStringEx(const std::string& in, std::vector<wchar_t> &vOut) {
#if defined(WIN32) || defined(WIN64)
    vOut.resize(in.size() + 1);
    std::locale loc;
    std::use_facet<std::ctype<wchar_t>>(loc).widen(in.c_str(), in.c_str() + in.size(), &vOut[0]);
#endif
}


/*!
 * \fn  inline std::string wstringToAnsi(const std::wstring& in)
 * \brief   Convert wstring to ANSI.
 * \param   in The in.
 * \return  A std::string.
 */
inline std::string wstringToAnsi(const std::wstring& in) {
#if defined(WIN32) || defined(WIN64)
    return str::_Narrow_str(in);
#endif
    return wstringToAnsiTrimBytes(in);
}


/*!
 * \fn  inline std::string wstringToUtf(const std::wstring& in)
 * \brief   Convert wstring to UTF. Originally by Mark Ransom's UTF16toUTF8() (stackoverflow)
 * \param   in The in.
 * \return  A std::string.
 */
inline std::string wstringToUtf(const std::wstring&  in) {
    std::string out;
    unsigned int codepoint;
    bool completecode = false;
    for (std::wstring::const_iterator p = in.begin(); p != in.end(); ++p) {
        if (*p >= 0xd800 && *p <= 0xdbff) {
            codepoint = ((*p - 0xd800) << 10) + 0x10000;
            completecode = false;
        }
        else if (!completecode && *p >= 0xdc00 && *p <= 0xdfff) {
            codepoint |= *p - 0xdc00;
            completecode = true;
        }
        else {
            codepoint = *p;
            completecode = true;
        }

        if (completecode) {
            if (codepoint <= 0x7f) {
                out.push_back(codepoint);
            }
            else if (codepoint <= 0x7ff) {
                out.push_back(0xc0 | ((codepoint >> 6) & 0x1f));
                out.push_back(0x80 | (codepoint & 0x3f));
            }
            else if (codepoint <= 0xffff) {
                out.push_back(0xe0 | ((codepoint >> 12) & 0x0f));
                out.push_back(0x80 | ((codepoint >> 6) & 0x3f));
                out.push_back(0x80 | (codepoint & 0x3f));
            }
            else {
                out.push_back(0xf0 | ((codepoint >> 18) & 0x07));
                out.push_back(0x80 | ((codepoint >> 12) & 0x3f));
                out.push_back(0x80 | ((codepoint >> 6) & 0x3f));
                out.push_back(0x80 | (codepoint & 0x3f));
            }
        }
    }
    return out;
}


/*!
 * \fn  inline std::wstring utfToWString(const std::string& in)
 * \brief   Convert UTF to wstring.
 * \param   in The in.
 * \return  A std::wstring.
 */
inline std::wstring utfToWString(const std::string& in) {
    if (in.empty())
        return L"";
#if defined(WIN32) || defined(WIN64)
    const size_t cchUTF8 = in.size() + 1;
    int cbUTF8 = static_cast<int>(cchUTF8);
    int cchUTF16 = MultiByteToWideChar(
        CP_UTF8,                // convert from UTF-8
        MB_ERR_INVALID_CHARS,   // error on invalid chars
        in.c_str(),            // source UTF-8 string
        cbUTF8,                 // total length of source UTF-8 string, in CHAR's (= bytes), including end-of-string \0
        NULL,                   // unused - no conversion done in this step
        0                       // request size of destination buffer, in WCHAR's
    );
    //ERROR_INSUFFICIENT_BUFFER
    int uiRetCode = GetLastError();
    if (uiRetCode == 1113 && cchUTF16 == 0)
        cchUTF16 = in.size();

    std::vector<wchar_t> vBuf;
    vBuf.resize(cchUTF16);

    int result = MultiByteToWideChar(
        CP_UTF8,                // convert from UTF-8
        MB_ERR_INVALID_CHARS,   // error on invalid chars
        in.c_str(),            // source UTF-8 string
        cbUTF8,                 // total length of source UTF-8 string, in CHAR's (= bytes), including end-of-string \0
        &vBuf[0],               // destination buffer
        cchUTF16                // size of destination buffer, in WCHAR's
    );

    if (result == 0)
        0;//handle error

    return &vBuf[0];
#endif

    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> conversion;
    return conversion.from_bytes(in.c_str());
}

/*!
 * \fn  inline int compareStringIgnoreCase(std::string sStr1, std::string sStr2)
 * \brief   Compare string ignore case.
 * \param   sStr1   The first s string.
 * \param   sStr2   The second s string.
 * \return  An int.
 */
inline int compareStringIgnoreCase(std::string sStr1, std::string sStr2) {
    const char* szStr1 = sStr1.c_str();
    const char* szStr2 = sStr2.c_str();
    const size_t nLen1 = strlen(szStr1);
    const size_t nLen2 = strlen(szStr2);

    if (nLen1 != nLen2)
        return (nLen1 > nLen2) ? 1 : -1;

    std::locale loc;
    char chr1;
    char chr2;

    for (size_t i = 0; i < nLen1; ++i) {
        chr1 = std::toupper<char>(szStr1[i], loc);
        chr2 = std::toupper<char>(szStr2[i], loc);
        if (chr1 != chr2)
            return (chr1 > chr2) ? 1 : -1;
    }

    return 0;
}


/*!
 * \fn  inline void safeCopy(char(&dest)[D], const char *pSource)
 * \brief   Safe copy.
 * \param [in,out]  dest)   The destination.
 * \param   pSource         The source.
 */
template <std::size_t D>
inline void safeCopy(char(&dest)[D], const char *pSource) {
    strncpy(dest, pSource, D - 1);
    dest[D - 1] = '\0';
}


/*!
 * \fn  inline bool isBlanckSymbol(char chr)
 * \brief   Query if 'chr' is blanck symbol.
 * \param   chr The character.
 * \return  true if blanck symbol, false if not.
 */
inline bool isBlanckSymbol(char chr) {
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


/*!
 * \fn  inline std::string toHex(const unsigned char *data, int len)
 * \brief   Convert unsigned char to a hexadecimal.
 * \param   pData   The data.
 * \param   size    The size.
 * \return  The given data converted to a hexadecimal(std::string).
 */
inline std::string toHex(const unsigned char *data, int len) {
    const char hexmap[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

    std::string s(len * 2, ' ');
    for (int i = 0; i < len; ++i) {
        s[2 * i] = hexmap[(data[i] & 0xF0) >> 4];
        s[2 * i + 1] = hexmap[data[i] & 0x0F];
    }
    return s;
}



/*!
* \fn  inline std::string toHex(const std::string& input)
* \brief   Convert ascii string to hex.
* \param   const std::string& input   The input.
* \return  The given data converted to hexadecimal.
*/
inline std::string toHex(const std::string& input) {
    static const char* const lut = "0123456789abcdef";
    size_t len = input.length();

    std::string output;
    output.reserve(2 * len);
    for (size_t i = 0; i < len; ++i) {
        const unsigned char c = input[i];
        output.push_back(lut[c >> 4]);
        output.push_back(lut[c & 15]);
    }
    return output;
}


inline std::string hexToString(const std::string& input) {
    static const char* const lut = "0123456789abcdef";
    size_t len = input.length();
    if (len & 1)
        return "";//    throw std::invalid_argument("odd length");

    std::string output;
    output.reserve(len / 2);
    for (size_t i = 0; i < len; i += 2) {
        char a = input[i];
        const char* p = std::lower_bound(lut, lut + 16, a);
        if (*p != a)
            throw std::invalid_argument("not a hex digit");

        char b = input[i + 1];
        const char* q = std::lower_bound(lut, lut + 16, b);
        if (*q != b)
            throw std::invalid_argument("not a hex digit");

        output.push_back(((p - lut) << 4) | (q - lut));
    }
    return output;
}


inline void hexToBytes(const std::string& input, std::vector<unsigned char>& output) {
    static const char* const lut = "0123456789ABCDEF";
    size_t len = input.length();
    if (len & 1)
        return; //throw std::invalid_argument("odd length");

    output.reserve(len / 2);
    for (size_t i = 0; i < len; i += 2) {
        char a = input[i];
        const char* p = std::lower_bound(lut, lut + 16, a);
        if (*p != a)
            throw std::invalid_argument("not a hex digit");

        char b = input[i + 1];
        const char* q = std::lower_bound(lut, lut + 16, b);
        if (*q != b)
            throw std::invalid_argument("not a hex digit");

        output.push_back(((p - lut) << 4) | (q - lut));
    }
}

}

#endif // COMMON__STR_MANIP_H
