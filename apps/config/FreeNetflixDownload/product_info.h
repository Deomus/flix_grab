#pragma once


#include "version.h"

#define STRINGIZE( x )      STR( x )
#define STR( x )            #x

#define INTVER              MAJOR_VERSION,MINOR_VERSION,MICRO_VERSION,BUILD_NUMBER
#define STRVER              STRINGIZE(INTVER)

#define VERSION             MAJOR_VERSION.MINOR_VERSION.MICRO_VERSION.BUILD_NUMBER
#define PRODUCT_VERSION     STRINGIZE(VERSION)


#define PRODUCT_NAME            "FreeNetflixDownload"
#define FILE_DESCRIPTION        "Free Netflix Download"
#define FILE_NAME               "FreeNetflixDownload.exe"

#define VENDING_PRODUCT_ID      "97"
#define VENDING_PUBLIC_KEY      "ADYQDQEL+yqxFaVFvxjaLQaSAFo1wX39bCuTnjLu4+KriDkcEQA="
#define VENDING_PRIVATE_KEY     "AQk9htXnsd9dMyg="

//#define PADDLE_API_KEY          "a7145dc81561fa0d638617949e2d8d8e"
//#define PADDLE_VENDOR_ID        "19321"
//#define PADDLE_PRODUCT_ID       "514362"
