#pragma once


#include "version.h"

#define STRINGIZE( x )      STR( x )
#define STR( x )            #x

#define INTVER              MAJOR_VERSION,MINOR_VERSION,MICRO_VERSION,BUILD_NUMBER
#define STRVER              STRINGIZE(INTVER)


#define VERSION             MAJOR_VERSION.MINOR_VERSION.MICRO_VERSION.BUILD_NUMBER
#define PRODUCT_VERSION     STRINGIZE(VERSION)

#define PRODUCT_NAME        "FreeYouTubeToMP3Converter"
#define FILE_DESCRIPTION    "Free YouTube to MP3 Converter"
#define FILE_NAME           "FreeYouTubeToMP3Converter.exe"

#define VENDING_PRODUCT_ID  "95"
#define VENDING_PUBLIC_KEY  "APf+DQELd0mRIGPb4ElujAqSACGXuFDMn/u8SihCSJ0ra6AUCgE="
#define VENDING_PRIVATE_KEY "AQlf30iV29Tt0gs="
