#ifndef COMMON_FILE_UTILS_H
#define COMMON_FILE_UTILS_H

#include <string>
#include <stdio.h>
#include <fstream>
using std::ifstream;
#include <algorithm>

#include "widen.h"
#include "util/str/tstring.h"
#include "util/str/str_manip.h"
#include "util/str/qsutils.h"

#define fseeko64        _fseeki64
#define ftello64        _ftelli64
#define tfopen_safe(ppFile, pszFileName, pszMode)       _wfopen_s((ppFile), (pszFileName), (pszMode))

#define PATH_SEPARATOR_T    LL("\\")

namespace fs {

/*!
 * \enum    EFileOpenMode
 * \brief   Values that represent file open modes.
 */
enum class EFileOpenMode {
    eAnsi = 0,
    eUtf8,
    //eUtf16
};

inline bool fileExists(const tstring& sFileName)
{
    if (sFileName.empty())
        return false;

    return fileExists(sFileName.c_str());
}

inline int readFile(const tstring& path, std::vector<std::string>& vLines) {
    std::ifstream ifs(path);

    if (!ifs.is_open())
        return -1;

    std::string line;
    while (std::getline(ifs, line))
        vLines.emplace_back(line);

    return 0;
}

inline int writeText(const tstring& sFileName, const char *pszValue) {
    int result = 0;
    bool bAlreadyClosed = false;
    std::ofstream *pFile = nullptr;

    try {
        std::ofstream file(sFileName.c_str(), std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);
        pFile = &file;
        file.write(reinterpret_cast<char const*>(pszValue), strlen(pszValue));
        file.close();
        bAlreadyClosed = true;
    }
    catch (...) {
        result = 101;
        if (false == bAlreadyClosed && pFile)
            (*pFile).close();
    }

    return result;
}

inline int freadText(const tstring& sFileName, std::string& sOut) {
    int nRes = 0;

    tstring sMode = LL("rb");
    FILE *pFile;
    errno_t err = tfopen_safe(&pFile, sFileName.c_str(), sMode.c_str());
    if (pFile == NULL || err != 0)
        return 100;
    fseeko64(pFile, 0L, SEEK_END);
    unsigned long long nSize = ftello64(pFile);
    fseeko64(pFile, 0L, SEEK_SET);
    sOut.resize((size_t)nSize + 1);

    fread(const_cast<char*>(sOut.c_str()), (size_t)nSize, 1, pFile);
    sOut[sOut.size() - 1] = 0;
    fclose(pFile);

    return nRes;
}

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

inline tstring getUserAppDataFolder()
{
    char* appdata = getenv("APPDATA");
    return QS2TSTR(CS2Q(appdata));
}

}

#endif // COMMON_FILE_UTILS_H
