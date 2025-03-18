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
    static const char *FreeYouTubeToMP3Converter            = "FreeYouTubeToMP3Converter";
    static const char *FreeVimeoDownload                    = "FreeVimeoDownload";
    static const char *FreeMySpassDownload                  = "FreeMySpassDownload";
    static const char *FreeYouKuDownload                    = "FreeYouKuDownload";
    static const char *FreeDailymotionDownload              = "FreeDailymotionDownload";
    static const char *FreeClipFishDownload                 = "FreeClipFishDownload";
    static const char *FreeTwitchDownload                   = "FreeTwitchDownload";
    static const char *FreeXVideosDownload                  = "FreeXVideosDownload";
    static const char *FreeFacebookVideoDownload            = "FreeFacebookVideoDownload";

    static const char *FreeNetflixDownload                  = "FreeNetflixDownload";
    
    static const char *kLangRegKey                          = "HKEY_CURRENT_USER\\Software\\FreeGrabApp\\CommonSettings";

    static const std::string urlToDownloadApp = "http://download.freegrabapp.com/";
    static const std::string urlToCheckVersion = "http://download.freegrabapp.com/version";


enum class EAppId
{
    eFreeYouTubeDownload            = 0,
    eFreeYouTubeToMP3Converter      = 1,
    eFreeVimeoDownload              = 2,
    eFreeMySpassDownload            = 3,
    eFreeYouKuDownload              = 4,
    eFreeDailymotionDownload        = 5,
    eFreeClipFishDownload           = 6,
    eFreeTwitchDownload             = 7,
    eFreeXVideosDownload            = 8,
    eFreeFacebookVideoDownload      = 9,
    eFreeNetflixDownload            = 10,
    eUnknown                        = -1
};


inline EAppId getAppId(const std::string &sAppName)
{
    if (sAppName.compare(fga::FreeYouTubeDownload) == 0)
        return EAppId::eFreeYouTubeDownload;
    else if (sAppName.compare(fga::FreeYouTubeToMP3Converter) == 0)
        return EAppId::eFreeYouTubeToMP3Converter;
    else if (sAppName.compare(fga::FreeVimeoDownload) == 0)
        return EAppId::eFreeVimeoDownload;
    else if (sAppName.compare(fga::FreeMySpassDownload) == 0)
        return EAppId::eFreeMySpassDownload;
    else if (sAppName.compare(fga::FreeYouKuDownload) == 0)
        return EAppId::eFreeYouKuDownload;
    else if (sAppName.compare(fga::FreeDailymotionDownload) == 0)
        return EAppId::eFreeDailymotionDownload;
    else if (sAppName.compare(fga::FreeClipFishDownload) == 0)
        return EAppId::eFreeClipFishDownload;
    else if (sAppName.compare(fga::FreeTwitchDownload) == 0)
        return EAppId::eFreeTwitchDownload;
    else if (sAppName.compare(fga::FreeXVideosDownload) == 0)
        return EAppId::eFreeXVideosDownload;
    else if (sAppName.compare(fga::FreeFacebookVideoDownload) == 0)
        return EAppId::eFreeFacebookVideoDownload;
    else if (sAppName.compare(fga::FreeNetflixDownload) == 0)
        return EAppId::eFreeNetflixDownload;
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
    case EAppId::eFreeYouTubeToMP3Converter:
        return "FreeYouTubeToMP3Converter";
    case EAppId::eFreeVimeoDownload:
        return "FreeVimeoDownload";
    case EAppId::eFreeMySpassDownload:
        return "FreeMySpassDownload";
    case EAppId::eFreeYouKuDownload:
        return "FreeYouKuDownload";
    case EAppId::eFreeDailymotionDownload:
        return "FreeDailymotionDownload";
    case EAppId::eFreeClipFishDownload:
        return "FreeClipFishDownload";
    case EAppId::eFreeTwitchDownload:
        return "FreeTwitchDownload";
    case EAppId::eFreeXVideosDownload:
        return "FreeXVideosDownload";
    case EAppId::eFreeFacebookVideoDownload:
        return "FreeFacebookVideoDownload";
    case EAppId::eFreeNetflixDownload:
        return "FreeNetflixDownload";
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
    case EAppId::eFreeYouTubeToMP3Converter:
        return "Free YouTube To MP3 Converter";
    case EAppId::eFreeVimeoDownload:
        return "Free Vimeo Download";
    case EAppId::eFreeMySpassDownload:
        return "Free MySpass Download";
    case EAppId::eFreeYouKuDownload:
        return "Free YouKu Download";
    case EAppId::eFreeDailymotionDownload:
        return "Free Dailymotion Download";
    case EAppId::eFreeClipFishDownload:
        return "Free ClipFish Download";
    case EAppId::eFreeTwitchDownload:
        return "Free Twitch Download";
    case EAppId::eFreeXVideosDownload:
        return "Free XVideos Download";
    case EAppId::eFreeFacebookVideoDownload:
        return "Free Facebook Video Download";
    case EAppId::eFreeNetflixDownload:
        return "Free Netflix Download";
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
    case EAppId::eFreeYouTubeToMP3Converter:
        return res_prefix + "freeyoutubetomp3converter.ico";
    case EAppId::eFreeVimeoDownload:
        return res_prefix + "freevimeodownload.ico";
    case EAppId::eFreeMySpassDownload:
        return res_prefix + "freemyspassdownload.ico";
    case EAppId::eFreeYouKuDownload:
        return res_prefix + "freeyoukudownload.ico";
    case EAppId::eFreeDailymotionDownload:
        return res_prefix + "freedailymotiondownload.ico";
    case EAppId::eFreeClipFishDownload:
        return res_prefix + "freeclipfishdownload.ico";
    case EAppId::eFreeTwitchDownload:
        return res_prefix + "freetwitchdownload.ico";
    case EAppId::eFreeXVideosDownload:
        return res_prefix + "freexvideosdownload.ico";
    case EAppId::eFreeFacebookVideoDownload:
        return res_prefix + "freefacebookvideodownload.ico";
    case EAppId::eFreeNetflixDownload:
        return ":icns/icon.ico";
    }

    return "main_icon.ico";
}


//inline std::string getAppTitle(fga::EAppId eAppId, const std::string& sVersion)
//{
//    return fga::getHumanFriendlyAppName(eAppId) + (sVersion.size() ? str::formatStr(" v. %s", sVersion) : "");
//}

} //gba


#endif // APP_DEFS_H
