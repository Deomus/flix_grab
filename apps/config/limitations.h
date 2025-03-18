#pragma once

#include "product_info.h"


inline bool checkDefaultSettingsEnabled() {
    bool enabled = true;

    if (QString(PRODUCT_NAME) == "FlixGrab"
        || QString(PRODUCT_NAME) == "FlixGrabMS"
        || QString(PRODUCT_NAME) == "FreeNetflixDownload"
        || QString(PRODUCT_NAME) == "NetflixDownloader"
        || QString(PRODUCT_NAME) == "VideoDownloaderForNF"
        || QString(PRODUCT_NAME) == "FlixGrabMusic"
        || QString(PRODUCT_NAME) == "FreeAppleMusicDownload"
        || QString(PRODUCT_NAME) == "FreeSpotifyDownload"
        || QString(PRODUCT_NAME) == "FreeAmazonMusicDownload"
        || QString(PRODUCT_NAME) == "FreeYouTubeToMP3Converter"
        || QString(PRODUCT_NAME) == "FreeDisneyPlusDownload"
        || QString(PRODUCT_NAME) == "VideoDownloaderForDisney"
        )
        enabled = false;

    return enabled;
}

inline bool loadingSlowdownEnabled() {
    bool enabled = true;

    if (QString(PRODUCT_NAME) == "FlixGrab"
        || QString(PRODUCT_NAME) == "FlixGrabMS"
        || QString(PRODUCT_NAME) == "FreeNetflixDownload"
        || QString(PRODUCT_NAME) == "NetflixDownloader"
        || QString(PRODUCT_NAME) == "VideoDownloaderForNF"
        || QString(PRODUCT_NAME) == "FlixGrabMusic"
        || QString(PRODUCT_NAME) == "FreeAppleMusicDownload"
        || QString(PRODUCT_NAME) == "FreeSpotifyDownload"
        || QString(PRODUCT_NAME) == "FreeAmazonMusicDownload"
        || QString(PRODUCT_NAME) == "FreeYouTubeToMP3Converter"
        || QString(PRODUCT_NAME) == "FreeDisneyPlusDownload"
        || QString(PRODUCT_NAME) == "VideoDownloaderForDisney"
        )
        enabled = false;
    
    return enabled;
}

inline bool contentLengthLimitEnabled() {
    bool enabled = false;

    if (QString(PRODUCT_NAME) == "FlixGrab"
        || QString(PRODUCT_NAME) == "FlixGrabMS"
        || QString(PRODUCT_NAME) == "FreeNetflixDownload"
        || QString(PRODUCT_NAME) == "NetflixDownloader"
        || QString(PRODUCT_NAME) == "VideoDownloaderForNF"
        || QString(PRODUCT_NAME) == "FlixGrabMusic"
        || QString(PRODUCT_NAME) == "FreeAppleMusicDownload"
        || QString(PRODUCT_NAME) == "FreeSpotifyDownload"
        || QString(PRODUCT_NAME) == "FreeAmazonMusicDownload"
        || QString(PRODUCT_NAME) == "FreeYouTubeToMP3Converter"
        || QString(PRODUCT_NAME) == "FreeDisneyPlusDownload"
        || QString(PRODUCT_NAME) == "VideoDownloaderForDisney"
        )
        enabled = true;
    
    return enabled;
}

inline bool forcedConversionToLowQualityEnabled() {
    bool enabled = false;

    //if (QString(PRODUCT_NAME) == "FlixGrabMusic"
    //    || QString(PRODUCT_NAME) == "FreeSpotifyDownload")
    //    enabled = true;

    return enabled;
}
