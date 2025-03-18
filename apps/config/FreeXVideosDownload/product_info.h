#pragma once


#include "version.h"

#define STRINGIZE( x )      STR( x )
#define STR( x )            #x

#define INTVER              MAJOR_VERSION,MINOR_VERSION,MICRO_VERSION,BUILD_NUMBER
#define STRVER              STRINGIZE(INTVER)


#define VERSION             MAJOR_VERSION.MINOR_VERSION.MICRO_VERSION.BUILD_NUMBER
#define PRODUCT_VERSION     STRINGIZE(VERSION)

#define PRODUCT_NAME        "FreeXVideosDownload"
#define FILE_DESCRIPTION    "Free XVideos Download"
#define FILE_NAME           "FreeXVideosDownload.exe"

#define VENDING_PRODUCT_ID  "96"
#define VENDING_PUBLIC_KEY  "AMf+DQEL2ydPeyKM5MQ45RSSAENCAfRX+43RpxbjW9ICYGttcgA="
#define VENDING_PRIVATE_KEY "AQkc5NQvW/wj75g="
