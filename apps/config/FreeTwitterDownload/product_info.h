#pragma once


#include "version.h"

#define STRINGIZE( x )      STR( x )
#define STR( x )            #x

#define INTVER              MAJOR_VERSION,MINOR_VERSION,MICRO_VERSION,BUILD_NUMBER
#define STRVER              STRINGIZE(INTVER)


#define VERSION             MAJOR_VERSION.MINOR_VERSION.MICRO_VERSION.BUILD_NUMBER
#define PRODUCT_VERSION     STRINGIZE(VERSION)

#define PRODUCT_NAME        "FreeTwitterDownload"
#define FILE_DESCRIPTION    "Free Twitter Download"
#define FILE_NAME           "FreeTwitterDownload.exe"

#define VENDING_PRODUCT_ID  "88"
#define VENDING_PUBLIC_KEY  "ACcTDQEL0G+325k1wVK99hGSAGhLL84ec+8kV/SlIigkRDAvYQE="
#define VENDING_PRIVATE_KEY "AQlirZR5BweG2Jk="
