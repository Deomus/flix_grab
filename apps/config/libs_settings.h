#pragma once

#include "getflix_settings.h"

#define     DEFAULT_MAXIMUM_CPU_USAGE       (0.85f)
#define     USER_AGENT                      "Mozilla/5.0 (Windows NT 10.0) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/96.0.4664.45 Safari/537.36"
#define     TESSERACTFOLDER                 "tessdata"


#ifndef AP_DLD
    #define AP_DLD 0
#endif
#ifndef AM_DLD
    #define AM_DLD 0
#endif
#ifndef HULU_DLD
    #define HULU_DLD 0
#endif
#ifndef FG_DLD
    #define FG_DLD 0
#endif
#ifndef FGM_DLD
    #define FGM_DLD 0
#endif
#ifndef NF_DLD
    #define NF_DLD 0
#endif
#ifndef ND_DLD
    #define ND_DLD 0
#endif
#ifndef HBO_DLD
    #define HBO_DLD 0
#endif
#ifndef DP_DLD
    #define DP_DLD 0
#endif
#ifndef YTB_DLD
    #define YTB_DLD 0
#endif
#ifndef YTBMP3_CNV
    #define YTBMP3_CNV 0
#endif
#ifndef SPOTIFY_DLD
    #define SPOTIFY_DLD 0
#endif
#ifndef VIMEO_DLD
    #define VIMEO_DLD 0
#endif
#ifndef DAILY_DLD
    #define DAILY_DLD 0
#endif
#ifndef TWITCH_DLD
    #define TWITCH_DLD 0
#endif
#ifndef XVIDEOS_DLD
    #define XVIDEOS_DLD 0
#endif
#ifndef FB_DLD
    #define FB_DLD 0
#endif
#ifndef INST_DLD
    #define INST_DLD 0
#endif
#ifndef PHUB_DLD
    #define PHUB_DLD 0
#endif
#ifndef TWITTER_DLD
    #define TWITTER_DLD 0
#endif

#ifndef NETFLIXPORTALENABLED
    #define NETFLIXPORTALENABLED 0
#endif
#ifndef AMAZONPORTALENABLED
    #define AMAZONPORTALENABLED 0
#endif
#ifndef AMAZONMUSICPORTALENABLED
    #define AMAZONMUSICPORTALENABLED 0
#endif
#ifndef DISNEYPLUSPORTALENABLED
    #define DISNEYPLUSPORTALENABLED 0
#endif
#ifndef HBOPORTALENABLED
    #define HBOPORTALENABLED 0
#endif
#ifndef HULUPORTALENABLED
    #define HULUPORTALENABLED 0
#endif
#ifndef YOUTUBEPORTALENABLED
    #define YOUTUBEPORTALENABLED 0
#endif
#ifndef SPOTIFYPORTALENABLED
    #define SPOTIFYPORTALENABLED 0
#endif
#ifndef PORNHUBPORTALENABLED
    #define PORNHUBPORTALENABLED 0
#endif
#ifndef XVIDEOSPORTALENABLED
    #define XVIDEOSPORTALENABLED 0
#endif
#ifndef TWITCHPORTALENABLED
    #define TWITCHPORTALENABLED 0
#endif
#ifndef INSTAGRAMPORTALENABLED
    #define INSTAGRAMPORTALENABLED 0
#endif
#ifndef FACEBOOKPORTALENABLED
    #define FACEBOOKPORTALENABLED 0
#endif
#ifndef DAILYMOTIONPORTALENABLED
    #define DAILYMOTIONPORTALENABLED 0
#endif
#ifndef VIMEOPORTALENABLED
    #define VIMEOPORTALENABLED 0
#endif
#ifndef TWITTERPORTALENABLED
    #define TWITTERPORTALENABLED 0
#endif
#ifndef SOUNDCLOUDPORTALENABLED
    #define SOUNDCLOUDPORTALENABLED 0
#endif

#ifndef APPLEMUSICPORTALENABLED
#define APPLEMUSICPORTALENABLED 0
#endif

#if NF_DLD == 1 || ND_DLD == 1 || FG_DLD == 1 || FGM_DLD == 1 || HBO_DLD == 1 || DP_DLD == 1 || AP_DLD == 1 || AM_DLD == 1 || HULU_DLD == 1 || SPOTIFY_DLD == 1 || APPLEMUSIC_DLD == 1 
    #define DECRYPTSERVICE 1
#endif
