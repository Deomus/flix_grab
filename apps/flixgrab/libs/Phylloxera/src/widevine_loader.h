#pragma once

#include <BlackBone/Process/Process.h>
#include <BlackBone/localHook/LocalHook.hpp>

#include <iostream>
#include <windows.h>


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

    BOOL hkFreeLibrary( _In_ HMODULE& hLibModule );

    HANDLE hkCreateThread(_In_opt_ LPSECURITY_ATTRIBUTES& lpThreadAttributes,
        _In_ SIZE_T& dwStackSize,
        _In_ LPTHREAD_START_ROUTINE& lpStartAddress,
        _In_opt_ __drv_aliasesMem LPVOID& lpParameter,
        _In_ DWORD& dwCreationFlags,
        _Out_opt_ LPDWORD& lpThreadId
        );


    BOOL hkSetTokenInformation(_In_ HANDLE& TokenHandle,
        _In_ TOKEN_INFORMATION_CLASS& TokenInformationClass,
        _In_reads_bytes_(TokenInformationLength) LPVOID& TokenInformation,
        _In_ DWORD& TokenInformationLength
        );


    BOOL hkSetProcessMitigationPolicy(
        _In_ PROCESS_MITIGATION_POLICY& MitigationPolicy,
        _In_reads_bytes_(dwLength) PVOID& lpBuffer,
        _In_ SIZE_T& dwLength
        );

    VOID hkExitProcess(_In_ UINT& exit_code);

private:
    
    blackbone::Detour<decltype(&::SetTokenInformation), WidevineLoader> detour_set_token_;
    blackbone::Detour<decltype(&::SetProcessMitigationPolicy), WidevineLoader> detour_set_mitigation_;

    blackbone::Detour<decltype(&::LoadLibraryW), WidevineLoader>    detour_load_library_w_;
    blackbone::Detour<decltype(&::FreeLibrary), WidevineLoader>     detour_free_library_;
    blackbone::Detour<decltype(&::CreateThread), WidevineLoader>    detour_create_thread_;

    blackbone::Detour<decltype(&::ExitProcess), WidevineLoader>     detour_exit_process_;
    blackbone::Process                                              current_process_;
};