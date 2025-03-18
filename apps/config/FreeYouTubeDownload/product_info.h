#pragma once


#include "version.h"

#define STRINGIZE( x )      STR( x )
#define STR( x )            #x

#define INTVER              MAJOR_VERSION,MINOR_VERSION,MICRO_VERSION,BUILD_NUMBER
#define STRVER              STRINGIZE(INTVER)


#define VERSION             MAJOR_VERSION.MINOR_VERSION.MICRO_VERSION.BUILD_NUMBER
#define PRODUCT_VERSION     STRINGIZE(VERSION)

#define PRODUCT_NAME        "FreeYouTubeDownload"
#define FILE_DESCRIPTION    "Free YouTube Download"
#define FILE_NAME           "FreeYouTubeDownload.exe"

#define VENDING_PRODUCT_ID  "98"
#define VENDING_PUBLIC_KEY  "ABYPDQELEGAIuRIyiXlrYhGSAIuEKdeNUINcwpQEtrhFq6lOlwE="
#define VENDING_PRIVATE_KEY "AQluV26yPpeyo14="
