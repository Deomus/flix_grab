// search_helpers.h

#ifndef SEARCH_HELPERS_H
#define SEARCH_HELPERS_H

#include <string>
#include <vector>
#include <list>

#include "str_parse.h"

namespace str {

/*!
 * \fn  inline std::vector<std::string> mkVect(const std::string& str1, const std::string& str2, const char *psz3 = nullptr)
 * \brief   Make vector.
 * \param   str1    The first string.
 * \param   str2    The second string.
 * \param   psz3    The pointer to char.
 * \return  A std::vector&lt;std::string&gt;
 */
inline std::vector<std::string> mkVect(const std::string& str1, const std::string& str2, const char *psz3 = nullptr) {
    std::vector<std::string> result;
    result.emplace_back(str1);
    result.emplace_back(str2);
    if (psz3)
        result.emplace_back(psz3);
    return result;
}



/*!
 * \fn  inline std::string subString(const std::string& src, const char *psz1, const char *psz2, const char *to_what_marker, size_t& cchInitialStartPos)
 * \brief   Get sub string of string.
 * \param   src                         Source string.
 * \param   psz1                        The first strtring.
 * \param   psz2                        The second strtring.
 * \param   to_what_marker              Copy substring to this marker of the end.
 * \param [in,out]  cchInitialStartPos  The initial start position.
 * \return                              Substring.
 */
inline std::string subString(const std::string& src, const char *psz1, const char *psz2, const char *to_what_marker, size_t& cchInitialStartPos) {
    std::string result;

    cchInitialStartPos = src.find(psz1, cchInitialStartPos);
    if (cchInitialStartPos != -1)
        cchInitialStartPos += strlen(psz1);

    if (psz2 != nullptr)
        cchInitialStartPos = src.find(psz2, cchInitialStartPos);

    if (cchInitialStartPos == -1)
        return std::move(result);

    if (psz2 != nullptr)
        cchInitialStartPos += strlen(psz2);

    size_t nEnd = -1;
    if (to_what_marker != nullptr)
        nEnd = src.find(to_what_marker, cchInitialStartPos);

    if (nEnd <= cchInitialStartPos)
        return "";

    return src.substr(cchInitialStartPos, nEnd - cchInitialStartPos);
}


/*!
 * \fn  inline std::string subString(const std::string& src, const char *psz1, const char *psz2, const char *to_what_marker)
 * \brief   Get sub string of string.
 * \param   src         Source string.
 * \param   psz1        The first strtring.
 * \param   psz2        The second strtring.
 * \param   to_what_marker              Copy substring to this marker of the end.
 * \return  Substring.
 */
inline std::string subString(const std::string& src, const char *psz1, const char *psz2, const char *to_what_marker) {
    size_t cchInitialStartPos = 0;
    return subString(src, psz1, psz2, to_what_marker, cchInitialStartPos);
}


/*!
 * \fn  inline std::string subString(const std::string& src, const std::string& find_what, const std::string& to_what_marker, size_t& cchInitialStartPos)
 * \brief   Get sub string of string.
 * \param   src                         Source string.
 * \param   find_what                       The what.
 * \param   to_what_marker                   The till what.
 * \param [in,out]  cchInitialStartPos  The initial start position.
 * \return  A std::string.
 */
inline std::string subString(const std::string& src, const std::string& find_what, const std::string& to_what_marker, size_t& cchInitialStartPos) {
    std::string result;

    size_t nSizePattern = 0;
    if (find_what.size())
        cchInitialStartPos = src.find(find_what, cchInitialStartPos + nSizePattern);

    if (cchInitialStartPos == -1)
        return std::string(result);

    cchInitialStartPos += find_what.size();

    size_t nEnd = -1;
    if (to_what_marker.size())
        nEnd = src.find(to_what_marker, cchInitialStartPos);

    if (nEnd <= cchInitialStartPos)
        return "";

    return src.substr(cchInitialStartPos, nEnd - cchInitialStartPos);
}


/*!
* \fn  inline std::string subString(const std::string& src, const std::string& find_what, const std::string& to_what_marker)
* \brief   Get sub string of string.
* \param   src         Source string.
* \param   find_what       The what.
* \param   to_what_marker   The till what.
* \return  A std::string.
*/
inline std::string subString(const std::string& src, const std::string& find_what, const std::string& to_what_marker) {
    size_t cchInitialStartPos = 0;
    return subString(src, find_what, to_what_marker, cchInitialStartPos);
}


/*!
 * \fn  inline std::string subString(const std::string& src, const std::vector<std::string>& patt, const std::string& to_what_marker, size_t cchInitialStartPos = 0)
 * \brief   Get sub string of string.
 * \param   src                 Source string.
 * \param   patt                Specifies the sequence of patterns.
 * \param   to_what_marker           The till what.
 * \param   cchInitialStartPos  The initial start position.
 * \return  A std::string.
 */
inline std::string subString(const std::string& src, const std::vector<std::string>& patt, const std::string& to_what_marker, size_t cchInitialStartPos = 0) {
    std::string result;
    std::vector<std::string>::const_iterator it = patt.cbegin();
    size_t nPos = cchInitialStartPos;

    if (patt.size()) {
        size_t nSizePattern = 0;
        for (; it != patt.end(); it++) {
            const std::string& find_what = *it;
            nPos = src.find(find_what, nPos + nSizePattern);
            if (nPos == -1)
                break;
            nSizePattern = it->size();
        }

        if (nPos == -1)
            return std::string(result);

        nPos += (--(patt.end()))->size();
    }

    size_t nEnd = -1;
    if (to_what_marker.size())
        nEnd = src.find(to_what_marker, nPos);

    if (nEnd <= nPos)
        return "";

    return src.substr(nPos, nEnd - nPos);
}


/*!
* \fn  inline std::string subString(size_t &nPos, const std::string& src, const std::vector<std::string> &patt, const std::string& to_what_marker)
* \brief   Get sub string of string.
* \param [in,out]  nPos    The start position.
* \param   src             Source string.
* \param   patt            Specifies the sequence of patterns.
* \param   to_what_marker       The till what.
* \return  A std::string.
*/
inline std::string subString(size_t &nPos, const std::string& src, const std::vector<std::string> &patt, const std::string& to_what_marker) {
    std::string result;
    std::vector<std::string>::const_iterator it = patt.cbegin();

    if (patt.size()) {
        for (; it != patt.end(); it++)
            nPos = src.find(*it, nPos);

        if (nPos == -1)
            return std::string(result);

        nPos += (--(patt.end()))->size();
    }

    size_t nEnd = -1;
    if (to_what_marker.size())
        nEnd = src.find(to_what_marker, nPos);

    if (nEnd <= nPos)
        return "";

    return src.substr(nPos, nEnd - nPos);
}


/*!
* \fn  inline std::string subStringI(const std::string& src, const std::vector<std::string> &patt, const std::string& to_what_marker, size_t cchInitialStartPos = 0)
* \brief   Get sub string of string.
* \param   src                 Source string.
* \param   patt                Specifies the sequence of patterns.
* \param   to_what_marker           The till what.
* \param   cchInitialStartPos  The initial start position.
* \return  A std::string.
*/
inline std::string subStringI(const std::string& src, const std::vector<std::string> &patt, const std::string& to_what_marker, size_t cchInitialStartPos = 0) {
    std::string result;
    std::vector<std::string>::const_iterator it = patt.cbegin();
    size_t nPos = cchInitialStartPos;

    if (patt.size()) {
        std::locale loc = std::locale();
        for (size_t i = 0; i < patt.size(); i++) {
            std::string pattern = patt.at(i);
            std::string::const_iterator iter = src.begin() + nPos;
            std::string::const_iterator iter2 = src.end();
            nPos = find_ci(iter, iter2, pattern);
            if (nPos == -1)
                return std::string(result);
        }
        nPos += (--(patt.end()))->size();
    }

    std::string::const_iterator iter = src.begin() + nPos;
    std::string::const_iterator iter2 = src.end();
    size_t nEnd = find_ci(iter, iter2, to_what_marker);
    if (nEnd == -1)
        return std::string(result);
    return src.substr(nPos, nEnd);
}


/*!
* \fn  inline std::string subStringI(const std::string& sIn, const std::string& sSearchString, const std::string& to_what_marker, size_t cchInitialStartPos = 0)
* \brief   Sub string i.
* \param   sIn                 Source string.
* \param   sSearchString       The pattern string.
* \param   to_what_marker           The till what.
* \param   cchInitialStartPos  The initial start position.
* \return  A std::string.
*/
inline std::string subStringI(const std::string& sIn, const std::string& sSearchString, const std::string& to_what_marker, size_t cchInitialStartPos = 0) {
    std::vector<std::string> patt;
    patt.emplace_back(sSearchString);
    return subStringI(sIn, patt, to_what_marker, cchInitialStartPos);
}


/*!
* \fn  inline size_t find(const std::string& src, const std::list<std::string>& lstStrings)
* \brief   Search for the first match.
* \param   src         Source string.
* \param   lstStrings  The list of patterns.
* \return  A position.
*/
inline size_t find(const std::string& src, const std::list<std::string>& lstStrings) {
    if (!lstStrings.size())
        return -1;
    size_t nPos = 0;
    std::list<std::string>::const_iterator it = lstStrings.cbegin();

    for (; it != lstStrings.end(); it++)
        nPos = src.find(it->c_str(), nPos);

    //nPos += (--(lstStrings.end()))->size();

    return nPos;
}


/*!
* \fn  inline bool endsWith(const std::string& src, const std::string& find_what)
* \brief   Check that the string ends with find_what.
* \param   src     Source string.
* \param   find_what   The what.
* \return  true if source string ends with find_what, false in other case.
*/
inline bool endsWith(const std::string& src, const std::string& find_what) {
    if (src.empty() && find_what.empty())
        return true;

    if (src.empty() || find_what.empty())
        return false;

    return src.find(find_what) == src.size() - find_what.size();
}


/*!
* \fn  inline bool contains(const std::string& src, const std::string& find_what)
* \brief   Query if source string contains string find_what.
* \param   src     Source string.
* \param   find_what   The what.
* \return  true if the string contains find_what, false if not.
*/
inline bool contains(const std::string& src, const std::string& find_what) {
    if (find_what.empty())
        return true;
    else if (src.empty())
        return false;
    else
        return src.find(find_what) != -1;
}


/*!
* \fn  inline size_t findEx(const std::string& sIn, const std::vector<std::string> &patt, size_t cchOffset = 0, bool bEndOf = false)
* \brief   Search position.
* \param   sIn         Source string.
* \param   patt        Specifies the sequebce of patterns.
* \param   cchOffset   The offset.
* \param   bEndOf      set true if need return position of charter following the last pattern
*                      charter.
* \return  The found position.
*/
inline size_t findEx(const std::string& sIn, const std::vector<std::string> &patt, size_t cchOffset = 0, bool bEndOf = false) {
    if (sIn.empty() || patt.empty())
        return -1;

    std::vector<std::string>::const_iterator it = patt.cbegin();
    size_t result = cchOffset;

    for (; it != patt.end(); it++) {
        result = sIn.find(*it, result);
        if (result == -1)
            return -1;
    }

    if (bEndOf)
        result += (--(patt.end()))->size();

    return result;
}


/*!
* \fn  inline size_t findEx(const std::wstring& sIn, const std::vector<std::wstring> &patt, size_t cchOffset = 0, bool bEndOf = false)
* \brief   Search position.
* \param   sIn         Source string.
* \param   patt        Specifies the sequebce of patterns.
* \param   cchOffset   The offset.
* \param   bEndOf      set true if need return position of charter following the last pattern
*                      charter.
* \return  The found position.
*/
inline size_t findEx(const std::wstring& sIn, const std::vector<std::wstring> &patt, size_t cchOffset = 0, bool bEndOf = false) {
    if (sIn.empty() || patt.empty())
        return -1;

    std::vector<std::wstring>::const_iterator it = patt.cbegin();
    size_t result = cchOffset;

    for (; it != patt.end(); it++) {
        result = sIn.find(*it, result);
        if (result == -1)
            return -1;
    }

    if (bEndOf)
        result += (--(patt.end()))->size();

    return result;
}

}

#endif //SEARCH_HELPERS_H
