#ifndef ENC_STR_H
#define ENC_STR_H

#include <string>
#include <algorithm>
#include <regex>

#include "tstring.h"
#include "str_manip.h"
#include "str_parse.h"

//
static const char *reservedChar[]    = {   "%",   "!",   "*",   "'",   "(",   ")",   " ",   ";",   ":",   "@",   "&",   "=",   "+",   "$",   ",",   "/",   "?",   "#",   "[",   "]" };
static const char *escapedChar[]     = { "%25", "%21", "%2A", "%27", "%28", "%29", "%2D", "%3B", "%3A", "%40", "%26", "%3D", "%2B", "%24", "%2C", "%2F", "%3F", "%23", "%5B", "%5D" };

namespace enc
{

 /*!
 * \fn  inline std::string decodeUrl(const std::string& sIn)
 * \brief   Decode URL.
 * \param   sIn The in.
 * \return  A std::string.
 */
inline std::string decodeUrl(const std::string& sIn)
{
    if (sIn.empty())
        return sIn;

    char *pBuf = new char[sIn.size() + 1];
    char *pOut = pBuf;
    std::string sDigits;
    for (unsigned i = 0; i < sIn.size(); i++)
    {
        wchar_t c = sIn[i];
        if (c == '%')
        {
            sDigits = sIn.substr(i + 1, 2);
            if (sDigits == "2B")
            {
                *pOut = ' ';
            }
            else
            {
                long nDigits = strtol(sDigits.c_str(), NULL, 16);
                if (nDigits == 0L)
                {
                    *pOut = (char)c;
                }
                else
                {
                    *pOut = (char)nDigits;
                    i += 2;
                }
            }
        }
        else if (c == '+')
        {
            *pOut = ' ';
        }
        else
        {
            *pOut = (char)c;
        }
        pOut++;
    }
    *pOut = 0;
    std::string ret = pBuf;
    delete [] pBuf;
    return ret;
}


 /*!
 * \fn  inline std::string encodeUrl(const std::string sIn)
 * \brief   Encode URL.
 * \param   sIn The in.
 * \return  A std::string.
 */
inline std::string encodeUrl(const std::string sIn)
{
    std::string result = sIn;
    
    for (int i = 0; i < ARRAYSIZE(reservedChar); i++)
        str::replace(result, reservedChar[i], escapedChar[i]);

    str::replace(result, ".", "%2E");
    return result;
}


 /*!
 * \fn  inline std::string decodeUrlLite(const std::string& sIn)
 * \brief   Decode URL lite.
 * \param   sIn The in.
 * \return  A std::string.
 */
inline std::string decodeUrlLite(const std::string& sIn)
{
    if (sIn.empty())
        return sIn;

    std::string result = sIn;

    for (unsigned i = 0; i < ARRAYSIZE(reservedChar); i++)
    {
        result = std::regex_replace(result,
            std::regex(escapedChar[i], std::regex_constants::icase|std::regex_constants::basic), std::string(reservedChar[i]));
    }
    return std::move(result);
}

}

#endif // ENC_STR_H
