#pragma once


#include "product_info.h"

#define COMPANY_NAME            "FreeGrabApp"
#define COMPANY_DOMAIN          "freegrabapp.com"
#define COPYRIGHT               "Copyright \251 2013-2025 " COMPANY_NAME

#ifdef FLIXGOT_COMPANY
    #define COMPANY_NAME        "FlixGot"
    #define COMPANY_DOMAIN      "flixgot.com"
    #define COPYRIGHT           "Copyright \251 2025 " COMPANY_NAME
#endif

#define COMPANY_URL             "https://" COMPANY_DOMAIN;
#define FAQ_URL                 "https://" COMPANY_DOMAIN "/faq/";
#define BUGREPORT_URL           "https://" COMPANY_DOMAIN "/bug-reporting/";

#define COMPANY_VENDING_URL     "https://vending.freegrabapp.com/Administration/Products"

#define COMPANY_SETTINGS_PATH   "HKEY_CURRENT_USER\\Software\\" COMPANY_NAME

#define LICENSES_STORAGE_PATH   COMPANY_SETTINGS_PATH "\\Licenses"
