#pragma once


enum EPortal {
    kUnknown = 0,
    kNetflix = 1,
    kAmazon = 2,
    kDisney = 3,
    kHulu = 4,
    kHBO = 5,
    kYoutube = 6,
    kDailymotion = 7,
    kVimeo = 8,
    kTwitch = 9,
    kFacebook = 10,
    kInstagram = 11,
    kTwitter = 12,
    kPornhub = 13,
    kXvideos = 14,
    kAmazonMusic = 15,
    kAppleMusic = 16,
    kSpotify = 17,
    kSoundcloud = 18,
};

inline QString testProxyUrl(const QString& product_name) {

    QString test_proxy_url = "https://www.netflix.com/";

    if (product_name == "FlixGrab"
        || product_name == "FlixGrabMS")
        test_proxy_url = "https://www.netflix.com/";
    else if (product_name == "FreeNetflixDownload"
        || product_name == "VideoDownloaderForNF"
        || product_name == "NetflixDownloader")
        test_proxy_url = "https://www.netflix.com/";
    else if (product_name == "FreeAmazonPrimeDownload"
        || product_name == "FreeAmazonMusicDownload"
        || product_name == "VideoDownloaderforAP")
        test_proxy_url = "https://www.amazon.com/";
    else if (product_name == "FreeDisneyPlusDownload")
        test_proxy_url = "https://disneyplus.com/";
    else if (product_name == "FreeHBODownload")
        test_proxy_url = "https://www.hbomax.com/";
    else if (product_name == "FreeHuluDownload")
        test_proxy_url = "https://www.hulu.com/";
    else if (product_name == "FreeYouTubeDownload")
        test_proxy_url = "https://www.youtube.com/";
    else if (product_name == "FreeYouTubeToMP3Converter")
        test_proxy_url = "https://www.youtube.com/";
    else if (product_name == "FreeFacebookVideoDownload")
        test_proxy_url = "https://facebook.com/";
    else if (product_name == "FreeInstagramDownload")
        test_proxy_url = "https://www.instagram.com/";
    else if (product_name == "FreeDailymotionDownload")
        test_proxy_url = "https://www.dailymotion.com/";
    else if (product_name == "FreeVimeoDownload")
        test_proxy_url = "https://vimeo.com/";
    else if (product_name == "FreeTwitchDownload")
        test_proxy_url = "https://www.twitch.tv/";
    else if (product_name == "FreeTwitterDownload")
        test_proxy_url = "http://www.twitter.com/";
    else if (product_name == "FreePornhubDownload")
        test_proxy_url = "https://pornhub.com/";
    else if (product_name == "FreeXVideosDownload")
        test_proxy_url = "https://www.xvideos.com/";
    else if (product_name == "FreeSoundcloudDownload")
        test_proxy_url = "https://soundcloud.com/";
    else if (product_name == "FreeSpotifyDownload")
        test_proxy_url = "https://open.spotify.com/";
    else if (product_name == "FreeAppleMusicDownload"
        || product_name == "FlixGrabMusic")
        test_proxy_url = "https://music.apple.com/";

    return test_proxy_url;
}

inline QUrl portalVideoGuideUrl(const QString& product_name) {

    QString video_guide_url = "https://freegrabapp.com/youtubechannel/";

    if (product_name == "FlixGrab"
        || product_name == "FlixGrabMS")
        video_guide_url = "https://freegrabapp.com/FlixGrabYT/";
    else if (product_name == "FlixGrabMusic")
        video_guide_url = "https://freegrabapp.com/FlixGrabMusicYT/";
    else if (product_name == "FreeNetflixDownload"
        || product_name == "VideoDownloaderForNF")
        video_guide_url = "https://freegrabapp.com/NetflixYT/";
    else if (product_name == "FreeAmazonPrimeDownload"
        || product_name == "VideoDownloaderforAP")
        video_guide_url = "https://freegrabapp.com/AmazonYT/";
    else if (product_name == "FreeAmazonMusicDownload")
        video_guide_url = "https://freegrabapp.com/AmazonMusicYT/";
    else if (product_name == "FreeDisneyPlusDownload")
        video_guide_url = "https://freegrabapp.com/DisneyYT/";
    else if (product_name == "FreeHBODownload")
        video_guide_url = "https://freegrabapp.com/HBOYT/";
    else if (product_name == "FreeHuluDownload")
        video_guide_url = "https://freegrabapp.com/HuluYT/";
    else if (product_name == "FreeYouTubeDownload")
        video_guide_url = "https://freegrabapp.com/YouTubeYT/";
    else if (product_name == "FreeYouTubeToMP3Converter")
        video_guide_url = "https://freegrabapp.com/YouTubeMP3YT/";
    else if (product_name == "FreeFacebookVideoDownload")
        video_guide_url = "https://freegrabapp.com/FacebookYT/";
    else if (product_name == "FreeInstagramDownload")
        video_guide_url = "https://freegrabapp.com/InstagramYT/";
    else if (product_name == "FreeDailymotionDownload")
        video_guide_url = "https://freegrabapp.com/DailymotionYT/";
    else if (product_name == "FreeVimeoDownload")
        video_guide_url = "https://freegrabapp.com/VimeoYT/";
    else if (product_name == "FreeTwitchDownload")
        video_guide_url = "https://freegrabapp.com/TwitchYT/";
    else if (product_name == "FreeTwitterDownload")
        video_guide_url = "https://freegrabapp.com/TwitterYT/";
    else if (product_name == "FreeXVideosDownload")
        video_guide_url = "https://freegrabapp.com/XVideos/";
    else if (product_name == "FreePornhubDownload")
        video_guide_url = "https://freegrabapp.com/PornHubYT/";
    else if (product_name == "FreeSpotifyDownload")
        video_guide_url = "https://freegrabapp.com/SpotifyYT/";
    else if (product_name == "FreeAppleMusicDownload")
        video_guide_url = "https://freegrabapp.com/AppleYT/";

    return QUrl(video_guide_url);
}

inline void portalName(const EPortal& portal, QString& portal_name) {

    switch (portal) {
        case kNetflix:
            portal_name = "Netflix";
            break;
        case kAppleMusic:
            portal_name = "Apple";
            break;
        case kAmazon:
        case kAmazonMusic:
            portal_name = "Amazon";
            break;
        case kDisney:
            portal_name = "Disney";
            break;
        case kHBO:
            portal_name = "HBO";
            break;
        case kHulu:
            portal_name = "Hulu";
            break;
        case kYoutube:
            portal_name = "YouTube";
            break;
        case kSpotify:
            portal_name = "Spotify";
            break;
        case kPornhub:
            portal_name = "Pornhub";
            break;
        case kXvideos:
            portal_name = "XVideos";
            break;
        case kTwitch:
            portal_name = "Twitch";
            break;
        case kInstagram:
            portal_name = "Instagram";
            break;
        case kFacebook:
            portal_name = "Facebook";
            break;
        case kDailymotion:
            portal_name = "Dailymotion";
            break;
        case kVimeo:
            portal_name = "Vimeo";
            break;
        case kTwitter:
            portal_name = "Twitter";
            break;
        case kSoundcloud:
            portal_name = "Soundcloud";
            break;
    }
}

inline QString portalArtwork(const EPortal& portal) {
    QString artwork;

    switch (portal) {
        case kNetflix:
            artwork = "qrc:///background/background/netflix.png";
            break;
        case kAppleMusic:
            artwork = "qrc:///background/background/apple_music.png";
            break;
        case kAmazon:
            artwork = "qrc:///background/background/amazon.png";
            break;
        case kAmazonMusic:
            artwork = "qrc:///background/background/amazon_music.png";
            break;
        case kDisney:
            artwork = "qrc:///background/background/disney.png";
            break;
        case kHBO:
            artwork = "qrc:///background/background/hbo.png";
            break;
        case kHulu:
            artwork = "qrc:///background/background/hulu.png";
            break;
        case kYoutube:
            artwork = "qrc:///background/background/youtube.png";
            break;
        case kSpotify:
            artwork = "qrc:///background/background/spotify.png";
            break;
        case kPornhub:
            artwork = "qrc:///background/background/pornhub.png";
            break;
        case kXvideos:
            artwork = "qrc:///background/background/xvideos.png";
            break;
        case kTwitch:
            artwork = "qrc:///background/background/twitch.png";
            break;
        case kInstagram:
            artwork = "qrc:///background/background/instagram.png";
            break;
        case kFacebook:
            artwork = "qrc:///background/background/facebook.png";
            break;
        case kDailymotion:
            artwork = "qrc:///background/background/dailymotion.png";
            break;
        case kVimeo:
            artwork = "qrc:///background/background/vimeo.png";
            break;
        case kTwitter:
            artwork = "qrc:///background/background/twitter.png";
            break;
    }

    return artwork;
}

inline bool pauseModeEnabled (const EPortal& portal) {
    bool enabled = false;
    
    switch (portal) {
        case kNetflix:
        case kAppleMusic:
        case kAmazon:
        case kAmazonMusic:
        case kDisney:
        case kHBO:
        case kHulu:
        case kYoutube:
        case kSpotify:
            enabled = true;
            break;
    }

    return enabled;
}

inline bool highContrastImageMaskEnabled(const EPortal& portal) {
    bool enabled = false;

    switch (portal) {
        case kYoutube:
        case kFacebook:
            enabled = true;
            break;
    }

    return enabled;
}

inline bool downloadVideoEnabled(const EPortal& portal, bool product_unlocked = false) {
    bool enabled = false;
    
    if (product_unlocked) {
        enabled = true;
    }
    else {
        if (portal != kInstagram)
            enabled = true;
    }

    return enabled;
}

//inline bool downloadAudioEnabled(const EPortal& portal, bool product_unlocked = false) {
//    bool enabled = true;
//
//    return enabled;
//}
//
//inline bool downloadImageEnabled(const EPortal& portal, bool product_unlocked = false) {
//    bool enabled = true;
//
//    return enabled;
//}

inline int retryLoadPageTimeout(const EPortal& portal) {
    int timeout = 90000;

    switch (portal) {
        case kSpotify:
            timeout = 24000;
            break;
        case kHulu:
            timeout = 30000;
            break;
    }

#ifdef _DEBUG
    timeout = 90000000;
#endif

    return timeout;
}

inline int retryLoadPageOnErrorTimeout(const EPortal& portal) {
    int timeout = 60000;

    switch (portal) {
        case kAmazonMusic:
            timeout = 5000;
            break;
    }

    return timeout;
}


inline QString defaultLoginUrl(const EPortal& portal) {
    QString url;

    switch (portal) {
        case kNetflix:
            url = "https://www.netflix.com/Login";
            break;
        case kAppleMusic:
            url = "https://music.apple.com/login";
            break;
        case kAmazon:
            url = "https://www.primevideo.com/auth-redirect/ref=atv_nb_sign_in?signin=1";
            break;
        case kAmazonMusic:
            url = "https://music.amazon.com/forceSignIn?useHorizonte=true";
            break;
        case kDisney:
            url = "https://www.disneyplus.com/identity/login";
            break;
        case kHBO:
            url = "https://play.hbomax.com/page/urn:hbo:page:home";
            break;
        case kHulu:
            url = "https://auth.hulu.com/web/login";
            break;
        case kYoutube:
            url = "https://accounts.google.com/signin/v2/identifier?service=youtube&uilel=3&passive=true&continue=";
            break;
        case kSpotify:
            url = "https://accounts.spotify.com/login";
            break;
        case kPornhub:
            url = "https://www.pornhubpremium.com/premium/login";
            break;
        case kInstagram:
            url = "https://www.instagram.com/accounts/login/";
            break;
        case kFacebook:
            url = "https://www.facebook.com/login.php?next=http%3A%2F%2Ffacebook.com%2Fhome.php&login_attempt=1";
            break;
    }

    return url;
}

inline QString loginId(const EPortal& portal) {
    QString id;

    switch (portal) {
        case kNetflix:
            id = "Login";
            break;
        case kAppleMusic:
            id = "/login?";
            break;
        case kAmazon:
            id = "signin";
            break;
        case kAmazonMusic:
            id = "signin";
            break;
        case kDisney:
            id = "login";
            break;
        case kHBO:
            id = "https://play.hbomax.com/page/urn:hbo:page:home";
            break;
        case kHulu:
            id = "login";
            break;
        case kSpotify:
            id = "login";
            break;
        //case kYoutube:
        //    id = "signin";
        //    break;
        case kPornhub:
            id = "login";
            break;
        case kInstagram:
            id = "login";
            break;
        //case kFacebook:
        //    id = "login";
        //    break;
    }

    return id;
}

inline QString profileId(const EPortal& portal) {
    QString id = "^";

    switch (portal) {
        case kNetflix:
            id = "browse";
            break;
        case kDisney:
            id = "select-profile";
            break;
        case kHBO:
            id = "profileSelect";
            break;
    }

    return id;
}

inline QString oneTimePasswordId(const EPortal& portal) {
    QString id;

    switch (portal) {
        case kAmazon:
        case kAmazonMusic:
            id = "verify";
            break;
    }

    return id;
}

inline QString cookieLoginRequestedId(const EPortal& portal) {
    QString id;

    switch (portal) {
        case kDisney:
            id = "_scid_r";
            break;
        case kSpotify:
            id = "sss";
            break;
    }

    return id;
}

inline QString cookieLoginPassedId(const EPortal& portal) {
    QString id;

    switch (portal) {
        case kNetflix:
            id = "SecureNetflixId";
            break;
        case kAmazon:
            id = "at-main";
            break;
        case kAmazonMusic:
            id = "at-main";
            break;
        case kDisney:
            id = "ag";
            break;
        case kHBO:
            id = "HBOMAX_LOGIN_REDIRECT";
            break;
        case kHulu:
            id = "_hulu_pid";
            break;
        case kYoutube:
            id = "SID";
            break;
        case kSpotify:
            id = "sp_key";  //?
            break;
        case kPornhub:
            id = "il";
            break;
        case kInstagram:
            id = "sessionid";
            break;
        case kFacebook:
            id = "c_user";
            break;
        case kAppleMusic:
            id = "media-user-token";
            break;
    }

    return id;
}

inline QString cookieProfilePassedId(const EPortal& portal) {
    QString id;

    switch (portal) {
        case kNetflix:
            id = "profilesNewSession";
            break;
        case kDisney:
            id = "ab.storage.userId";
            break;
    }

    return id;
}

inline QString cookieAcceptedId(const EPortal& portal) {
    QString id;

    switch (portal) {
        case kYoutube:
            id = "consent";
            break;
    }

    return id;
}

inline QStringList blockedCookieIds(const EPortal& portal) {
    QStringList cookie;

    switch (portal) {
        case kNetflix:
            cookie = QStringList{ "memclid" };
            break;
        case kPornhub:
            cookie = QStringList{ "ua", "pp-sid", "etaguid", "_ym_uid", "_ym_d", "bs" };
            break;
        case kSoundcloud:
            cookie = QStringList{ "_px3" }; //"sclocale", "_pxvid", "_px3", "_pxff_rf"
            break;
    }

    return cookie;
}

inline QSize signInPageSize(const EPortal& portal) {
    QSize size;

    switch (portal) {
        case kNetflix:
            size = QSize(424, 572);
            break;
        case kAppleMusic:
            size = QSize(800, 560);
            break;
        case kAmazon:
            size = QSize(388, 685);
            break;
        case kAmazonMusic:
            size = QSize(620, 750);
            break;
        case kDisney:
            size = QSize(750, 750);
            break;
        case kHBO:
            size = QSize(1000, 750);
            break;
        case kHulu:
            size = QSize(550, 830);
            break;
        case kYoutube:
            size = QSize(506, 654);
            break;
        case kSpotify:
            size = QSize(530, 750);
            break;
        case kPornhub:
            size = QSize(822, 490);
            break;
        case kInstagram:
            size = QSize(350, 388);
            break;
        case kFacebook:
            size = QSize(630, 538);
            break;
        default:
            size = QSize(630, 538);
            break;
    }

    return size;
}

inline QString subtitlesAdditionalInfo(const EPortal& portal) {
    QString info;

    switch (portal) {
        case kNetflix:
        case kAmazon:
        case kDisney:
        case kHBO:
        case kHulu:
            info = "Forced";
            break;
        case kYoutube:
            info = "Auto";
            break;
    }

    return info;
}

inline QString subtitlesCCInfo(const EPortal& portal) {
    Q_UNUSED(portal);
    QString info = "[CC]";
    
    return info;
}

inline QString subtitlesAdditionalInfoShort(const EPortal& portal) {
    QString info;

    switch (portal) {
        case kNetflix:
        case kAmazon:
        case kDisney:
        case kHBO:
        case kHulu:
            info = "F";
            break;
        case kYoutube:
            info = "A";
            break;
    }

    return info;
}

inline int normalQualityVideoHeight(const EPortal& portal) {
    int height = 720;

    switch (portal) {
        case kUnknown:
            height = 0;
            break;
    }
    
    return height;
}

inline int lowQualityVideoHeight(const EPortal& portal) {
    int height = 360;

    switch (portal) {
        case kUnknown:
            height = 0;
            break;
        case kAmazon:
        case kHulu:
            height = 300;
            break;
    }
    
    return height;
}

inline int normalQualityAudioBitrate(const EPortal& portal) {
    int bitrate = 192;

    switch (portal) {
        case kAppleMusic:
        case kAmazon:
        case kAmazonMusic:
        case kSpotify:
        case kNetflix:
            bitrate = 128;
            break;
        case kDailymotion:
            bitrate = 159;
            break;
    }

    return bitrate;
}

inline int lowQualityAudioBitrate(const EPortal& portal) {
    int bitrate = 96;

    switch (portal) {
        case kSpotify:
            bitrate = 128;
            break;
        case kAppleMusic:
        case kAmazon:
        case kAmazonMusic:
            bitrate = 95;
            break;
        case kYoutube:
            bitrate = 97;
            break;
        case kTwitch:
            bitrate = 97;
            break;
    }

    return bitrate;
}

inline int normalQualityImageWidth(const EPortal& portal) {
    int width = 750;

    switch (portal) {
        case kUnknown:
            width = 0;
            break;
    }
    
    return width;
}

inline int lowQualityImageWidth(const EPortal& portal) {
    int width = 640;

    switch (portal) {
        case kUnknown:
            width = 0;
            break;
    }
    
    return width;
}

inline int durationLimit(const EPortal& portal) { //min
    int duration = 3;

    switch (portal) {
        case kYoutube:
        case kPornhub:
        case kXvideos:
        case kInstagram:
        case kFacebook:
        case kDailymotion:
        case kVimeo:
        case kTwitter:
        case kAppleMusic:
        case kAmazonMusic:
        case kSpotify:
        case kSoundcloud:
            duration = 1;
            break;
    }
    
    return duration;
}

inline float portalSlowdownFactor(const EPortal& portal) {
    float slowdownfactor = 3;

    switch (portal) {
        case kInstagram:
            slowdownfactor = 0;
            break;
    }

    return slowdownfactor;
}

inline bool isVideoHosting(const EPortal& portal) {
    bool ok = false;

    switch (portal) {
        case kYoutube:
        case kPornhub:
        case kXvideos:
        case kTwitch:
        case kFacebook:
        case kDailymotion:
        case kVimeo:
        case kTwitter:
            ok = true;
            break;
    }

    return ok;
}
