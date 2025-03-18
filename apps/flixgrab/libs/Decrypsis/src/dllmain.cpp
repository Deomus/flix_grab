
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>

#include "decrypsis.h"
#include <windows.h>


void RedirectIOToConsole()
{
    if (AllocConsole()) {

		FILE *s;
		freopen_s(&s, "CONOUT$", "w", stdout);
		freopen_s(&s, "CONOUT$", "w", stderr);

		//Clear the error state for each of the C++ standard stream objects. We need to do this, as
		//attempts to access the standard streams before they refer to a valid target will cause the
		//iostream objects to enter an error state. In versions of Visual Studio after 2005, this seems
		//to always occur during startup regardless of whether anything has been read from or written to
		//the console or not.
		/*std::wcout.clear();
		std::cout.clear();
		std::wcerr.clear();
		std::cerr.clear();
		std::wcin.clear();
		std::cin.clear();

		*/

		// make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog
		// point to console as well
		//std::ios::sync_with_stdio();
    }
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{

    switch (ul_reason_for_call) 
    {
    case DLL_PROCESS_ATTACH: 
        {
                        
            DisableThreadLibraryCalls(hModule);

			//MessageBoxA(NULL, "Run", "Run", 0);
            DWORD size = GetEnvironmentVariableA("DECRYPSIS_CONSOLE_ENABLE", nullptr, 0);

            if (GetLastError() != ERROR_ENVVAR_NOT_FOUND && size > 0)
                RedirectIOToConsole();
                       
//#ifndef NDEBUG
//            RedirectIOToConsole();
//#endif

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
