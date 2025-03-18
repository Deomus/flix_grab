#pragma once


#include "version.h"

#define STRINGIZE( x )      STR( x )
#define STR( x )            #x

#define INTVER              MAJOR_VERSION,MINOR_VERSION,MICRO_VERSION,BUILD_NUMBER
#define STRVER              STRINGIZE(INTVER)


#define VERSION             MAJOR_VERSION.MINOR_VERSION.MICRO_VERSION.BUILD_NUMBER
#define PRODUCT_VERSION     STRINGIZE(VERSION)

#define PRODUCT_NAME        "FreeAppleMusicDownload"
#define FILE_DESCRIPTION    "Free Apple Music Download"
#define FILE_NAME           "FreeAppleMusicDownload.exe"

#define VENDING_PRODUCT_ID  "106"
#define VENDING_PUBLIC_KEY  "EMRUDQELXw2WJtqUoMZMmRWSAIJTFRsyxVU8y9YkMuaiLoK+UQA="
#define VENDING_PRIVATE_KEY "AQnAwB7HVH+C7SU="
