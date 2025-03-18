
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>
//#include "Injection.h"
#include "phylloxera.h"

#include <windows.h>


void RedirectIOToConsole()
{
    int hConHandle;
    long lStdHandle;
    CONSOLE_SCREEN_BUFFER_INFO coninfo;
    FILE *fp;
   
    if (AllocConsole()) {
        //AttachConsole(GetCurrentProcessId());

        // redirect unbuffered STDOUT to the console
        lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
        hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

        if (hConHandle != -1)
        {
            fp = _fdopen(hConHandle, "w");
            *stdout = *fp;
            setvbuf(stdout, NULL, _IONBF, 0);
        }


        // redirect unbuffered STDIN to the console

        lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
        hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

        if (hConHandle != -1)
        {
            fp = _fdopen(hConHandle, "r");
            *stdin = *fp;
            setvbuf(stdin, NULL, _IONBF, 0);
        }

        // redirect unbuffered STDERR to the console
        lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
        hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

        if (hConHandle != -1)
        {
            fp = _fdopen(hConHandle, "w");
            *stderr = *fp;
            setvbuf(stderr, NULL, _IONBF, 0);
        }

        // make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog
        // point to console as well
        std::ios::sync_with_stdio();
    }
    
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{

    switch (ul_reason_for_call) 
    {
    case DLL_PROCESS_ATTACH: 
        {
                        
            DisableThreadLibraryCalls(hModule);
                       
#ifndef NDEBUG
            RedirectIOToConsole();
#endif

        }
        break;

    case DLL_PROCESS_DETACH:
    {

         //DisableThreadLibraryCalls(hModule);
        // Apply the hook
    }
    break;
    }



    return TRUE;
}
