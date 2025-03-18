#pragma once

#include <BlackBone/Config.h>
#include <BlackBone/Process/Process.h>

#include "BlackBone/PE/PEImage.h"
#include <BlackBone/Misc/Utils.h>
#include <BlackBone/Misc/DynImport.h>
#include <BlackBone/localHook/LocalHook.hpp>
#include "stack_walk.h"


#include <iostream>
#include <windows.h>
#include <fstream>

#include <wincrypt.h>

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

    BOOL hkCreateProcessAsUserA(
        _In_opt_ HANDLE& hToken,
        _In_opt_ LPCSTR& lpApplicationName,
        _Inout_opt_ LPSTR& lpCommandLine,
        _In_opt_ LPSECURITY_ATTRIBUTES& lpProcessAttributes,
        _In_opt_ LPSECURITY_ATTRIBUTES& lpThreadAttributes,
        _In_ BOOL& bInheritHandles,
        _In_ DWORD& dwCreationFlags,
        _In_opt_ LPVOID& lpEnvironment,
        _In_opt_ LPCSTR& lpCurrentDirectory,
        _In_ LPSTARTUPINFOA& lpStartupInfo,
        _Out_ LPPROCESS_INFORMATION& lpProcessInformation
    );
    BOOL hkCreateProcessA(
        _In_opt_ LPCSTR& lpApplicationName,
        _Inout_opt_ LPSTR& lpCommandLine,
        _In_opt_ LPSECURITY_ATTRIBUTES& lpProcessAttributes,
        _In_opt_ LPSECURITY_ATTRIBUTES& lpThreadAttributes,
        _In_ BOOL& bInheritHandles,
        _In_ DWORD& dwCreationFlags,
        _In_opt_ LPVOID& lpEnvironment,
        _In_opt_ LPCSTR& lpCurrentDirectory,
        _In_ LPSTARTUPINFOA& lpStartupInfo,
        _Out_ LPPROCESS_INFORMATION& lpProcessInformation
    );


    DWORD hkGetFileAttributesW(IN LPCWSTR& lpFileName
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

    //DWORD hkCertGetNameString(
    //  _In_ PCCERT_CONTEXT& pCertContext,
    //  _In_ DWORD& dwType,
    //  _In_ DWORD& dwFlags,
    //  _In_opt_ PVOID& pvTypePara,
    //  _Out_writes_to_opt_(cchNameString, return) LPSTR& pszNameString,
    //  _In_ DWORD& cchNameString
    //);

    HANDLE
        hkCreateThread(
            _In_opt_ LPSECURITY_ATTRIBUTES& lpThreadAttributes,
            _In_ SIZE_T& dwStackSize,
            _In_ LPTHREAD_START_ROUTINE& lpStartAddress,
            _In_opt_ __drv_aliasesMem LPVOID& lpParameter,
            _In_ DWORD& dwCreationFlags,
            _Out_opt_ LPDWORD& lpThreadId
        );
        /*
        BOOL hkIsDebuggerPresent(VOID);
        WORD hkRtlCaptureStackBackTrace(
                _In_ DWORD& FramesToSkip,
                _In_ DWORD& FramesToCapture,
                _Out_writes_to_(FramesToCapture, return) PVOID*& BackTrace,
                _Out_opt_ PDWORD& BackTraceHash
            );
        HMODULE hkGetModuleHandleW(_In_opt_ LPCWSTR& lpModuleName);
        DWORD hkGetFinalPathNameByHandleW(
                _In_ HANDLE& hFile,
                _Out_writes_(cchFilePath) LPWSTR& lpszFilePath,
                _In_ DWORD& cchFilePath,
                _In_ DWORD& dwFlags
            );
        LPSTR            hkGetCommandLineA( VOID  );
        LPWSTR           hkGetCommandLineW( VOID  );
        */
        DWORD hkGetModuleFileNameW(
            _In_opt_ HMODULE& hModule,
            _Out_writes_to_(nSize, ((return < nSize) ? (return +1) : nSize)) LPWSTR& lpFilename,
            _In_ DWORD& nSize
        );

/*            NTSTATUS hkNtQueryInformationProcess(
                IN HANDLE& ProcessHandle,
                IN PROCESSINFOCLASS& ProcessInformationClass,
                OUT PVOID& ProcessInformation,
                IN ULONG& ProcessInformationLength,
                OUT PULONG& ReturnLength OPTIONAL
            );*/

        
            //LONG  hkQueryDisplayConfig(
            //    _In_ UINT32& flags,
            //    _Inout_ UINT32 *& numPathArrayElements,
            //    _Out_writes_to_(*numPathArrayElements, *numPathArrayElements) DISPLAYCONFIG_PATH_INFO *& pathArray,
            //    _Inout_ UINT32 *& numModeInfoArrayElements,
            //    _Out_writes_to_(*numModeInfoArrayElements, *numModeInfoArrayElements) DISPLAYCONFIG_MODE_INFO *& modeInfoArray,
            //    DISPLAYCONFIG_TOPOLOGY_ID *& currentTopologyId);

            //LONG   hkDisplayConfigGetDeviceInfo(
            //        _Inout_ DISPLAYCONFIG_DEVICE_INFO_HEADER *& requestPacket);

            //BOOL hkEnumDisplayMonitors(
            //        _In_opt_ HDC& hdc,
            //        _In_opt_ LPCRECT& lprcClip,
            //        _In_ MONITORENUMPROC& lpfnEnum,
            //        _In_ LPARAM& dwData);
            //BOOL hkEnumDisplayDevicesA(
            //        _In_opt_ LPCSTR& lpDevice,
            //        _In_ DWORD& iDevNum,
            //        _Inout_ PDISPLAY_DEVICEA& lpDisplayDevice,
            //        _In_ DWORD& dwFlags);
            //
            //BOOL hkEnumDisplayDevicesW(
            //        _In_opt_ LPCWSTR& lpDevice,
            //        _In_ DWORD& iDevNum,
            //        _Inout_ PDISPLAY_DEVICEW& lpDisplayDevice,
            //        _In_ DWORD& dwFlags);
            //HMONITOR hkMonitorFromPoint(
            //        _In_ POINT& pt,
            //        _In_ DWORD& dwFlags);

            //HMONITOR hkMonitorFromRect(
            //    _In_ LPCRECT& lprc,
            //    _In_ DWORD& dwFlags);

            //HMONITOR hkMonitorFromWindow(
            //    _In_ HWND& hwnd,
            //    _In_ DWORD& dwFlags);


    bool            Hook();

    bool            Unhook();

    NTSTATUS        InjectLoader(DWORD proc_id);
        
    


    static ProcessInjector* Instance();

   
private:
    blackbone::Detour<decltype(&::SetTokenInformation), ProcessInjector> detour_set_token_;
    blackbone::Detour<decltype(&::SetProcessMitigationPolicy), ProcessInjector> detour_set_mitigation_;

    Detour<decltype(&::CreateProcessAsUserW), ProcessInjector>      detour_create_process_as_user_w_;
    Detour<decltype(&::CreateProcessAsUserA), ProcessInjector>      detour_create_process_as_user_a_;
    Detour<decltype(&::CreateProcessW), ProcessInjector>            detour_create_process_w_;
    Detour<decltype(&::CreateProcessA), ProcessInjector>            detour_create_process_a_;

    //Detour<decltype(&::CertGetNameStringA), ProcessInjector>        detour_cert_getname_;
//  Detour<decltype(&::CreateThread), ProcessInjector>              detour_cr_thread_;
    //Detour<decltype(&::CryptQueryObject), ProcessInjector>        detour_crypt_obj;
    
    //Detour<decltype(&::IsDebuggerPresent), ProcessInjector>               detour_debugger_pres_;
    //Detour<decltype(&::RtlCaptureStackBackTrace), ProcessInjector>        detour_capture_stack_;
    //Detour<decltype(&::GetModuleHandle), ProcessInjector>             detour_get_handle_;
    //Detour<decltype(&::GetFinalPathNameByHandle), ProcessInjector>        detour_get_pathname_;
    Detour<decltype(&::GetModuleFileNameW), ProcessInjector>            detour_get_module_fn_;
    Detour<decltype(&::GetFileAttributesW), ProcessInjector>            detour_get_file_attribute_fn_;
    //Detour<decltype(&::NtQueryInformationProcess), ProcessInjector>           detour_query_info_proc_;

    //Detour<decltype(&::GetCommandLineA), ProcessInjector>     detour_get_commandlineA_;
    //Detour<decltype(&::GetCommandLineW), ProcessInjector>     detour_get_commandlineW_;

    //Detour<decltype(&::QueryDisplayConfig), ProcessInjector>          detour_QueryDisplayConfig_fn_;
    //Detour<decltype(&::DisplayConfigGetDeviceInfo), ProcessInjector>    detour_DisplayConfigGetDeviceInfo_fn_;
    //Detour<decltype(&::EnumDisplayMonitors), ProcessInjector>           detour_EnumDisplayMonitors_fn_;
    //Detour<decltype(&::EnumDisplayDevicesA), ProcessInjector>           detour_EnumDisplayDevicesA_fn_;
    //Detour<decltype(&::EnumDisplayDevicesW), ProcessInjector>           detour_EnumDisplayDevicesW_fn_;
    //Detour<decltype(&::MonitorFromPoint), ProcessInjector>              detour_MonitorFromPoint_fn_;
    //Detour<decltype(&::MonitorFromRect), ProcessInjector>              detour_MonitorFromRect_fn_;
    //Detour<decltype(&::MonitorFromWindow), ProcessInjector>              detour_MonitorFromWindow_fn_;


    Process                 _current;
    StackWalk               swalk_;

#if FILELOGGER
    std::wofstream file_logger;
#endif

};