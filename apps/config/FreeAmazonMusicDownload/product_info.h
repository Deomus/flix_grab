#pragma once


#include "version.h"

#define STRINGIZE( x )      STR( x )
#define STR( x )            #x

#define INTVER              MAJOR_VERSION,MINOR_VERSION,MICRO_VERSION,BUILD_NUMBER
#define STRVER              STRINGIZE(INTVER)


#define VERSION             MAJOR_VERSION.MINOR_VERSION.MICRO_VERSION.BUILD_NUMBER
#define PRODUCT_VERSION     STRINGIZE(VERSION)

#define PRODUCT_NAME        "FreeAmazonMusicDownload"
#define FILE_DESCRIPTION    "Free Amazon Music Download"
#define FILE_NAME           "FreeAmazonMusicDownload.exe"

#define VENDING_PRODUCT_ID  "103"
#define VENDING_PUBLIC_KEY  "ALkPDQEL6mz8kGYQg2pUJxiSALA4Q00ZM8OQqlrxJADEdOcclQA="
#define VENDING_PRIVATE_KEY "AQmihucOy3FCUoU="
