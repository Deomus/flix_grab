#include "process_injector.h"
#include "phylloxera.h"

#include <BlackBone/Process/RPC/RemoteFunction.hpp>

#include "widevine_loader.h"
#include "utils.h"


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

    std::wcout << L"Injection in process: " << proc_id << std::endl;

    NTSTATUS st = child.AttachSuspended(proc_id);

    if (NT_SUCCESS(st)) {

        const ModuleData* module = _current.modules().GetModule((module_t)GetCurrentModule());

        if (module)
        {
            std::wcout << L"Injection Module: " << module->fullPath << std::endl;
            module = child.modules().Inject(module->fullPath, &st);

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
                }
                
               
            }

        }
        else
            std::wcout << L"Cant find injection module." << std::endl;

    }
    else  {
        std::wcout << L"Cant Attach to the process " << proc_id << std::endl;
    }

    return st;
}

BOOL ProcessInjector::hkCreateProcessAsUserW(_In_opt_ HANDLE& hToken, _In_opt_ LPCWSTR& lpApplicationName, _Inout_opt_ LPWSTR& lpCommandLine, _In_opt_ LPSECURITY_ATTRIBUTES& lpProcessAttributes, _In_opt_ LPSECURITY_ATTRIBUTES& lpThreadAttributes, _In_ BOOL& bInheritHandles, _In_ DWORD& dwCreationFlags, _In_opt_ LPVOID& lpEnvironment, _In_opt_ LPCWSTR& lpCurrentDirectory, _In_ LPSTARTUPINFOW& lpStartupInfo, _Out_ LPPROCESS_INFORMATION& lpProcessInformation)
{
    std::wcout << L"CreateProcessAsUserW: " << lpCommandLine << std::endl;

    dwCreationFlags = dwCreationFlags ^ EXTENDED_STARTUPINFO_PRESENT;
    bInheritHandles = TRUE;
    hToken = NULL;
    lpStartupInfo->cb = sizeof(STARTUPINFOW);

    lpStartupInfo->wShowWindow = SW_SHOWDEFAULT;
    lpStartupInfo->hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    lpStartupInfo->hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    lpStartupInfo->hStdError = GetStdHandle(STD_ERROR_HANDLE);

    lpStartupInfo->lpDesktop = NULL;

    BOOL result = ::CreateProcessAsUserW(hToken, lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes,
        bInheritHandles, dwCreationFlags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation);

    if (wcsstr(lpCommandLine, L"ppapi") && result)
    {
        InjectLoader(lpProcessInformation->dwProcessId);

    }

    return result;
}

BOOL ProcessInjector::hkCreateProcessW(_In_opt_ LPCWSTR& lpApplicationName, _Inout_opt_ LPWSTR& lpCommandLine, _In_opt_ LPSECURITY_ATTRIBUTES& lpProcessAttributes, _In_opt_ LPSECURITY_ATTRIBUTES& lpThreadAttributes, _In_ BOOL& bInheritHandles, _In_ DWORD& dwCreationFlags, _In_opt_ LPVOID& lpEnvironment, _In_opt_ LPCWSTR& lpCurrentDirectory, _In_ LPSTARTUPINFOW& lpStartupInfo, _Out_ LPPROCESS_INFORMATION& lpProcessInformation)
{
    if (lpCommandLine) {
        std::wcout << L"CreateProcessW: " << lpCommandLine << std::endl;
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

    BOOL result = CallOriginal(_detCreateProcessW, lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes,
        bInheritHandles, dwCreationFlags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation);

    /*BOOL result = _detCreateProcessW.CallOriginal(std::move(lpApplicationName), std::move(lpCommandLine), std::move(lpProcessAttributes), std::move(lpThreadAttributes),
        std::move(bInheritHandles), std::move(dwCreationFlags), std::move(lpEnvironment), std::move(lpCurrentDirectory), std::move(lpStartupInfo), std::move(lpProcessInformation));*/

    /*  void* p = &CreateProcessW;

      BOOL result = ::CreateProcessW(lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes,
      bInheritHandles, dwCreationFlags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation);*/

    if (lpCommandLine && wcsstr(lpCommandLine, L"ppapi") && result)
    {
        InjectLoader(lpProcessInformation->dwProcessId);
    }

    if (!bSuspended)
        ResumeThread(lpProcessInformation->hThread);



    return result;
}

bool ProcessInjector::Hook()
{
    bool result = true;
    //Inline

    /*   if (result)
           result = _detCreateProcessAsUserW.Hook(&::CreateProcessAsUserW, &ProcessInjector::hkCreateProcessAsUserW, this, HookType::Int3, CallOrder::NoOriginal, ReturnMethod::UseNew);
           if (result)
           result = _detCreateProcessW.Hook(&::CreateProcessW, &ProcessInjector::hkCreateProcessW, this, HookType::Int3, CallOrder::NoOriginal, ReturnMethod::UseNew);*/

    result &= _detCreateProcessAsUserW.Hook(&::CreateProcessAsUserW, &ProcessInjector::hkCreateProcessAsUserW, this, HookType::Inline, CallOrder::NoOriginal, ReturnMethod::UseNew);
    result &= _detCreateProcessW.Hook(&::CreateProcessW, &ProcessInjector::hkCreateProcessW, this, HookType::Inline, CallOrder::NoOriginal, ReturnMethod::UseNew);

    std::wcout << L"Injector Hook: " << result << std::endl;

    return result;
}

bool ProcessInjector::Unhook()
{
    bool result = true;

    result &= _detCreateProcessAsUserW.Restore();
    result &= _detCreateProcessW.Restore();

    std::wcout << L"Injector Unhook: " << result << std::endl;

    return result;
}

BOOL ProcessInjector::hkSetTokenInformation(_In_ HANDLE& TokenHandle, _In_ TOKEN_INFORMATION_CLASS& TokenInformationClass, _In_reads_bytes_(TokenInformationLength) LPVOID& TokenInformation, _In_ DWORD& TokenInformationLength)
{
    std::wcout << L"SetTokenInformation : " << TokenInformationClass << std::endl;
    TokenHandle = NULL;
    return TRUE;
}

BOOL ProcessInjector::hkSetProcessMitigationPolicy(_In_ PROCESS_MITIGATION_POLICY& MitigationPolicy, _In_reads_bytes_(dwLength) PVOID& lpBuffer, _In_ SIZE_T& dwLength)
{
    std::wcout << L"SetProcessMitigationPolicy : " << MitigationPolicy << std::endl;
    return TRUE;
}
