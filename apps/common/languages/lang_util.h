// lang_util.h

#ifndef LANG_UTIL_H
#define LANG_UTIL_H

#include <string>
#include "../util/str/tstring.h"

namespace lang
{

const char * vCulturesSupported[] = {
    "en-US", "en-GB", "da-DK", "de-DE", "el-GR", "es-ES", "fr-FR", "it-IT", "hu-HU",
    "ja-JP", "nl-NL", "pl-PL", "pt-BR", "pt-PT", "ru-RU", "zh-CN", "zh-TW", "sv-SE", "tr-TR" };


 /*!
 * \fn  inline bool isSupportedCulture(const std::string& sIn)
 * \brief   Query if 'sIn' is supported culture.
 * \param   sIn The in.
 * \return  true if supported culture, false if not.
 */
inline bool isSupportedCulture(const std::string& sIn)
{
    bool bRet = false;
    const size_t nLen = sizeof(vCulturesSupported) / sizeof((vCulturesSupported)[0]);
    for (size_t i = 0 ; i<nLen ; i++ )
    {
        if (sIn.compare(vCulturesSupported[i]) == 0 )
        {
            bRet = true;
            break;
        }
    }
    return bRet;
}

}

#endif // LANG_UTIL_H
