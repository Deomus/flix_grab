#pragma once

#include <BlackBone/Process/Process.h>
#include <BlackBone/localHook/LocalHook.hpp>

#include <iostream>
#include <windows.h>

#include <fstream>

class WidevineLoader
{

public:
    bool                    Hook();
    bool                    Unhook();
    
public:
    static WidevineLoader*  Instance();

    /*static bool         Install();
    static bool         Uninstall();
*/
private:
    WidevineLoader();
    ~WidevineLoader();

    HMODULE hkLoadLibraryW(_In_ LPCWSTR& lpLibFileName);
    HMODULE hkLoadLibraryExW(
        _In_ LPCWSTR& lpLibFileName,
        _Reserved_ HANDLE& hFile,
        _In_ DWORD& dwFlags
    );

    BOOL hkFreeLibrary( _In_ HMODULE& hLibModule );

    HANDLE hkCreateThread(_In_opt_ LPSECURITY_ATTRIBUTES& lpThreadAttributes,
        _In_ SIZE_T& dwStackSize,
        _In_ LPTHREAD_START_ROUTINE& lpStartAddress,
        _In_opt_ __drv_aliasesMem LPVOID& lpParameter,
        _In_ DWORD& dwCreationFlags,
        _Out_opt_ LPDWORD& lpThreadId
        );

    FARPROC
        hkGetProcAddress(
            _In_ HMODULE& hModule,
            _In_ LPCSTR& lpProcName
        );

#if _DEBUG
    std::wofstream file_logger;
#endif

private:
        
    blackbone::Detour<decltype(&::LoadLibraryW), WidevineLoader>    detour_load_library_w_;
    blackbone::Detour<decltype(&::LoadLibraryExW), WidevineLoader>  detour_load_library_ex_w_;
    blackbone::Detour<decltype(&::FreeLibrary), WidevineLoader>     detour_free_library_;
    blackbone::Detour<decltype(&::CreateThread), WidevineLoader>    detour_create_thread_;
    blackbone::Detour<decltype(&::GetProcAddress), WidevineLoader>  detour_get_procaddress_;
        
    blackbone::Process                                              current_process_;
};