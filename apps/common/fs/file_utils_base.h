#ifndef COMMON_FILE_UTILS_BASE_H
#define COMMON_FILE_UTILS_BASE_H

#include "../widen.h"
#include "../util/str/tstring.h"
#include "../util/str/str_parse.h"

#if defined(WIN32) || defined(WIN64)
#define PATH_SEPARATOR_A    "\\"
#define PATH_SEPARATOR_T    LL("\\")
#define PATH_SEPARATOR      WIDEN(PATH_SEPARATOR_A)
#else
#define PATH_SEPARATOR      "/"
#define PATH_SEPARATOR_A    PATH_SEPARATOR
#define PATH_SEPARATOR_T    PATH_SEPARATOR
#endif

#if defined(WIN32) || defined(WIN64)
#define tfopen_safe(ppFile, pszFileName, pszMode)       _wfopen_s((ppFile), (pszFileName), (pszMode))
#define tfprintf                                        fwprintf
#else
namespace fs
{

     /*!
     * \fn  inline int fopen_safe(FILE **ppFile, const char *pszFileName, const char *pszMode)
     * \brief   Open file in safe mode.
     * \param [in,out]  ppFile  The file.
     * \param   pszFileName     Filename of the file.
     * \param   pszMode         The mode.
     * \return  An int.
     */
    inline int fopen_safe(FILE **ppFile, const char *pszFileName, const char *pszMode)
    {
        *ppFile = fopen(pszFileName, pszMode);
        if (*ppFile == NULL)
            return 1;
        return 0;
    }
}
#define tfopen_safe(ppFile, pszFileName, pszMode)       fs::fopen_safe((ppFile), (pszFileName), (pszMode))
#define tfprintf                                        fprintf
#define ftello64                                        ftello
#endif

namespace fs
{

 /*!
 * \enum    ESpecialFolder
 * \brief   Values that represent special folders.
 */
enum class ESpecialFolder
{
    eSystem         = 0,
    eDesktop        = 1,
    eApplications,
    eDocuments,
    eDownloads,
    eMovies,
    eMusic,
    ePictures,
    ePublic
};


 /*!
 * \enum    EAppSpecificFolder
 * \brief   Values that represent application specific folders.
 */
enum class EAppSpecificFolder
{
    eApp = 0,
    eResources,
    eConfig,
    eLogs
};


 /*!
 * \fn  inline void concat(tstring& result, const tstring& s2)
 * \brief   Concatenate strings.
 * \param [in,out]  result  The result.
 * \param   s2              The second tstring.
 */
inline void concat(tstring& result, const tstring& s2)
{
    bool bAddSlash = false;

    if (result.size() > 0)
        bAddSlash = (int(result[result.size() - 1]) != '/' && int(result[result.size() - 1]) != int(PATH_SEPARATOR_T[0]));

    const tchar *pszSecondArg = s2.c_str();

    if (!bAddSlash)
    {
        if (*pszSecondArg == LL('/') || *pszSecondArg == PATH_SEPARATOR_T[0])
            ++pszSecondArg;
    }
    else
    {
        if (*pszSecondArg == LL('/') || *pszSecondArg == PATH_SEPARATOR_T[0])
            bAddSlash = false;
    }

    if (bAddSlash)
        result.append(PATH_SEPARATOR_T);

    result.append(pszSecondArg);
}


template<typename T>


 /*!
 * \fn  inline std::basic_string<T> tconcatPaths(const std::basic_string<T>& s1, const std::basic_string<T>& s2)
 * \brief   Concatenate paths.
 * \param   s1  The first std::basic_string&lt;T&gt;
 * \param   s2  The second std::basic_string&lt;T&gt;
 * \return  A std::basic_string&lt;T&gt;
 */
inline std::basic_string<T> tconcatPaths(const std::basic_string<T>& s1, const std::basic_string<T>& s2)
{
    if (s2.empty())
        return s1;

    bool bAddSlash = false;

    if (s1.empty())
        return s2;

    std::basic_string<T> result = s1;

    bAddSlash = (int(result[result.size() - 1]) != '/' && int(result[result.size() - 1]) != int(PATH_SEPARATOR_A[0]));

    const T *pszSecondArg = s2.c_str();

    if (!bAddSlash)
    {
        if (*pszSecondArg == '/' || *pszSecondArg == PATH_SEPARATOR_A[0])
            ++pszSecondArg;
    }
    else
    {
        if (*pszSecondArg == '/' || *pszSecondArg == PATH_SEPARATOR_A[0])
            bAddSlash = false;
    }

    if (bAddSlash)
        result.append(PATH_SEPARATOR_A);

    result.append(pszSecondArg);
    return result;
}


 /*!
 * \fn  inline tstring concatPaths(const tstring& s1, const tstring& s2)
 * \brief   Concatenate paths.
 * \param   s1  The first tstring.
 * \param   s2  The second tstring.
 * \return  A tstring.
 */
inline tstring concatPaths(const tstring& s1, const tstring& s2)
{
    if (s2.empty())
        return s1;

    bool bAddSlash = false;

    if (s1.empty())
        return s2;

    tstring result = s1;

    bAddSlash = (int(result[result.size() - 1]) != '/' && int(result[result.size() - 1]) != int(PATH_SEPARATOR_T[0]));

    const tchar *pszSecondArg = s2.c_str();

    if (!bAddSlash)
    {
        if (*pszSecondArg == LL('/') || *pszSecondArg == PATH_SEPARATOR_T[0])
            ++pszSecondArg;
    }
    else
    {
        if (*pszSecondArg == LL('/') || *pszSecondArg == PATH_SEPARATOR_T[0])
            bAddSlash = false;
    }

    if (bAddSlash)
        result.append(PATH_SEPARATOR_T);

    result.append(pszSecondArg);
    return result;
}

}

#include "file_defs.h"


#endif // COMMON_FILE_UTILS_BASE_H
