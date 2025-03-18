#pragma once


#define     NO_PURCHASING               (0)
#define     PADDLE_BACKEND              (1)
#define     WINDOWS_STORE_BACKEND       (2)
#define     VENDING_BACKEND             (3)


#ifdef PURCHASING_BACKEND
    #undef PURCHASING_BACKEND
#endif


//Purchasing subsystem;
//#define     PURCHASING_BACKEND          PADDLE_BACKEND
//#define     PURCHASING_BACKEND          WINDOWS_STORE_BACKEND
#define     PURCHASING_BACKEND          VENDING_BACKEND


//Update subsystem;
#ifndef UPDATE_ENABLED
    #define UPDATE_ENABLED              1
#endif
