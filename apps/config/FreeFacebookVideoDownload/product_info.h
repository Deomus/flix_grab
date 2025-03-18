#pragma once


#include "version.h"

#define STRINGIZE( x )      STR( x )
#define STR( x )            #x

#define INTVER              MAJOR_VERSION,MINOR_VERSION,MICRO_VERSION,BUILD_NUMBER
#define STRVER              STRINGIZE(INTVER)


#define VERSION             MAJOR_VERSION.MINOR_VERSION.MICRO_VERSION.BUILD_NUMBER
#define PRODUCT_VERSION     STRINGIZE(VERSION)

#define PRODUCT_NAME        "FreeFacebookVideoDownload"
#define FILE_DESCRIPTION    "Free Facebook Video Download"
#define FILE_NAME           "FreeFacebookVideoDownload.exe"

#define VENDING_PRODUCT_ID  "94"
#define VENDING_PUBLIC_KEY  "AAf/DQELVosizZEfptzUuxWSAGZ1s62m5pUvHjba/ESa7YTIKwA="
#define VENDING_PRIVATE_KEY "AQntK5VobAsseSc="
