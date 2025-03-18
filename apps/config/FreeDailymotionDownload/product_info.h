#pragma once


#include "version.h"

#define STRINGIZE( x )      STR( x )
#define STR( x )            #x

#define INTVER              MAJOR_VERSION,MINOR_VERSION,MICRO_VERSION,BUILD_NUMBER
#define STRVER              STRINGIZE(INTVER)


#define VERSION             MAJOR_VERSION.MINOR_VERSION.MICRO_VERSION.BUILD_NUMBER
#define PRODUCT_VERSION     STRINGIZE(VERSION)

#define PRODUCT_NAME        "FreeDailymotionDownload"
#define FILE_DESCRIPTION    "Free Dailymotion Download"
#define FILE_NAME           "FreeDailymotionDownload.exe"

#define VENDING_PRODUCT_ID  "91"
#define VENDING_PUBLIC_KEY  "AHcADQELt0UJScCaUT2xywmSAPikBmpzh9qh6HzKOKscrd3HrAE="
#define VENDING_PRIVATE_KEY "AQmJskFhR0JNNF4="
