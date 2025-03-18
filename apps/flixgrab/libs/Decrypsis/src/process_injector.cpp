#include "process_injector.h"
#include "decrypsis.h"

#include <BlackBone/Process/RPC/RemoteFunction.hpp>

#include "widevine_loader.h"
#include "utils.h"
#include <list>

ProcessInjector* ProcessInjector::Instance()
{
    static ProcessInjector        hk;
    return &hk;
}

ProcessInjector::ProcessInjector()
{
    _current.Attach(GetCurrentProcessId());
    std::wcout << L"ProcessInjector Constructed!" << std::endl;
}

ProcessInjector::~ProcessInjector()
{
    std::wcout << L"ProcessInjector Destructed!" << std::endl;
}


NTSTATUS ProcessInjector::InjectLoader(DWORD proc_id)
{
    Process child;
#ifdef FILELOGGER 
    std::wofstream file_logger;
    file_logger.open(L"C:\\temp\\ProcessInjector.log", std::ofstream::out | std::ofstream::app | std::ofstream::binary);
    file_logger << std::endl;
    file_logger << L"Injection in process: " << proc_id << " from " << ::_getpid() << ", currentthread: " << GetCurrentThreadId() << std::endl;
    //std::this_thread::sleep_for(std::chrono::seconds(30));
#endif

    std::wcout << L"Injection in process: " << proc_id << " from " << ::_getpid() << ", currentthread: " << GetCurrentThreadId() << std::endl;

    NTSTATUS st = child.AttachSuspended(proc_id);

    if (NT_SUCCESS(st)) {

        const ModuleData* module = _current.modules().GetModule((module_t)GetCurrentModule());

        if (module)
        {
            std::wcout << L"Injection Module: " << module->fullPath << std::endl;
#ifdef FILELOGGER
            file_logger << L"Injection Module: " << module->fullPath << std::endl;
#endif
            module = child.modules().Inject(module->fullPath, &st);
#ifdef FILELOGGER
            file_logger << "Injection Module Status: " << std::hex << st << std::endl;
#endif

            if (NT_SUCCESS(st)) {

                auto proc = child.modules().GetExport(module, "DetourPoint").procAddress;

                //if (proc)
                 //   child.remote().ExecDirect(proc, 0);
                            
                            //RemoteFunction<decltype(&WidevineLoader::Install)>         remote_install(child, &WidevineLoader::Install);
                //RemoteFunction<decltype(&DetourPoint)>         remote_install(child, &DetourPoint);

                if (proc) {
                    RemoteFunction<decltype(&DetourPoint)>         remote_install(child, proc);
                                        
                    bool result = false;

                    st = remote_install.Call(result);

                    std::cout << "Loader Returned Status: " << st << std::endl;
#ifdef FILELOGGER
                    file_logger << "Loader Returned Status: " << std::hex << st << std::endl;
#endif
                }
                else {
                    std::cout << "error: not found DetourPoint" << st << std::endl;
#ifdef FILELOGGER
                    file_logger << "Loader DetourPoint failed " << std::endl;
#endif
                }
            }
        }
        else {
            std::wcout << L"Cant find injection module." << std::endl;
#ifdef FILELOGGER
            file_logger << L"Cant find injection module." << std::endl;
#endif
        }

    }
    else  {
        std::wcout << L"Cant Attach to the process " << proc_id << ", NTSTATUS: " << std::hex << st << std::endl;
#ifdef FILELOGGER
        file_logger << L"Cant Attach to the process " << proc_id << ", NTSTATUS: " << std::hex << st << std::endl;
#endif
    }

    return st;
}

BOOL ProcessInjector::hkCreateProcessAsUserW(_In_opt_ HANDLE& hToken, _In_opt_ LPCWSTR& lpApplicationName, _Inout_opt_ LPWSTR& lpCommandLine, _In_opt_ LPSECURITY_ATTRIBUTES& lpProcessAttributes, _In_opt_ LPSECURITY_ATTRIBUTES& lpThreadAttributes, _In_ BOOL& bInheritHandles, _In_ DWORD& dwCreationFlags, _In_opt_ LPVOID& lpEnvironment, _In_opt_ LPCWSTR& lpCurrentDirectory, _In_ LPSTARTUPINFOW& lpStartupInfo, _Out_ LPPROCESS_INFORMATION& lpProcessInformation)
{
    std::wstring tmp(lpCommandLine);
    //if (lpCommandLine && wcsstr(lpCommandLine, L"QtWeb")) {
    //    std::wstring delimiter = L"--";
    //    auto pos = tmp.find(delimiter);
    //    std::wstring cmd = tmp.substr(0, pos);
    //    std::wstring params = tmp.substr(pos);
    //    tmp = cmd + L" --disable-web-security --disable-features=IsolateOrigins --site-per-process --allow-file-access-from-files --user-data-dir=\"C:/Temp/qtweb\" --disable-gpu "
    //        + params;
    //    //  tmp = L"\"C:\\repo\\builds\\Release\\FlixGrab\\QtWebEngineProcess.exe\" --type=renderer --disable-speech-api --enable-threaded-compositing --enable-features=AllowContentInitiatedDataUrlNavigations,NetworkServiceInProcess,TracingServiceInProcess --disable-features=BackgroundFetch,ConsolidatedMovementXY,DnsOverHttpsUpgrade,InstalledApp,MojoVideoCapture,PictureInPicture,SmsReceiver,UseSkiaRenderer,WebPayments,WebUSB,IsolateOrigins --site-per-process --allow-file-access-from-files --lang=en --webengine-schemes=qrc:sLV --device-scale-factor=1 --num-raster-threads=2 --enable-main-frame-before-activation --renderer-client-id=3 --mojo-platform-channel-handle=2876 /prefetch:1 --disable-web-security --user-data-dir=\"C:\\Temp\"";
    //}

    if (lpCommandLine && wcsstr(lpCommandLine, L"--type=utility"))
    {
        //tmp += L" --disable-web-security --enable-logging --v=1 "; // --single-process --register-pepper-plugins --disable-seccomp-filter-sandbox ";
        // --widevine-path=\"C:\\sources\\My\\FlixGrabAll\\FlixGrab\\3rdparty\\widevinecdm\\\"
        tmp += L" --register-pepper-plugins --single-process  --disable-seccomp-filter-sandbox";
        // tmp += L" --widevine-path=\"C:\\Users\\admin\\AppData\\Local\\Temp\\widevinecdm.dll\" ";

        //lpStartupInfo->dwFlags = STARTF_USESHOWWINDOW;
        //lpStartupInfo->wShowWindow = SW_SHOWDEFAULT; // SW_SHOWDEFAULT;
        ////lpStartupInfo->hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
        ////lpStartupInfo->hStdInput = GetStdHandle(STD_INPUT_HANDLE);
        ////lpStartupInfo->hStdError = GetStdHandle(STD_ERROR_HANDLE);
        //lpStartupInfo->cb = sizeof(STARTUPINFOW);
        //lpStartupInfo->lpDesktop = NULL;
        ////dwCreationFlags |= CREATE_NEW_CONSOLE;
        ////dwCreationFlags &= ~CREATE_NO_WINDOW;
    }
    wchar_t newCmdLine[1000];
    StrCpyW(newCmdLine, tmp.c_str());

    if (lpCommandLine) {
        std::wcout << L"CreateProcessAsUserW: " << newCmdLine << std::endl;
    }


    /*
    dwCreationFlags = dwCreationFlags ^ EXTENDED_STARTUPINFO_PRESENT;
    bInheritHandles = TRUE;
    hToken = NULL;
    lpStartupInfo->cb = sizeof(STARTUPINFOW);

    lpStartupInfo->wShowWindow = SW_SHOWDEFAULT;
    lpStartupInfo->hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    lpStartupInfo->hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    lpStartupInfo->hStdError = GetStdHandle(STD_ERROR_HANDLE);

    lpStartupInfo->lpDesktop = NULL;
    */
    bool bSuspended = true;

    if (dwCreationFlags & EXTENDED_STARTUPINFO_PRESENT)
        dwCreationFlags = dwCreationFlags ^ EXTENDED_STARTUPINFO_PRESENT;

    if (!(dwCreationFlags & CREATE_SUSPENDED)) {
        dwCreationFlags |= CREATE_SUSPENDED;
        bSuspended = false;
    }

//#ifdef FILELOGGER
//    std::wofstream file_logger;
//    file_logger.open(L"C:\\temp\\ProcessInjector.log", std::ofstream::out | std::ofstream::app | std::ofstream::binary);
//    file_logger << std::endl;
//    file_logger << L"CreateProcessAsUserW: " << newCmdLine << std::endl;
//#endif
    BOOL result;
    result = CallOriginal(detour_create_process_as_user_w_, hToken, lpApplicationName, newCmdLine, lpProcessAttributes, lpThreadAttributes,
        bInheritHandles, dwCreationFlags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation);
//#ifdef FILELOGGER
//    file_logger << L" " << result << std::endl;
//#endif
    //--type=utility 
    if (lpCommandLine && result && wcsstr(lpCommandLine, L"--type=utility"))
    {
        //HANDLE DToken = nullptr;
        //#ifdef FILELOGGER
                //if (0 == DuplicateToken(hToken, SecurityImpersonation, &DToken))
                //{
                //    auto err = GetLastError();
                //    file_logger << "DuplicateToken failed: " << err << std::endl;
                //}
                //else
                //    file_logger << "DuplicateToken ok " <<  std::endl;

                //if (DToken != nullptr && 0 == ::SetThreadToken(NULL, DToken))
                //{
                //    auto err = GetLastError();
                //    file_logger << "SetThreadToken failed: " << err << std::endl;
                //}
                //else 
                //    file_logger << "SetThreadToken ok " << std::endl;
        //#endif

        // todo: fix crash:  InjectLoader(lpProcessInformation->dwProcessId);
    }
    if (!bSuspended)
        ResumeThread(lpProcessInformation->hThread);

    return result;
}

BOOL ProcessInjector::hkCreateProcessW(_In_opt_ LPCWSTR& lpApplicationName, _Inout_opt_ LPWSTR& lpCommandLine, _In_opt_ LPSECURITY_ATTRIBUTES& lpProcessAttributes, _In_opt_ LPSECURITY_ATTRIBUTES& lpThreadAttributes, _In_ BOOL& bInheritHandles, _In_ DWORD& dwCreationFlags, _In_opt_ LPVOID& lpEnvironment, _In_opt_ LPCWSTR& lpCurrentDirectory, _In_ LPSTARTUPINFOW& lpStartupInfo, _Out_ LPPROCESS_INFORMATION& lpProcessInformation)
{
    std::wstring tmp(lpCommandLine);
    if (lpCommandLine && wcsstr(lpCommandLine, L"--type=utility")) 
    {
        //tmp += L" --disable-web-security --enable-logging --v=1 "; // --single-process --register-pepper-plugins --disable-seccomp-filter-sandbox ";
        // --widevine-path=\"C:\\sources\\My\\FlixGrabAll\\FlixGrab\\3rdparty\\widevinecdm\\\"
        tmp += L" --register-pepper-plugins --single-process  --disable-seccomp-filter-sandbox";
    }
    wchar_t newCmdLine[1000];
    StrCpyW(newCmdLine, tmp.c_str());
    
    if (lpCommandLine) {
        std::wcout << L"CreateProcessW: " << newCmdLine << std::endl;
#ifdef FILELOGGER
        std::wofstream file_logger;
        file_logger.open(L"C:\\temp\\ProcessInjector.log", std::ofstream::out | std::ofstream::app | std::ofstream::binary);
        file_logger << std::endl;
        file_logger << L"CreateProcessW: " << newCmdLine << std::endl;
#endif
    }

    /* dwCreationFlags = dwCreationFlags ^ EXTENDED_STARTUPINFO_PRESENT;
    bInheritHandles = TRUE;
    lpStartupInfo->cb = sizeof(STARTUPINFOW);

    lpStartupInfo->wShowWindow = SW_SHOWDEFAULT;
    lpStartupInfo->hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    lpStartupInfo->hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    lpStartupInfo->hStdError = GetStdHandle(STD_ERROR_HANDLE);

    lpStartupInfo->lpDesktop = NULL;*/

    bool bSuspended = true;

    if (dwCreationFlags & EXTENDED_STARTUPINFO_PRESENT)
        dwCreationFlags = dwCreationFlags ^ EXTENDED_STARTUPINFO_PRESENT;

    if (!(dwCreationFlags & CREATE_SUSPENDED)) {
        dwCreationFlags |= CREATE_SUSPENDED;
        bSuspended = false;
    }


    /*  bInheritHandles = TRUE;
      lpStartupInfo->cb = sizeof(STARTUPINFOW);

      lpStartupInfo->wShowWindow = SW_SHOWDEFAULT;
      lpStartupInfo->hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
      lpStartupInfo->hStdInput = GetStdHandle(STD_INPUT_HANDLE);
      lpStartupInfo->hStdError = GetStdHandle(STD_ERROR_HANDLE);*/

    BOOL result = CallOriginal(detour_create_process_w_, lpApplicationName, newCmdLine, lpProcessAttributes, lpThreadAttributes,
        bInheritHandles, dwCreationFlags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation);

    /*BOOL result = _detCreateProcessW.CallOriginal(std::move(lpApplicationName), std::move(lpCommandLine), std::move(lpProcessAttributes), std::move(lpThreadAttributes),
        std::move(bInheritHandles), std::move(dwCreationFlags), std::move(lpEnvironment), std::move(lpCurrentDirectory), std::move(lpStartupInfo), std::move(lpProcessInformation));*/

    /*  void* p = &CreateProcessW;

      BOOL result = ::CreateProcessW(lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes,
      bInheritHandles, dwCreationFlags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation);*/
    //--type=utility 
    if (lpCommandLine && wcsstr(lpCommandLine, L"--type=utility") && result)
    {
        InjectLoader(lpProcessInformation->dwProcessId);
    }

    if (!bSuspended)
        ResumeThread(lpProcessInformation->hThread);

    std::wcout << L"CreateProcessW done: " << (result? "OK": "FAIL") << " " << (bSuspended ? "Suspended" : "ResumeThread") << std::endl;

    return result;
}

BOOL ProcessInjector::hkCreateProcessAsUserA(_In_opt_ HANDLE& hToken, _In_opt_ LPCSTR& lpApplicationName, _Inout_opt_ LPSTR& lpCommandLine, _In_opt_ LPSECURITY_ATTRIBUTES& lpProcessAttributes,
    _In_opt_ LPSECURITY_ATTRIBUTES& lpThreadAttributes, _In_ BOOL& bInheritHandles, _In_ DWORD& dwCreationFlags, _In_opt_ LPVOID& lpEnvironment, _In_opt_ LPCSTR& lpCurrentDirectory,
    _In_ LPSTARTUPINFOA& lpStartupInfo, _Out_ LPPROCESS_INFORMATION& lpProcessInformation)
{
    std::wcout << L"CreateProcessAsUserA: " << lpCommandLine << std::endl;

    BOOL result = CallOriginal(detour_create_process_as_user_a_, hToken, lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes,
        bInheritHandles, dwCreationFlags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation);

    return result;
}

BOOL ProcessInjector::hkCreateProcessA(_In_opt_ LPCSTR& lpApplicationName, _Inout_opt_ LPSTR& lpCommandLine, _In_opt_ LPSECURITY_ATTRIBUTES& lpProcessAttributes, _In_opt_ LPSECURITY_ATTRIBUTES& lpThreadAttributes, 
    _In_ BOOL& bInheritHandles, _In_ DWORD& dwCreationFlags, _In_opt_ LPVOID& lpEnvironment, _In_opt_ LPCSTR& lpCurrentDirectory, _In_ LPSTARTUPINFOA& lpStartupInfo, _Out_ LPPROCESS_INFORMATION& lpProcessInformation)
{
    std::wcout << L"CreateProcessA: " << lpCommandLine << std::endl;

    BOOL result = CallOriginal(detour_create_process_a_, lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes,
        bInheritHandles, dwCreationFlags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation);
    return result;
}

DWORD ProcessInjector::hkGetFileAttributesW(IN LPCWSTR& lpFileName)
{
    if (NULL != wcsstr(lpFileName, L"widevinecdm.dll"))
        return FILE_ATTRIBUTE_NORMAL;
    return CallOriginal(detour_get_file_attribute_fn_, lpFileName);
    //return GetFileAttributesW(lpFileName);
}

bool ProcessInjector::Hook()
{
    bool result = true;
    //Inline


    //result &= detour_create_process_as_user_w_.Hook(&::CreateProcessAsUserW, &ProcessInjector::hkCreateProcessAsUserW, this, HookType::Inline, CallOrder::NoOriginal, ReturnMethod::UseNew);
    result &= detour_create_process_w_.Hook(&::CreateProcessW, &ProcessInjector::hkCreateProcessW, this, HookType::Inline, CallOrder::NoOriginal, ReturnMethod::UseNew);
    //result &= detour_create_process_as_user_a_.Hook(&::CreateProcessAsUserA, &ProcessInjector::hkCreateProcessAsUserA, this, HookType::Inline, CallOrder::NoOriginal, ReturnMethod::UseNew);
    //result &= detour_create_process_a_.Hook(&::CreateProcessA, &ProcessInjector::hkCreateProcessA, this, HookType::Inline, CallOrder::NoOriginal, ReturnMethod::UseNew);

//#if VERIFY_CDM_HOST
    //detour_debugger_pres_.Hook(&::IsDebuggerPresent, &ProcessInjector::hkIsDebuggerPresent, this, HookType::Inline, CallOrder::NoOriginal, ReturnMethod::UseNew);
    //detour_capture_stack_.Hook(&::RtlCaptureStackBackTrace, &ProcessInjector::hkRtlCaptureStackBackTrace, this, HookType::Inline, CallOrder::NoOriginal, ReturnMethod::UseNew);
    //detour_get_handle_.Hook(&::GetModuleHandle, &ProcessInjector::hkGetModuleHandleW, this, HookType::Inline, CallOrder::NoOriginal, ReturnMethod::UseNew);
    //detour_get_pathname_.Hook(&::GetFinalPathNameByHandle, &ProcessInjector::hkGetFinalPathNameByHandleW, this, HookType::Inline, CallOrder::NoOriginal, ReturnMethod::UseNew);
    //detour_get_commandlineA_.Hook(&::GetCommandLineA, &ProcessInjector::hkGetCommandLineA, this, HookType::Inline, CallOrder::NoOriginal, ReturnMethod::UseNew);
    //detour_get_commandlineW_.Hook(&::GetCommandLineW, &ProcessInjector::hkGetCommandLineW, this, HookType::Inline, CallOrder::NoOriginal, ReturnMethod::UseNew);
    detour_get_module_fn_.Hook(&::GetModuleFileNameW, &ProcessInjector::hkGetModuleFileNameW, this, HookType::Inline, CallOrder::NoOriginal, ReturnMethod::UseNew);
    detour_get_file_attribute_fn_.Hook(&::GetFileAttributesW, &ProcessInjector::hkGetFileAttributesW, this, HookType::Inline, CallOrder::NoOriginal, ReturnMethod::UseNew);
    //detour_query_info_proc_.Hook(&::NtQueryInformationProcess, &ProcessInjector::hkNtQueryInformationProcess, this, HookType::Inline, CallOrder::NoOriginal, ReturnMethod::UseNew);

    //detour_QueryDisplayConfig_fn_.Hook(&::QueryDisplayConfig, &ProcessInjector::hkQueryDisplayConfig, this, HookType::Inline, CallOrder::NoOriginal, ReturnMethod::UseNew);
    //detour_DisplayConfigGetDeviceInfo_fn_.Hook(&::DisplayConfigGetDeviceInfo, &ProcessInjector::hkDisplayConfigGetDeviceInfo, this, HookType::Inline, CallOrder::NoOriginal, ReturnMethod::UseNew);
    //detour_EnumDisplayMonitors_fn_.Hook(&::EnumDisplayMonitors, &ProcessInjector::hkEnumDisplayMonitors, this, HookType::Inline, CallOrder::NoOriginal, ReturnMethod::UseNew);
    //detour_EnumDisplayDevicesA_fn_.Hook(&::EnumDisplayDevicesA, &ProcessInjector::hkEnumDisplayDevicesA, this, HookType::Inline, CallOrder::NoOriginal, ReturnMethod::UseNew);
    //detour_EnumDisplayDevicesW_fn_.Hook(&::EnumDisplayDevicesW, &ProcessInjector::hkEnumDisplayDevicesW, this, HookType::Inline, CallOrder::NoOriginal, ReturnMethod::UseNew);

    //detour_MonitorFromPoint_fn_.Hook(&::MonitorFromPoint, &ProcessInjector::hkMonitorFromPoint, this, HookType::Inline, CallOrder::NoOriginal, ReturnMethod::UseNew);
    //detour_MonitorFromRect_fn_.Hook(&::MonitorFromRect, &ProcessInjector::hkMonitorFromRect, this, HookType::Inline, CallOrder::NoOriginal, ReturnMethod::UseNew);
    //detour_MonitorFromWindow_fn_.Hook(&::MonitorFromWindow, &ProcessInjector::hkMonitorFromWindow, this, HookType::Inline, CallOrder::NoOriginal, ReturnMethod::UseNew);

    //#endif
    std::wcout << L"Injector Hook: " << result << std::endl;

    return result;
}

bool ProcessInjector::Unhook()
{
    bool result = true;

    result &= detour_create_process_as_user_w_.Restore();
    result &= detour_create_process_w_.Restore();
    //result &= detour_create_process_as_user_a_.Restore();
    //result &= detour_create_process_a_.Restore();

    std::wcout << L"Injector Unhook: " << result << std::endl;

    return result;
}

BOOL ProcessInjector::hkSetTokenInformation(_In_ HANDLE& TokenHandle, _In_ TOKEN_INFORMATION_CLASS& TokenInformationClass, _In_ LPVOID&, _In_ DWORD&)
{
    std::wcout << L"SetTokenInformation : " << TokenInformationClass << std::endl;
    TokenHandle = NULL;
    return TRUE;
}

BOOL ProcessInjector::hkSetProcessMitigationPolicy(_In_ PROCESS_MITIGATION_POLICY& MitigationPolicy, _In_ PVOID&, _In_ SIZE_T&)
{
    std::wcout << L"SetProcessMitigationPolicy : " << MitigationPolicy << std::endl;
    return TRUE;
}

//DWORD ProcessInjector::hkCertGetNameString(
//  _In_ PCCERT_CONTEXT& pCertContext,
//  _In_ DWORD& dwType,
//  _In_ DWORD& dwFlags,
//  _In_opt_ PVOID& pvTypePara,
//  _Out_writes_to_opt_(cchNameString, return) LPSTR& pszNameString,
//  _In_ DWORD& cchNameString
//)
//{
//  std::wcout << "CertGetNameString" << std::endl;
//  return 0;
//}

HANDLE
ProcessInjector::hkCreateThread(
    _In_opt_ LPSECURITY_ATTRIBUTES& lpThreadAttributes,
    _In_ SIZE_T& dwStackSize,
    _In_ LPTHREAD_START_ROUTINE& lpStartAddress,
    _In_opt_ __drv_aliasesMem LPVOID& lpParameter,
    _In_ DWORD& dwCreationFlags,
    _Out_opt_ LPDWORD& lpThreadId
) {
    std::wcout << "hkCreateThread..." << std::endl;
    return 0;
}


//BOOL ProcessInjector::hkIsDebuggerPresent(VOID) 
//{
//  std::wcout << "IsDebuggerPresent..." << std::endl;
//  return false;
//}
//WORD ProcessInjector::hkRtlCaptureStackBackTrace(
//  _In_ DWORD& FramesToSkip,
//  _In_ DWORD& FramesToCapture,
//  _Out_writes_to_(FramesToCapture, return) PVOID*& BackTrace,
//  _Out_opt_ PDWORD& BackTraceHash
//) {
//    auto cnt = CallOriginal(detour_capture_stack_, FramesToSkip, FramesToCapture, BackTrace, BackTraceHash);
//    std::wcout << "RtlCaptureStackBackTrace" << std::endl;
//    
//    //auto frames = swalk_.GetCallStack(BackTrace, cnt);
//    //bool inWvcdm = true;
//    //if (inWvcdm) {
//    //    int j = 0;
//    //    //std::wcout << "RtlCaptureStackBackTrace =" << cnt << ": " << FramesToSkip << " > " << (int)(FramesToCapture) << " > " << BackTrace[0] << " ..." << std::endl;
//    //    for (std::list<StackWalk::StackFrame>::iterator it = frames.begin(); it != frames.end(); it++, j++)
//    //    {
//    //        StackWalk::StackFrame frame = *it;
//    //        const auto& name = frame.GetModuleName();
//    //        if(name.find_first_of(L"Decrypsis") == std::wstring::npos && 
//    //            name.find_first_of(L"Qt5") == std::wstring::npos)
//    //        std::wcout << frame.GetModuleName() << "!0x" << std::hex << frame.GetOffset() << " (0x" << frame.GetAddress() << ")" << std::dec<< std::endl;
//    //    }
//    //}
//  return cnt;
//}

//HMODULE ProcessInjector::hkGetModuleHandleW(_In_opt_ LPCWSTR& lpModuleName)
//{
//    //const auto mm = L"C:\\Program Files (x86)\\Google\\Chrome\\Application\\79.0.3945.130\\chrome.exe";
//    
//    auto result = CallOriginal(detour_get_handle_, lpModuleName);
//    //std::wcout << "GetModuleHandle =" << result << " " << (lpModuleName == NULL? L"NULL": lpModuleName) << std::endl;
//    return result;
//}
//
//DWORD ProcessInjector::hkGetFinalPathNameByHandleW(
//    _In_ HANDLE& hFile,
//    _Out_writes_(cchFilePath) LPWSTR& lpszFilePath,
//    _In_ DWORD& cchFilePath,
//    _In_ DWORD& dwFlags
//)
//{
//    swalk_.GetModuleList();
//    auto result = CallOriginal(detour_get_pathname_, hFile, lpszFilePath, cchFilePath, dwFlags);
//    //std::wcout << "GetFinalPathNameByHandle =" << result << " (" << hFile << "), flags=" << dwFlags << ", " << lpszFilePath << std::endl;
//    return result;
//}

std::wstring dirnameOf(const std::wstring& fname)
{
    size_t pos = fname.find_last_of(L"\\/");
    return (std::wstring::npos == pos)
        ? L""
        : fname.substr(0, pos);
}

DWORD ProcessInjector::hkGetModuleFileNameW(
    _In_opt_ HMODULE& hModule,
    _Out_writes_to_(nSize, ((return < nSize) ? (return +1) : nSize)) LPWSTR& lpFilename,
    _In_ DWORD& nSize
)
{
    auto result = CallOriginal(detour_get_module_fn_, hModule, lpFilename, nSize);
    if (result < nSize
        && nSize != MAX_PATH + 85) // 85 is marker from VerifyCdmHost() to bypass the hook in this call
    {
        wchar_t* pch = NULL;
        if (NULL != (pch = wcsstr(lpFilename, L"Decrypsis.dll")) ||
            NULL != (pch = wcsstr(lpFilename, L"Decrypsis_d.dll")) ||
            NULL != (pch = wcsstr(lpFilename, L"FlixGrab.exe")) ||
            NULL != (pch = wcsstr(lpFilename, L"FlixGrab_d.exe")) ||
            NULL != (pch = wcsstr(lpFilename, L"FlixGrabMusic.exe")) ||
            NULL != (pch = wcsstr(lpFilename, L"FlixGrabMusic_d.exe")) ||
            NULL != (pch = wcsstr(lpFilename, L"FlixGrabMS.exe")) ||
            NULL != (pch = wcsstr(lpFilename, L"FlixGrabMS_d.exe")) ||
            NULL != (pch = wcsstr(lpFilename, L"FreeNetflixDownload.exe")) ||
            NULL != (pch = wcsstr(lpFilename, L"FreeNetflixDownload_d.exe")) ||
            NULL != (pch = wcsstr(lpFilename, L"VideoDownloaderForNF.exe")) ||
            NULL != (pch = wcsstr(lpFilename, L"VideoDownloaderForNF_d.exe")) ||
            NULL != (pch = wcsstr(lpFilename, L"NetflixDownloader.exe")) ||
            NULL != (pch = wcsstr(lpFilename, L"NetflixDownloader_d.exe")) ||
            NULL != (pch = wcsstr(lpFilename, L"FreeAmazonPrimeDownload.exe")) ||
            NULL != (pch = wcsstr(lpFilename, L"FreeAmazonPrimeDownload_d.exe")) ||
            NULL != (pch = wcsstr(lpFilename, L"VideoDownloaderForAP.exe")) ||
            NULL != (pch = wcsstr(lpFilename, L"VideoDownloaderForAP_d.exe")) ||
            NULL != (pch = wcsstr(lpFilename, L"FreeAmazonMusicDownload.exe")) ||
            NULL != (pch = wcsstr(lpFilename, L"FreeAmazonMusicDownload_d.exe")) ||
            NULL != (pch = wcsstr(lpFilename, L"FreeAppleMusicDownload.exe")) ||
            NULL != (pch = wcsstr(lpFilename, L"FreeAppleMusicDownload_d.exe")) ||
            NULL != (pch = wcsstr(lpFilename, L"FreeDisneyPlusDownload.exe")) ||
            NULL != (pch = wcsstr(lpFilename, L"FreeDisneyPlusDownload_d.exe")) ||
            NULL != (pch = wcsstr(lpFilename, L"FreeHBODownload.exe")) ||
            NULL != (pch = wcsstr(lpFilename, L"FreeHBODownload_d.exe")) ||
            NULL != (pch = wcsstr(lpFilename, L"FreeSpotifyDownload.exe")) ||
            NULL != (pch = wcsstr(lpFilename, L"FreeSpotifyDownload_d.exe")) ||
            NULL != (pch = wcsstr(lpFilename, L"FreeHuluDownload.exe")) ||
            NULL != (pch = wcsstr(lpFilename, L"FreeHuluDownload_d.exe"))) {
            std::wcout << "GetModuleFileNameW hooked! Original: " << lpFilename << std::endl;

            // case if chrome files are side-by-side with main module: wcsncpy_s(pch, 12, L"chrome.exe\0\0", 12);
            // case if chrome.exe was extracted to temp folder in VerifyCdmHost():

          auto tmpsize = GetTempPath(nSize, lpFilename);
//            result = tmpsize + 10; // size of chrome.exe
            std::wstring chrome = L"\\chrome.exe\0\0";
            std::wstring str = dirnameOf(lpFilename);
            result = str.length();

            if (result <= nSize) // if we fit into the buffer
            {
                wcsncpy_s(&lpFilename[result], nSize - result, chrome.c_str(), chrome.length() + 1);
                result += chrome.length() + 1;
            }
            std::wcout << "Hooked: " << lpFilename << std::endl;
            return result;
        }

        pch = NULL;
/*        //if (NULL != (pch = wcsstr(lpFilename, L"widevinecdm.dll"))) 
        if (NULL != (pch = wcsstr(lpFilename, L"wirafreemod.dll"))) //widevinecdm_mod
        {
            std::wcout << "GetModuleFileNameW hooked! Original: " << lpFilename << std::endl;
            auto tmpsize = GetTempPath(nSize, lpFilename);
            std::wstring widevinecdm = L"\\widevinecdm.dll\0\0";
            std::wstring str = dirnameOf(lpFilename);
            result = str.length();
            if (result <= nSize) // if we fit into the buffer
            {
                wcsncpy_s(&lpFilename[result], nSize - result, widevinecdm.c_str(), widevinecdm.length());
                result += widevinecdm.length() + 1;
            }
#if _DEBUG
            std::wcout << "Hooked: " << lpFilename << std::endl;
#endif
            return result;
        }*/
    }
    //std::wcout << "GetModuleFileNameW =" << result << " (" << hModule << "), " << lpFilename << ", " << nSize << std::endl;
    return result;
}
//
//LONG ProcessInjector::hkQueryDisplayConfig(
//    _In_ UINT32& flags,
//    _Inout_ UINT32*& numPathArrayElements,
//    _Out_writes_to_(*numPathArrayElements, *numPathArrayElements) DISPLAYCONFIG_PATH_INFO*& pathArray,
//    _Inout_ UINT32*& numModeInfoArrayElements,
//    _Out_writes_to_(*numModeInfoArrayElements, *numModeInfoArrayElements) DISPLAYCONFIG_MODE_INFO*& modeInfoArray,
//    DISPLAYCONFIG_TOPOLOGY_ID*& currentTopologyId)
//{
//    auto result = CallOriginal(detour_QueryDisplayConfig_fn_, flags, numPathArrayElements, pathArray, numModeInfoArrayElements, modeInfoArray, currentTopologyId);
//    std::wcout << "hook QueryDisplayConfig: " << flags << ", result: " << result << std::endl;
//    return result;
//}
//
//LONG ProcessInjector::hkDisplayConfigGetDeviceInfo(
//    _Inout_ DISPLAYCONFIG_DEVICE_INFO_HEADER*& requestPacket)
//{
//    auto result = CallOriginal(detour_DisplayConfigGetDeviceInfo_fn_, requestPacket);
//    std::wcout << "hook DisplayConfigGetDeviceInfo: " << result << std::endl;
//    return result;
//}
//
//BOOL ProcessInjector::hkEnumDisplayMonitors(
//    _In_opt_ HDC& hdc,
//    _In_opt_ LPCRECT& lprcClip,
//    _In_ MONITORENUMPROC& lpfnEnum,
//    _In_ LPARAM& dwData)
//{
//    auto result = CallOriginal(detour_EnumDisplayMonitors_fn_, hdc, lprcClip, lpfnEnum, dwData);
//    std::wcout << "hook EnumDisplayMonitors: " << result << ", hdc: " << (long long) hdc << ", lprcClip:" << (lprcClip != NULL? lprcClip->left: -1) << "x" << (lprcClip != NULL ? lprcClip->top : -1) << ", lpfnEnum:" << (long long)lpfnEnum << std::endl;
//    return result;
//}
//
//BOOL ProcessInjector::hkEnumDisplayDevicesA(
//    _In_opt_ LPCSTR& lpDevice,
//    _In_ DWORD& iDevNum,
//    _Inout_ PDISPLAY_DEVICEA& lpDisplayDevice,
//    _In_ DWORD& dwFlags) {
//    auto result = CallOriginal(detour_EnumDisplayDevicesA_fn_, lpDevice, iDevNum, lpDisplayDevice, dwFlags);
//    std::wcout << "hook EnumDisplayDevicesA: " << result << ", lpDevice: " << lpDevice << ", iDevNum: " << iDevNum << std::endl;
//    return result;
//}
//
//BOOL ProcessInjector::hkEnumDisplayDevicesW(
//    _In_opt_ LPCWSTR& lpDevice,
//    _In_ DWORD& iDevNum,
//    _Inout_ PDISPLAY_DEVICEW& lpDisplayDevice,
//    _In_ DWORD& dwFlags) {
//    auto result = CallOriginal(detour_EnumDisplayDevicesW_fn_, lpDevice, iDevNum, lpDisplayDevice, dwFlags);
//    std::wcout << "hook EnumDisplayDevicesW: " << result << ", lpDevice: " << (lpDevice !=  NULL? lpDevice: L"") << ", iDevNum: " << iDevNum << std::endl;
//    return result;
//}
//
//HMONITOR ProcessInjector::hkMonitorFromPoint(
//    _In_ POINT& pt,
//    _In_ DWORD& dwFlags) {
//    auto result = CallOriginal(detour_MonitorFromPoint_fn_, pt, dwFlags);
//    std::wcout << "hook MonitorFromPoint: " << result << ", pt: " << pt.x << ", dwFlags: " << dwFlags << std::endl;
//    return result;
//}
//
//HMONITOR ProcessInjector::hkMonitorFromRect(
//    _In_ LPCRECT& lprc,
//    _In_ DWORD& dwFlags) {
//    auto result = CallOriginal(detour_MonitorFromRect_fn_, lprc, dwFlags);
//    std::wcout << "hook MonitorFromRect: " << result << ", lprc: " << lprc->left << ", dwFlags: " << dwFlags << std::endl;
//    return result;
//}
//
//HMONITOR ProcessInjector::hkMonitorFromWindow(
//    _In_ HWND& hwnd,
//    _In_ DWORD& dwFlags) {
//    auto result = CallOriginal(detour_MonitorFromWindow_fn_, hwnd, dwFlags);
//    std::wcout << "hook MonitorFromWindow: " << result << ", hwnd: " << hwnd << ", dwFlags: " << dwFlags << std::endl;
//    return result;
//}


/*
NTSTATUS ProcessInjector::hkNtQueryInformationProcess(
    IN HANDLE& ProcessHandle,
    IN PROCESSINFOCLASS& ProcessInformationClass,
    OUT PVOID& ProcessInformation,
    IN ULONG& ProcessInformationLength,
    OUT PULONG& ReturnLength OPTIONAL
)
{
    auto result = CallOriginal(detour_query_info_proc_, ProcessHandle, ProcessInformationClass, ProcessInformation, ProcessInformationLength, ReturnLength);
    std::wcout << "NtQueryInformationProcess =" << result << " (" << ProcessHandle << ", " << ProcessInformationClass << ", " << ProcessInformation << "," << ProcessInformationLength << std::endl;
    return result;
}*/


// L"C:\\Program Files(x86)\\Google\\Chrome\\Application\\chrome.exe\" --type=utility  --service-sandbox-type=cdm --enable-audio-service-sandbox --service-request-c";

//LPSTR  ProcessInjector::hkGetCommandLineA(VOID)
//{
//    //auto result = CallOriginal(detour_get_commandlineA_);
//    //std::wcout << "GetCommandLineA =" << result << std::endl;
//
//    CHAR buf[MAX_PATH + 1];
//    GetModuleFileNameA(NULL, buf, MAX_PATH);
//    std::string base_path = buf;
//    std::string::size_type pos = base_path.find_last_of("\\/");
//    base_path = base_path.substr(0, pos);
//    //std::string chrome_ver = "\\80.0.3987.132";
//
//    auto ret = new char[MAX_PATH + 100];
//    strcpy_s(ret, MAX_PATH + 100, ("\"" + base_path + "\\chrome.exe" + "\"").c_str());
//    std::cout << "GetCommandLineA =" << ret << std::endl;
//    return ret;
//    //return result;
//}
//LPWSTR ProcessInjector::hkGetCommandLineW(VOID)
//{
//    //auto result = CallOriginal(detour_get_commandlineW_);
//    //std::wcout << "GetCommandLineW =" << result << std::endl;
//    //return L"\"C:\\Program Files(x86)\\Google\\Chrome\\Application\\chrome.exe\" --type=utility  --service-sandbox-type=cdm --enable-audio-service-sandbox --service-request-c";
//    
//    TCHAR buf[MAX_PATH + 1];
//    GetModuleFileNameW(NULL, buf, MAX_PATH);
//    std::wstring base_path = buf;
//    std::wstring::size_type pos = base_path.find_last_of(L"\\/");
//    base_path = base_path.substr(0, pos);
//    //std::wstring chrome_ver = L"\\80.0.3987.132";
//
//    auto ret = new wchar_t[MAX_PATH + 100];
//    wcscpy_s(ret, MAX_PATH + 100, (L"\"" + base_path + L"\\chrome.exe" + L"\"").c_str());
//    std::wcout << "GetCommandLineW =" << ret << std::endl;
//    return ret;
//    //return result; // L"\\chrome.exe" +  L"\"C:\\Program Files(x86)\\Google\\Chrome\\Application\\chrome.exe\" --type=utility  --service-sandbox-type=cdm --enable-audio-service-sandbox --service-request-c"
//}
