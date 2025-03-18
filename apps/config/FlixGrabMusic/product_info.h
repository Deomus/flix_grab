#pragma once


#include "version.h"

#define STRINGIZE( x )      STR( x )
#define STR( x )            #x

#define INTVER              MAJOR_VERSION,MINOR_VERSION,MICRO_VERSION,BUILD_NUMBER
#define STRVER              STRINGIZE(INTVER)


#define VERSION             MAJOR_VERSION.MINOR_VERSION.MICRO_VERSION.BUILD_NUMBER
#define PRODUCT_VERSION     STRINGIZE(VERSION)

#define PRODUCT_NAME        "FlixGrabMusic"
#define FILE_DESCRIPTION    "FlixGrabMusic Download"
#define FILE_NAME           "FlixGrabMusic.exe"

#define VENDING_PRODUCT_ID          "104"
#define VENDING_PUBLIC_KEY          "ALkPDQELVeM1ERArNjZ2IgmSAERXe/whGWxsktDctmniORW3CQA="
#define VENDING_PRIVATE_KEY         "AQkxiJN1V+ft0sg="

//Windows Store product key
#define STORE_PRODUCT_ID    "9NN862LD8LKP"

//#define BETA                "Beta"
