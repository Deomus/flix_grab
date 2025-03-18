#pragma once


#include "version.h"

#define STRINGIZE( x )      STR( x )
#define STR( x )            #x

#define INTVER              MAJOR_VERSION,MINOR_VERSION,MICRO_VERSION,BUILD_NUMBER
#define STRVER              STRINGIZE(INTVER)


#define VERSION             MAJOR_VERSION.MINOR_VERSION.MICRO_VERSION.BUILD_NUMBER
#define PRODUCT_VERSION     STRINGIZE(VERSION)

#define PRODUCT_NAME        "FreeAmazonPrimeDownload"
#define FILE_DESCRIPTION    "Free Amazon Prime Download"
#define FILE_NAME           "FreeAmazonPrimeDownload.exe"

#define VENDING_PRODUCT_ID  "87"
#define VENDING_PUBLIC_KEY  "AGj+DQELWm438hodAv9hXAKSAB/XsB6JgaBP/wLsp51Ft8JAoAE="
#define VENDING_PRIVATE_KEY "AQljCDzKa0DNJ70="
