// winapi_dyncall.h


#ifndef WINAPI_DYNCALL_H
#define WINAPI_DYNCALL_H

#if defined(WIN32) || defined(WIN64)

#include <windows.h>
#include <shlwapi.h>
#include <shlobj.h>
#include <shellapi.h>

#include <iostream>


inline HRESULT WinApiDynCall_SHGetKnownFolderPath(REFKNOWNFOLDERID rfid,
    DWORD dwFlags,
    HANDLE hToken,
    PWSTR *ppszPath)
{
    HINSTANCE hDLL = LoadLibrary(L"shell32.dll"); // Handle to DLL
    if (hDLL == NULL)
    {
        int err = GetLastError();
        std::cout << "Couldn't load dll | err = " << err << std::endl;
        return E_FAIL;
    }
    typedef HRESULT(__stdcall *EntryPointfuncPtr)(REFKNOWNFOLDERID rfid,
        DWORD dwFlags,
        HANDLE hToken,
        PWSTR *ppszPath);
    EntryPointfuncPtr LibMainEntryPoint;
    LibMainEntryPoint = (EntryPointfuncPtr)GetProcAddress(hDLL, "SHGetKnownFolderPath");
    if (LibMainEntryPoint == NULL)
    {
        int err = GetLastError();
        std::cout << "GetProcAddress return null | err = " << err << std::endl;
        return E_FAIL;
    }
    try
    {
        HRESULT hRes = LibMainEntryPoint(rfid, dwFlags, hToken, ppszPath); 
        //HRESULT hRes = SHGetKnownFolderPath(rfid, dwFlags, hToken, ppszPath);
        return hRes;
    }
    catch (...)
    {
        int err = GetLastError();
        std::cout << "exception | err = " << err << std::endl;
        return E_FAIL;
    }
}


inline void WinApiDynCall_CoTaskMemFree(LPVOID pv)
{
    HINSTANCE hDLL = LoadLibrary(L"ole32.dll"); // Handle to DLL
    if (hDLL == NULL)
    {
        int err = GetLastError();
        std::cout << "Couldn't load dll | err = " << err << std::endl;
        return;
    }
    typedef void(__stdcall *EntryPointfuncPtr)(LPVOID pv);
    EntryPointfuncPtr LibMainEntryPoint;
    LibMainEntryPoint = (EntryPointfuncPtr)GetProcAddress(hDLL, "CoTaskMemFree");
    if (LibMainEntryPoint == NULL)
    {
        int err = GetLastError();
        std::cout << "GetProcAddress return null | err = " << err << std::endl;
        return;
    }
    try
    {
        LibMainEntryPoint(pv); 
        //CoTaskMemFree(pv);
    }
    catch (...)
    {
        int err = GetLastError();
        std::cout << "exception | err = " << err << std::endl;
        return;
    }
}


inline bool WinApiDynCall_SetProcessDPIAware()
{
    HINSTANCE hDLL = LoadLibrary(L"user32.dll"); // Handle to DLL
    if (hDLL == NULL)
    {
        int err = GetLastError();
        std::cout << "Couldn't load dll | err = " << err << std::endl;
        return false;
    }
    typedef BOOL(__stdcall *EntryPointfuncPtr)(VOID);
    EntryPointfuncPtr LibMainEntryPoint;
    LibMainEntryPoint = (EntryPointfuncPtr)GetProcAddress(hDLL, "SetProcessDPIAware");
    if (LibMainEntryPoint == NULL)
    {
        int err = GetLastError();
        std::cout << "GetProcAddress return null | err = " << err << std::endl;
        return false;
    }
    try
    {
        BOOL bRes = LibMainEntryPoint(); // SetProcessDPIAware();
        if (bRes == TRUE)
            return true;
    }
    catch (...)
    {
        int err = GetLastError();
        std::cout << "exception | err = " << err << std::endl;
        return false;
    }
    return false;
}


inline HRESULT WinApiDynCall_SetProcessDpiAwareness()
{
    HINSTANCE hDLL = LoadLibrary(L"Shcore.dll"); // Handle to DLL
    if (hDLL == NULL)
    {
        int err = GetLastError();
        std::cout << "Couldn't load dll | err = " << err << std::endl;
        return E_FAIL;
    }
    typedef HRESULT(__stdcall *EntryPointfuncPtr)(int);
    EntryPointfuncPtr LibMainEntryPoint;
    LibMainEntryPoint = (EntryPointfuncPtr)GetProcAddress(hDLL, "SetProcessDpiAwareness");
    if (LibMainEntryPoint == NULL)
    {
        int err = GetLastError();
        std::cout << "GetProcAddress return null | err = " << err << std::endl;
        return E_FAIL;
    }
    try
    {
        // PROCESS_DPI_UNAWARE              = 0,
        // PROCESS_SYSTEM_DPI_AWARE         = 1,
        // PROCESS_PER_MONITOR_DPI_AWARE    = 2
        return LibMainEntryPoint(1); // SetProcessDpiAwareness();
    }
    catch (...)
    {
        int err = GetLastError();
        std::cout << "exception | err = " << err << std::endl;
        return E_FAIL;
    }
    return E_FAIL;
}



#endif

#endif // WINAPI_DYNCALL_H
