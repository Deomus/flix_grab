#pragma once


#include "version.h"

#define STRINGIZE( x )      STR( x )
#define STR( x )            #x

#define INTVER              MAJOR_VERSION,MINOR_VERSION,MICRO_VERSION,BUILD_NUMBER
#define STRVER              STRINGIZE(INTVER)


#define VERSION             MAJOR_VERSION.MINOR_VERSION.MICRO_VERSION.BUILD_NUMBER
#define PRODUCT_VERSION     STRINGIZE(VERSION)

#define PRODUCT_NAME        "FreeVimeoDownload"
#define FILE_DESCRIPTION    "Free Vimeo Download"
#define FILE_NAME           "FreeVimeoDownload.exe"

#define VENDING_PRODUCT_ID  "92"
#define VENDING_PUBLIC_KEY  "ADcADQELG85m866qiJjhCBaSAMVOt2OWzYBBev53kUQKJXHo1QA="
#define VENDING_PRIVATE_KEY "AQmMtZ7DwSYtJ3s="
