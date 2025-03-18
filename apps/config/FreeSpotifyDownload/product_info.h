#pragma once


#include "version.h"

#define STRINGIZE( x )      STR( x )
#define STR( x )            #x

#define INTVER              MAJOR_VERSION,MINOR_VERSION,MICRO_VERSION,BUILD_NUMBER
#define STRVER              STRINGIZE(INTVER)


#define VERSION             MAJOR_VERSION.MINOR_VERSION.MICRO_VERSION.BUILD_NUMBER
#define PRODUCT_VERSION     STRINGIZE(VERSION)

#define PRODUCT_NAME        "FreeSpotifyDownload"
#define FILE_DESCRIPTION    "Free Spotify Download"
#define FILE_NAME           "FreeSpotifyDownload.exe"

#define VENDING_PRODUCT_ID  "102"
#define VENDING_PUBLIC_KEY  "ALkPDAEKlGWlwMzGui4kYJIAC9UGU4qYqhqy6MWZWvQU5xoBAQ=="
#define VENDING_PRIVATE_KEY "AQktpMnc52c+k9A="
