// appfx_defs.h
#pragma once

#ifndef APP_DEFS_H
#define APPFX_DEFS_H

#include <string>

//
#define COMPANYNAME                 "FreeGrabApp Ltd"
#define FILEINFO_PRODUCT_NAME       "FreeGrabApp"


namespace intern
{
    // Client validation key
    static const wchar_t* CLIVK = L"74321F2A-5A55-464E-BFDD-3F5FE3C6A6BA";

    inline std::wstring GetClivk()
    {
        return std::wstring(L"{") + intern::CLIVK + L"}";
    }
}


namespace fga
{
    static const char *kDvsMutexBaseName                    = "Global\\FREEGRABAPP";

    static const char *kCompanyRegistryRoot                 = "Software\\FreeGrabApp\\";
    static const char *kCompanyRegRoot                      = "HKEY_CURRENT_USER\\Software\\FreeGrabApp\\";
    static const char *kLangRegistryKey                     = "Software\\DVDVideoSoft\\FreeGrabApp";
    static const char *kLangRegValueName                    = "CurrentCulture";

    static const char *FreeYouTubeDownload                  = "FreeYouTubeDownload";
    
    static const char *kLangRegKey                          = "HKEY_CURRENT_USER\\Software\\FreeGrabApp\\CommonSettings";


enum class EAppId
{
    eFreeYouTubeDownload            = 0,
    eUnknown                        = -1
};


inline EAppId getAppId(const std::string &sAppName)
{
    if (sAppName.compare(fga::FreeYouTubeDownload) == 0)
        return EAppId::eFreeYouTubeDownload;
    return EAppId::eUnknown;
}


inline std::string getAppName(EAppId nId)
{
    switch (nId)
    {
    case EAppId::eUnknown:
        return "";
    case EAppId::eFreeYouTubeDownload:
        return "FreeYouTubeDownload";
    }
    return "";
}


inline std::string getHumanFriendlyAppName(EAppId nId)
{
    switch (nId)
    {
    case EAppId::eUnknown:
        return "";
    case EAppId::eFreeYouTubeDownload:
        return "Free YouTube Download";
    }

    return "";
}


inline std::string getMainIconAppPath(EAppId nId)
{
    std::string res_prefix = ":icns/";
    switch (nId)
    {
    case EAppId::eUnknown:
        return "main_icon.ico";
    case EAppId::eFreeYouTubeDownload:
        return res_prefix + "freeytvdownloader.ico";
    }

    return "main_icon.ico";
}


//inline std::string getAppTitle(fga::EAppId eAppId, const std::string& sVersion)
//{
//    return fga::getHumanFriendlyAppName(eAppId) + (sVersion.size() ? str::formatStr(" v. %s", sVersion) : "");
//}

} //gba


#endif // APP_DEFS_H
