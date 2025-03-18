#ifndef COMMON_STR_PARSE_H
#define COMMON_STR_PARSE_H

#include <string>
#include <list>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <locale>
#include <stdarg.h>
#include <regex>

#include "str_manip.h"


//Helper macros for search functions
#define ADD_WS(x)               patt.emplace_back(LL(x))
#define ADD_S(x)                patt.emplace_back(x)
#define MAKE_PATT3(s1, s2, s3)   std::vector<std::string> patt; patt.emplace_back(s1); patt.emplace_back(s2); patt.emplace_back(s3);
#define FILL_PATT3(s1, s2, s3)   patt.clear(); patt.emplace_back(s1); patt.emplace_back(s2); patt.emplace_back(s3);

namespace str {

enum EVerDifference {
    eEqual = -1,
    eMajor,
    eMinor,
    eBuild,
    eTimestamp
};


/*!
 * \fn  inline size_t findLastNotOf(const std::basic_string<T>& input, const T charToTrim)
 * \brief   Search for the last charter that is not charToTrim.
 * \param   input         The in.
 * \param   charToTrim  The character to trim.
 * \return  The position of charter.
 */
template<typename T>
inline size_t findLastNotOf(const std::basic_string<T>& input, const T charToTrim) {
    size_t pos = input.size();
    while (pos-- > 0) {
        if (input[pos] != charToTrim)
            break;
    }
    return pos;
}


/*!
 * \fn  inline std::string to_lower(const std::string& input)
 * \brief   Convert string to a lower.
 * \param   input The in.
 * \return  input as a std::string.
 */
inline std::string to_lower(const std::string& input) {
    std::string result = input;
    std::transform(input.begin(), input.end(), result.begin(), tolower);
    return result;
}



/*!
 * \fn  inline std::string to_upper(const std::string& input)
 * \brief   Convert string to an upper.
 * \param   input The in.
 * \return  input as a std::string.
 */
inline std::string to_upper(const std::string& input) {
    std::string result = input;
    std::transform(input.begin(), input.end(), result.begin(), toupper);
    return result;
}


/*!
 * \fn  inline bool stringToBool(const std::string& input)
 * \brief   Convert string to bool.
 * \param   input The in.
 * \return  true if input equal 1 or true or yes or ok.
 */
inline bool stringToBool(const std::string& input) {
    std::string sLower = to_lower(input);
    return (sLower == "1" || sLower == "true" || sLower == "yes" || sLower == "ok");
}


/*!
 * \fn  inline std::string& trim_left(std::string& input)
 * \brief   Trim left for string.
 * \param [in,out]  input The in.
 * \return  A std::string&amp;
 */
inline std::string& trim_left(std::string& input) {
    if (input.empty())
        return input;
    size_t pos = input.find_first_not_of(' ');
    input.erase(0, pos);
    return input;
}


/*!
 * \fn  inline std::wstring& trim_left(std::wstring& input)
 * \brief   Trim left for wstring.
 * \param [in,out]  Input string, to be modified.
 * \return  A reference to the modified input.
 */
inline std::wstring& trim_left(std::wstring& input) {
    if (input.empty())
        return input;
    size_t pos = input.find_first_not_of(L' ');
    input.erase(0, pos);
    return input;
}


/*!
 * \fn  inline void trim_left(std::basic_string<T>& input, const T& charsToTrim)
 * \brief   Trim left.
 * \param [in,out]  input The in.
 * \param   charsToTrim The characters to trim.
 */
template<typename T>
inline void trim_left(std::basic_string<T>& input, const T& charsToTrim) {
    if (input.empty())
        return;
    size_t pos = input.find_first_not_of(charsToTrim);
    input.erase(0, pos);
}


/*!
 * \fn  inline std::basic_string<T> trim_left_copy(const std::basic_string<T>& input, const T& charsToTrim)
 * \brief   Trim left copy.
 * \param   input         The in.
 * \param   charsToTrim The characters to trim.
 * \return  A std::basic_string&lt;T&gt;
 */
template<typename T>
inline std::basic_string<T> trim_left_copy(const std::basic_string<T>& input, const T& charsToTrim) {
    std::basic_string<T> sResult = input;
    trim_left(sResult, charsToTrim);
    return sResult;
}


/*!
 * \fn  inline std::wstring trim_left_copy(const std::wstring& input)
 * \brief   Trim left copy for wstring.
 * \param   input The in.
 * \return  A std::wstring.
 */
inline std::wstring trim_left_copy(const std::wstring& input) {
    if (input.empty())
        return input;
    return input.substr(input.find_first_not_of(L' '));
}


/*!
 * \fn  inline std::string trim_left_copy(const std::string& input)
 * \brief   Trim left copy for string.
 * \param   input The in.
 * \return  A std::string.
 */
inline std::string trim_left_copy(const std::string& input) {
    if (input.empty())
        return input;
    return input.substr(input.find_first_not_of(' '));
}


/*!
 * \fn  inline std::string& trim_right(std::string& input)
 * \brief   Trim right for string.
 * \param [in,out]  input The in.
 * \return  A std::string&amp;
 */
inline std::string& trim_right(std::string& input) {
    if (input.empty())
        return input;
    size_t pos = input.find_last_not_of(' ');
    input.erase(pos + 1);
    return input;
}


/*!
 * \fn  inline std::wstring& trim_right(std::wstring& input)
 * \brief   Trim right for wstring.
 * \param [in,out]  input The in.
 * \return  A std::wstring&amp;
 */
inline std::wstring&  trim_right(std::wstring& input) {
    if (input.empty())
        return input;
    size_t pos = input.find_last_not_of(L' ');
    input.erase(pos + 1);
    return input;
}


/*!
 * \fn  inline void trim_right(std::basic_string<T> & input, const T& charToTrim)
 * \brief   Trim right.
 * \param [in,out]  input The in.
 * \param   charToTrim  The character to trim.
 */
template<typename T>
inline void trim_right(std::basic_string<T> & input, const T& charToTrim) {
    if (input.empty())
        return;
    size_t pos = input.find_last_not_of(charToTrim);
    input.erase(pos + 1);
}


/*!
 * \fn  inline void trim_right(std::basic_string<T> & input, const T *charsToTrim)
 * \brief   Trim right.
 * \param [in,out]  input The in.
 * \param   charsToTrim The characters to trim.
 */
template<typename T>
inline void trim_right(std::basic_string<T> & input, const T *charsToTrim) {
    if (input.empty() || charsToTrim == nullptr)
        return;
    size_t pos = input.find_last_not_of(charsToTrim);
    input.erase(pos + 1);
}


/*!
 * \fn  inline void trimRight(std::basic_string<T> & input, const T charToTrim)
 * \brief   Trim right.
 * \param [in,out]  input The in.
 * \param   charToTrim  The character to trim.
 */
template<typename T>
inline void trimRight(std::basic_string<T> & input, const T charToTrim) {
    if (input.empty())
        return;
    size_t pos = findLastNotOf(input, charToTrim);
    input.erase(pos + 1);
}


/*!
 * \fn  inline std::string trim_right_copy(const std::string& input)
 * \brief   Trim right copy for string.
 * \param   input The in.
 * \return  A std::string.
 */
inline std::string trim_right_copy(const std::string& input) {
    if (input.empty())
        return input;
    return input.substr(0, input.find_last_not_of(' ') + 1);
}


/*!
 * \fn  inline std::wstring trim_right_copy(const std::wstring& input)
 * \brief   Trim right copy for wstring.
 * \param   input The in.
 * \return  A std::wstring.
 */
inline std::wstring trim_right_copy(const std::wstring& input) {
    if (input.empty())
        return input;
    return input.substr(0, input.find_last_not_of(L' ') + 1);
}


#if defined(WIN32) || defined(WIN64)
/*!
 * \fn  inline int compare_ci(const std::wstring strVal1, const wchar_t *pszVal2)
 * \brief   Compare(case sensitive).
 * \param   strVal1 The string value.
 * \param   pszVal2 The wchar_t value.
 * \return  An int (if strVal1 &gt; pszVal2 return 1, if if strVal1 &lt; pszVal2 return -1 else
 *          return 0).
 */
inline int compare_ci(const std::wstring strVal1, const wchar_t *pszVal2) {
    return ::_wcsicmp(strVal1.c_str(), pszVal2);
}
#endif


/*!
 * \struct  caseInsensitiveComparer
 * \brief   A case insensitive comparer.
 */
template<typename charT>
struct caseInsensitiveComparer {
    caseInsensitiveComparer(const std::locale& loc) : loc_(loc) {}

    /*!
     * \fn  bool operator()(charT ch1, charT ch2)
     * \brief   Function call operator.
     * \param   ch1 The first ch.
     * \param   ch2 The second ch.
     * \return  Return true if ch1 is equal ch2.
     */
    bool operator()(charT ch1, charT ch2) {
        return std::tolower(ch1, loc_) == std::tolower(ch2, loc_);
    }
private:
    const std::locale& loc_;
};


/*!
 * \fn  inline bool endsWith(const std::basic_string<T> &str, const std::basic_string<T> &what)
 * \brief   Checks whether the string ends with the given substring.
 * \param   input       The in.
 * \param   what        What to find.
 * \return  true if it succeeds, false if it fails.
 */
template<typename T>
inline bool startsWith(
                        const std::basic_string<T> &str,
                        const std::basic_string<T> &what) {
    if (str.empty())
        return false;
    //TODO:
    return strncmp(str.c_str(), what.c_str(), what.size());
}


/*!
 * \fn  inline bool endsWith(const std::basic_string<T> & input, const std::basic_string<T> & what)
 * \brief   Find what in input.
 * \param   input     The in.
 * \param   what   What to find.
 * \return  true if it succeeds, false if it fails.
 */
template<typename T>
inline bool endsWith(
                    const std::basic_string<T> &input,
                    const std::basic_string<T> &what) {
    size_t sizePos = input.find(what);
    if (sizePos == -1)
        return false;
    else
        return sizePos == input.size() - what.size();
}


/*!
 * \fn  size_t find_ci(const std::basic_string<T> & input, const std::basic_string<T> & what, const std::locale & loc = std::locale())
 * \brief   Search what in input.
 * \param   input     The in.
 * \param   what   What to find.
 * \param   loc     The std::locale.
 * \return  The position or -1 if not found.
 */
template<typename T>
size_t find_ci(
                const std::basic_string<T> &input,
                const std::basic_string<T> &what,
                const std::locale &loc = std::locale()) {
    typename std::basic_string<T>::const_iterator it = std::search(input.begin(), input.end(),
        what.begin(), what.end(), caseInsensitiveComparer<T>(loc));
    if (it != input.end())
        return it - input.begin();
    else
        return -1;
}


/*!
 * \fn  inline size_t find_ci(std::string::const_iterator itInStart, std::string::const_iterator itInEnd, const std::string &what, const std::locale &loc = std::locale())
 * \brief   Search what in input.
 * \param   itInStart   The iterator in start.
 * \param   itInEnd     The iterator in end.
 * \param   what        What to find.
 * \param   loc         The std::locale.
 * \return  The position or -1 if not found.
 */
inline size_t find_ci(
                        std::string::const_iterator itInStart,
                        std::string::const_iterator itInEnd,
                        const std::string &what,
                        const std::locale &loc = std::locale()) {
    std::string::const_iterator it = std::search(itInStart, itInEnd,
        what.begin(), what.end(), caseInsensitiveComparer<typename std::string::value_type>(loc));
    if (it != itInEnd)
        return it - itInStart;
    else
        return -1;
}


/*!
 * \fn  inline std::wstring replace(const std::wstring& input, const std::wstring &what, const std::wstring &new_str)
 * \brief   Replace.
 * \param   input   The input.
 * \param   what    What to find.
 * \param   new_str   The with.
 * \return  A std::wstring.
 */
inline std::wstring replace(
                            const std::wstring &input,
                            const std::wstring &what,
                            const std::wstring &new_str) {
    std::wstring result = input;
    size_t pos = 0;
    size_t cchWhat = what.size();
    size_t cchWith = new_str.size();

    while (true) {
        pos = result.find(what, pos);// + cchWhat - cchWith + 1);
        if (pos == -1)
            break;
        result.erase(pos, what.size());
        result.insert(pos, new_str);
        pos += cchWith - cchWhat + 1;
    }

    return std::move(result);
}


/*!
 * \fn  inline std::string replace(const std::string& input, const std::string &what, const std::string &new_str)
 * \brief   Replace.
 * \param   input       The input.
 * \param   what        What to find.
 * \param   new_str     The with.
 * \return  A std::string.
 */
inline std::string replace(
                            const std::string &input,
                            const std::string &what,
                            const std::string &new_str) {
    std::string result = input;
    size_t pos = 0;
    size_t cchWhat = what.size();
    size_t cchWith = new_str.size();

    while (true) {
        pos = result.find(what, pos);// + cchWhat - cchWith + 1);
        if (pos == -1)
            break;
        result.erase(pos, what.size());
        result.insert(pos, new_str);
        pos += cchWith - cchWhat + 1;
    }

    return std::move(result);
}


/*!
 * \fn  inline std::wstring& replace(std::wstring& input, const std::wstring &what, const std::wstring &new_str)
 * \brief   Replace.
 * \param [in,out]  input The in.
 * \param   what       What to find.
 * \param   new_str       The with.
 * \return  A std::wstring&amp;
 */
inline std::wstring& replace(std::wstring& input, const std::wstring &what, const std::wstring &new_str) {
    size_t pos = 0;
    size_t cchWhat = what.size();
    size_t cchWith = new_str.size();

    while (true) {
        pos = input.find(what, pos);// + cchWhat - cchWith + 1);
        if (pos == -1)
            break;
        input.erase(pos, what.size());
        input.insert(pos, new_str);
        pos += cchWith - cchWhat + 1;
    }

    return input;
}


/*!
 * \fn  inline std::string& replace(std::string &input, const std::string &what, const std::string &new_str)
 * \brief   Replace.
 * \param [in,out]input The input.
 * \param   what        What to find.
 * \param   new_str     The new_str.
 * \return  A reference to the modified input.
 */
inline std::string& replace(
                            std::string &input,
                            const std::string &what,
                            const std::string &new_str) {
    size_t pos = 0;
    size_t cchWhat = what.size();
    size_t cchWith = new_str.size();

    while (true) {
        pos = input.find(what, pos);// + cchWhat - cchWith + 1);
        if (pos == -1)
            break;
        input.erase(pos, what.size());
        input.insert(pos, new_str);
        pos += cchWith - cchWhat + 1;
    }

    return input;
}


inline int count(const std::string& str, const std::string& sub) {
    if (sub.empty())
        return 0;

    int count = 0;
    for (size_t offset = str.find(sub); offset != std::string::npos; offset = str.find(sub, offset + sub.length()))
        ++count;

    return count;
}


inline bool hexstrToBytes(
                            const std::string &hex_str,
                            std::vector<char> &bytes) {
    bytes.clear();

    const size_t len = hex_str.length();
    char buf[3] = { 0 };
    int num;

    for (size_t i = 0; i < len; i = i + 2) {
        if (i + 1 >= len)
            return false;
        buf[0] = hex_str[i];
        buf[1] = hex_str[i + 1];
        if (buf[0] < 0 || isxdigit((unsigned)buf[0]) == 0)
            return false;
        if (buf[1] < 0 || isxdigit((unsigned)buf[1]) == 0)
            return false;

        num = strtoul(buf, nullptr, 16);
        bytes.push_back((char)num);
    }
    return true;
}


inline std::string bytesToHexstr(
                                const unsigned char *data,
                                size_t data_size) {
    if (!data)
        return "";
    std::string hex_str;
    char buf[3] = { 0 };
    for (size_t i = 0; i < data_size; ++i) {
        if (data[i] < 16) {
            _itoa(data[i], &buf[1], 16);
            buf[0] = '0';
        }
        else
            _itoa(data[i], buf, 16);
        hex_str.append(buf);
    }
    return hex_str;
}


inline std::string bytesToHexstr(const std::vector<char> &data) {
    if (data.empty())
        return "";
    return bytesToHexstr((const unsigned char*)&data[0], data.size());
}


//template <typename T> //char_traits<tchar>, allocator<tchar>>
//std::basic_stringstream<T> toString(const T &val) {
//    std::basic_stringstream<T> out;
//    out << val;
//    return out.str();
//}


/*!
 * \fn  inline std::string formatStr(const std::string &format_str, const std::string &arg)
 * \brief   Format string, with printf [%s] syntax.
 * \param   format_str Describes the format to use.
 * \param   arg    The string.
 * \return  The formatted string.
 */
inline std::string formatStr(const std::string &format_str, const std::string &arg) {
    std::vector<char> vBuf;
    vBuf.resize(format_str.size() /*%s*/ - 2 + arg.size() + /*'\0'*/1);
#if defined(WIN32) || defined(WIN64) //UTF16 system
#pragma warning( push )
#pragma warning ( disable : 4996 )
    sprintf((char *)&vBuf[0], format_str.c_str(), arg.c_str());
#pragma warning( pop )
#else
    sprintf((char *)&vBuf[0], format_str.c_str(), arg.c_str());
#endif
    return &vBuf[0];
}


/*!
 * \fn  inline std::string formatStr(size_t nBuffSize, char *fmt, ...)
 * \brief   Format string.
 * \param   nBuffSize   Size of the buffer.
 * \param [in,out]  fmt describes the format to use.
 * \param   ...         Variable arguments.
 * \return  The formatted string.
 */
inline std::string formatStr(size_t nBuffSize, char *fmt, ...) {
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





/*!
 * \fn  inline std::wstring removeChars(std::wstring& input, const std::wstring& sChars)
 * \brief   Remove the characters.
 * \param [in,out]  input The in.
 * \param   sChars      The characters.
 * \return  A std::wstring.
 */
inline std::wstring removeChars(std::wstring& input, const std::wstring& sChars) {
    for (std::wstring::const_iterator it = input.begin(); it != input.end();) {
        if (sChars.find(*it) != -1)
            input.erase(it, it + 1);
        else
            it++;
    }

    return input;
}


/*!
 * \fn  inline std::string removeChars(std::string& input, const std::string& sChars)
 * \brief   Remove the characters.
 * \param [in,out]  input The in.
 * \param   sChars      The characters.
 * \return  A std::string.
 */
inline std::string removeChars(std::string& input, const std::string& sChars) {
    for (std::string::const_iterator it = input.begin(); it != input.end();) {
        if (sChars.find(*it) != -1)
            input.erase(it, it + 1);
        else
            it++;
    }

    return input;
}


/*!
 * \fn  inline void tokenize(const std::basic_string<T>& input, const std::basic_string<T>& separator, std::vector<std::basic_string<T>>& vec_parts)
 * \brief   Tokenize string.
 * \param   input                 The in.
 * \param   separator          The separator.
 * \param [in,out]  vec_parts   The vector parts.
 */
template <typename T>
inline void tokenize(
                    const std::basic_string<T> &input,
                    const std::basic_string<T> &separator,
                    std::vector<std::basic_string<T>> &vec_parts) {
    if (separator.empty())
        return;

    size_t cchStart = 0;
    size_t cchEnd = input.find(separator, cchStart);

    vec_parts.clear();

    if (input.empty())
        return;

    if (cchEnd == -1) {
        vec_parts.push_back(input);
    }
    else {
        size_t cchSrcLen = input.size();
        size_t cchSeparatorLen = separator.size();

        do {
            vec_parts.push_back(input.substr(cchStart, cchEnd - cchStart));

            cchStart = cchEnd + cchSeparatorLen;
            cchEnd = input.find(separator, cchStart);

            if (cchEnd == -1)
                vec_parts.push_back(input.substr(cchStart, cchSrcLen - cchStart));
        } while (cchEnd != -1);
    }
}


/*!
 * \fn  inline const char *rstrstr(const char *sSrc, size_t sizeSrcLen, const char *s2)
 * \brief   Restrict string.
 * \param   sSrc        Source .
 * \param   sizeSrcLen  Length of the size source.
 * \param   s2          The mask.
 * \return  null if it fails, else a char* that begins as sSrc.
 */
inline const char *rstrstr(const char *sSrc, size_t sizeSrcLen, const char *s2) {
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


/*!
 * \fn  inline std::basic_string<T> makeSafeFileName(const std::basic_string<T> &sVal)
 * \brief   Make safe file name.
 * \param   sVal    The value.
 * \return  A std::basic_string&lt;T&gt;
 */
template<typename T>
inline std::basic_string<T> makeSafeFileName(const std::basic_string<T> &sVal) {
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
    str::replace(sRet, "'", "-");

    str::replace(sRet, "...", ".");
    str::replace(sRet, "..", ".");


    str::trim_left(sRet);
    return sRet;
}


/*!
 * \fn  inline std::basic_string<wchar_t> makeSafeFileName(const std::basic_string<wchar_t> &sVal)
 * \brief   Make safe file name.
 * \param   sVal    The value.
 * \return  A std::basic_string&lt;wchar_t&gt;
 */
template<>
inline std::basic_string<wchar_t> makeSafeFileName(const std::basic_string<wchar_t> &sVal) {
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
    str::replace(sRet, L"'", L"-");

    str::replace(sRet, L"...", L".");
    str::replace(sRet, L"..", L".");


    str::trim_left(sRet);
    return sRet;
}


/*!
 * \fn  int inline compareVersions(const std::string& sVer1, const std::string& sVer2, bool& bOk)
 * \brief   Compare versions.
 * \param   sVer1       The first s version.
 * \param   sVer2       The second s version.
 * \param [in,out]  bOk true if sVer1 and sVer2 is valid.
 * \return  return 0 if versions is equal, if sVe2r larger sVer1  return 1 else return -1.
 */
int inline compareVersions(const std::string& sVer1, const std::string& sVer2, bool& bOk, EVerDifference& difference) {
    bOk = true;
    std::vector<int> vecVer1;
    std::vector<int> vecVer2;

    const std::regex pattern("\\d+");

    for (std::sregex_iterator it(sVer1.begin(), sVer1.end(), pattern); it != std::sregex_iterator(); ++it) {
        vecVer1.push_back(str::stringToInt(it->str()));
    }

    for (std::sregex_iterator it(sVer2.begin(), sVer2.end(), pattern); it != std::sregex_iterator(); ++it) {
        vecVer2.push_back(str::stringToInt(it->str()));
    }


    if ((vecVer1.size() != 4) || (vecVer2.size() != 4)) {
        bOk = false;
        return 0;
    }

    difference = eEqual;

    int nComparisonResult = 0;
    for (unsigned int i = 0; i < vecVer1.size(); ++i) {
        int nDiff = vecVer2[i] - vecVer1[i];
        if (nDiff == 0)
            continue;

        difference = (EVerDifference)i;
        if (nDiff > 0) {
            nComparisonResult = 1;
            break;
        }
        else {
            nComparisonResult = -1;
            break;
        }
    }
    return nComparisonResult;
}


/*!
 * \fn  int inline compareVersions(const std::string& sVer1, const std::string& sVer2, bool& bOk)
 * \brief   Compare versions.
 * \param   sVer1       The first s version.
 * \param   sVer2       The second s version.
 * \param [in,out]  bOk true if sVer1 and sVer2 is valid.
 * \return  return 0 if versions is equal, if sVe2r larger sVer1  return 1 else return -1.
 */
int inline compareVersions(const std::string& sVer1, const std::string& sVer2, bool& bOk) {
    EVerDifference difference = eEqual;
    return compareVersions(sVer1, sVer2, bOk, difference);
}


class TemplateBasedReplace {
public:
    /*!
     * \fn  TemplateBasedReplace(const std::unordered_map<std::string, std::string>& template_vars)
     * \brief   Constructor
     * \param   template_vars       Var/value pairs, for example { "VER", "5.2.0" }
     */
    TemplateBasedReplace(const std::unordered_map<std::string, std::string>& template_vars) {
        for (std::unordered_map<std::string, std::string>::const_iterator it = template_vars.cbegin(); it != template_vars.cend(); ++it)
            vars_[(*it).first] = (*it).second;
        //std::copy(vars.begin, template_vars.cbegin(), template_vars.cend());
    }

    /*!
     * \fn  SetTemplateVar(const std::string& var_name, const std::string& value)
     * \brief
     * \param   const std::string& var_name
     * \param   const std::string& value
     */
    void SetTemplateVar(const std::string& var_name, const std::string& value) {
        vars_[var_name] = value;
    }

    /*!
     * \fn  std::string Replace(const std::string& src) const
     * \brief   Processes the template, replacing keys in curly brackets with var values, provided in the constructor
     * \param   const std::string& src      Template, for example "http://www.test.com/j/send_query_{SRC}_{APPID}_{VER}"
     */
    std::string Replace(const std::string& src) const {
        std::string url = src;

        for (std::unordered_map<std::string, std::string>::const_iterator it = vars_.cbegin(); it != vars_.cend(); ++it)
            str::replace(url, std::string("{") + it->first + "}", it->second);

        return std::move(url);
    }

private:
    std::unordered_map<std::string, std::string> vars_;
};

}
#endif //COMMON_STR_PARSE_H
