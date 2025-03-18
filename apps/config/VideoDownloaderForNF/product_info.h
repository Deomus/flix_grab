#pragma once


#include "version.h"

#define STRINGIZE( x )      STR( x )
#define STR( x )            #x

#define INTVER              MAJOR_VERSION,MINOR_VERSION,MICRO_VERSION,BUILD_NUMBER
#define STRVER              STRINGIZE(INTVER)


#define VERSION             MAJOR_VERSION.MINOR_VERSION.MICRO_VERSION.BUILD_NUMBER
#define PRODUCT_VERSION     STRINGIZE(VERSION)

#define PRODUCT_NAME        "VideoDownloaderForNF"
#define FILE_DESCRIPTION    "Video Downloader for NF"
#define FILE_NAME           "VideoDownloaderForNF.exe"

//Windows Store product key
#define STORE_PRODUCT_ID    "9P70MWN3B1NK"

