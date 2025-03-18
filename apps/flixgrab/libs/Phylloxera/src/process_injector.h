#pragma once

#include <BlackBone/Config.h>
#include <BlackBone/Process/Process.h>

#include "BlackBone/PE/PEImage.h"
#include <BlackBone/Misc/Utils.h>
#include <BlackBone/Misc/DynImport.h>
#include <BlackBone/localHook/LocalHook.hpp>


#include <iostream>
#include <windows.h>


using namespace blackbone;


class ProcessInjector
{
public:
   
    static          bool    Install() {
        return Instance()->Hook();
    }

    static          bool    Uninstall() {
        return Instance()->Unhook();
    }
   
private:
    ProcessInjector();

    ~ProcessInjector();


    BOOL hkCreateProcessAsUserW(
        _In_opt_ HANDLE& hToken,
        _In_opt_ LPCWSTR& lpApplicationName,
        _Inout_opt_ LPWSTR& lpCommandLine,
        _In_opt_ LPSECURITY_ATTRIBUTES& lpProcessAttributes,
        _In_opt_ LPSECURITY_ATTRIBUTES& lpThreadAttributes,
        _In_ BOOL& bInheritHandles,
        _In_ DWORD& dwCreationFlags,
        _In_opt_ LPVOID& lpEnvironment,
        _In_opt_ LPCWSTR& lpCurrentDirectory,
        _In_ LPSTARTUPINFOW& lpStartupInfo,
        _Out_ LPPROCESS_INFORMATION& lpProcessInformation
        );

    BOOL hkCreateProcessW(
        _In_opt_ LPCWSTR& lpApplicationName,
        _Inout_opt_ LPWSTR& lpCommandLine,
        _In_opt_ LPSECURITY_ATTRIBUTES& lpProcessAttributes,
        _In_opt_ LPSECURITY_ATTRIBUTES& lpThreadAttributes,
        _In_ BOOL& bInheritHandles,
        _In_ DWORD& dwCreationFlags,
        _In_opt_ LPVOID& lpEnvironment,
        _In_opt_ LPCWSTR& lpCurrentDirectory,
        _In_ LPSTARTUPINFOW& lpStartupInfo,
        _Out_ LPPROCESS_INFORMATION& lpProcessInformation
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


    bool            Hook();

    bool            Unhook();

    NTSTATUS        InjectLoader(DWORD proc_id);
        
    


    static ProcessInjector* Instance();

   
private:
        
    Detour<decltype(&::CreateProcessAsUserW), ProcessInjector> _detCreateProcessAsUserW;
    Detour<decltype(&::CreateProcessW), ProcessInjector> _detCreateProcessW;



    Process                 _current;
      
};