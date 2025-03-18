#pragma once


#include "version.h"

#define STRINGIZE( x )          STR( x )
#define STR( x )                #x

#define INTVER                  MAJOR_VERSION,MINOR_VERSION,MICRO_VERSION,BUILD_NUMBER
#define STRVER                  STRINGIZE(INTVER)


#define VERSION                 MAJOR_VERSION.MINOR_VERSION.MICRO_VERSION.BUILD_NUMBER
#define PRODUCT_VERSION         STRINGIZE(VERSION)

#define PRODUCT_NAME            "FreeHBODownload"
#define FILE_DESCRIPTION        "Free HBO Download"
#define FILE_NAME               "FreeHBODownload.exe"

#define VENDING_PRODUCT_ID      "101"
#define VENDING_PUBLIC_KEY      "ALkPDQELcsMxrnDeBCAMhh2SAGBu7tvBAcvFivpFGvrvrHJIHgE="
#define VENDING_PRIVATE_KEY     "AQkRGBfVAVQy+3o="
