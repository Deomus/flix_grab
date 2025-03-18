#pragma once


#include "version.h"

#define STRINGIZE( x )      STR( x )
#define STR( x )            #x

#define INTVER              MAJOR_VERSION,MINOR_VERSION,MICRO_VERSION,BUILD_NUMBER
#define STRVER              STRINGIZE(INTVER)


#define VERSION             MAJOR_VERSION.MINOR_VERSION.MICRO_VERSION.BUILD_NUMBER
#define PRODUCT_VERSION     STRINGIZE(VERSION)

#define PRODUCT_NAME        "FreePornhubDownload"
#define FILE_DESCRIPTION    "Free Pornhub Download"
#define FILE_NAME           "FreePornhubDownload.exe"

#define VENDING_PRODUCT_ID  "90"
#define VENDING_PUBLIC_KEY  "ANcADQEL6qXYdNCY0WlfnRmSAB49cWX7VXMWt9D09o4HM4oyIgA="
#define VENDING_PRIVATE_KEY "AQnC/ud363Fa3Mk="
