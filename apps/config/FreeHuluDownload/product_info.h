#pragma once


#include "version.h"

#define STRINGIZE( x )      STR( x )
#define STR( x )            #x

#define INTVER              MAJOR_VERSION,MINOR_VERSION,MICRO_VERSION,BUILD_NUMBER
#define STRVER              STRINGIZE(INTVER)


#define VERSION             MAJOR_VERSION.MINOR_VERSION.MICRO_VERSION.BUILD_NUMBER
#define PRODUCT_VERSION     STRINGIZE(VERSION)

#define PRODUCT_NAME        "FreeHuluDownload"
#define FILE_DESCRIPTION    "Free Hulu Download"
#define FILE_NAME           "FreeHuluDownload.exe"

#define VENDING_PRODUCT_ID  "86"
#define VENDING_PUBLIC_KEY  "AGgHDQELDSugZg5D0cXBsg+SAP0DUyhGLTn3PpTE7D6h/AxyIgE="
#define VENDING_PRIVATE_KEY "AQmi57vUOWNg+X0="
