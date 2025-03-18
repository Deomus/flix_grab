#pragma once


#include "version.h"

#define STRINGIZE( x )      STR( x )
#define STR( x )            #x

#define INTVER              MAJOR_VERSION,MINOR_VERSION,MICRO_VERSION,BUILD_NUMBER
#define STRVER              STRINGIZE(INTVER)


#define VERSION             MAJOR_VERSION.MINOR_VERSION.MICRO_VERSION.BUILD_NUMBER
#define PRODUCT_VERSION     STRINGIZE(VERSION)

#define PRODUCT_NAME            "NetflixDownloader"
#define FILE_DESCRIPTION        "Netflix Downloader"
#define FILE_NAME               "NetflixDownloader.exe"

#define VENDING_PRODUCT_ID      "100"
#define VENDING_PUBLIC_KEY      "ALkPDQEL0ySOucvqyG1JJBmSAEPtjF6kMDIE3yi9nMMgBhzKhQE="
#define VENDING_PRIVATE_KEY     "AQnN3LkaDHi6xhI="

#define FLIXGOT_COMPANY
