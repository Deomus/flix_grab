#pragma once


#include "version.h"

#define STRINGIZE( x )      STR( x )
#define STR( x )            #x

#define INTVER              MAJOR_VERSION,MINOR_VERSION,MICRO_VERSION,BUILD_NUMBER
#define STRVER              STRINGIZE(INTVER)


#define VERSION             MAJOR_VERSION.MINOR_VERSION.MICRO_VERSION.BUILD_NUMBER
#define PRODUCT_VERSION     STRINGIZE(VERSION)

#define PRODUCT_NAME        "FreeTwitchDownload"
#define FILE_DESCRIPTION    "Free Twitch Download"
#define FILE_NAME           "FreeTwitchDownload.exe"

#define VENDING_PRODUCT_ID  "89"
#define VENDING_PUBLIC_KEY  "AAcIDQELhAqZekhRAcJTMxOSAHWNSUHpQiRyh/gY/zisNwpVjQA="
#define VENDING_PRIVATE_KEY "AQkJN35vDEDH13s="
