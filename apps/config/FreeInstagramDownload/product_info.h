#pragma once


#include "version.h"

#define STRINGIZE( x )      STR( x )
#define STR( x )            #x

#define INTVER              MAJOR_VERSION,MINOR_VERSION,MICRO_VERSION,BUILD_NUMBER
#define STRVER              STRINGIZE(INTVER)


#define VERSION             MAJOR_VERSION.MINOR_VERSION.MICRO_VERSION.BUILD_NUMBER
#define PRODUCT_VERSION     STRINGIZE(VERSION)

#define PRODUCT_NAME        "FreeInstagramDownload"
#define FILE_DESCRIPTION    "Free Instagram Download"
#define FILE_NAME           "FreeInstagramDownload.exe"

#define VENDING_PRODUCT_ID  "93"
#define VENDING_PUBLIC_KEY  "AKf/DQELCm3gctrGPQDcagOSAAucp/jbgyumE/iTEuTicfNqXwE="
#define VENDING_PRIVATE_KEY "AQmEWfLygHERlVU="
