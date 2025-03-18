#include "../Config.h"
#include "ProcessCore.h"
#include "../Misc/DynImport.h"
#include "../Include/Macro.h"
#include <VersionHelpers.h>

#ifdef FILELOGGER
    #include <iostream>
    #include <iomanip>
    #include <fstream>
#endif

namespace blackbone
{

#ifdef COMPILER_GCC
#define PROCESS_DEP_ENABLE  0x00000001
#endif

ProcessCore::ProcessCore()
    : _native( nullptr )
{
    DynImport::load( "GetProcessDEPPolicy", L"kernel32.dll" );
}

ProcessCore::~ProcessCore()
{
    Close();
}

/// <summary>
/// Attach to existing process
/// </summary>
/// <param name="pid">Process ID</param>
/// <param name="access">Access mask</param>
/// <returns>Status</returns>
NTSTATUS ProcessCore::Open( DWORD pid, DWORD access )
{
    // Prevent handle leak
    Close();

#ifdef FILELOGGER
    std::ofstream file_logger;
    file_logger.open("C:\\temp\\BlackBone.log", std::ofstream::out | std::ofstream::app | std::ofstream::binary);
    file_logger << std::endl;
    file_logger << "ProcessCore::Open pid: " << pid << std::endl;
#endif

    // Handle current process differently
    _hProcess = (pid == GetCurrentProcessId()) ? GetCurrentProcess() : OpenProcess(access, false, pid);

    // Some routines in win10 does not support pseudo handle
    if (IsWindows10OrGreater() && pid == GetCurrentProcessId())
        _hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

    if (_hProcess != NULL)
    {
#ifdef FILELOGGER
        file_logger << " _hProcess != NULL " << std::endl;
#endif
        _pid = pid;
        auto st = Init();
#ifdef FILELOGGER
        file_logger << " status " << st << std::endl;
#endif
        return st;
    }

    auto st = LastNtStatus();
#ifdef FILELOGGER
    file_logger << " status " << st << std::endl;
#endif

    return st;
}

/// <summary>
/// Attach to existing process
/// </summary>
/// <param name="pid">Process ID</param>
/// <param name="access">Access mask</param>
/// <returns>Status</returns>
NTSTATUS ProcessCore::Open( HANDLE handle )
{
    Close();

    _hProcess = handle;
    _pid = GetProcessId( _hProcess );

    return Init();
}


/// <summary>
/// Initialize some internal data
/// </summary>
/// <returns>Status code</returns>
NTSTATUS ProcessCore::Init()
{
    // Detect x86 OS
    SYSTEM_INFO info = { { 0 } };
    GetNativeSystemInfo( &info );

    if (info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL)
    {
        _native.reset( new x86Native( _hProcess ) );
    }
    else
    {
        // Detect wow64 barrier
        BOOL wowSrc = FALSE;
        IsWow64Process( GetCurrentProcess(), &wowSrc );

        if (wowSrc == TRUE)
            _native.reset( new NativeWow64( _hProcess ) );
        else
            _native.reset( new Native( _hProcess ) );
    }

    // Get DEP info
    // For native x64 processes DEP is always enabled
    if (_native->GetWow64Barrier().targetWow64 == false)
    {
        _dep = true;
    }
    else
    {
        DWORD flags = 0;
        BOOL perm = 0;

        if (SAFE_CALL( GetProcessDEPPolicy, _hProcess, &flags, &perm ))
            _dep = (flags & PROCESS_DEP_ENABLE) != 0;
    }

    return STATUS_SUCCESS;
}

/// <summary>
/// Close current process handle
/// </summary>
void ProcessCore::Close()
{
    if (_hProcess)
    {
        CloseHandle( _hProcess );

        _hProcess = NULL;
        _pid = 0;
        _native.reset( nullptr );
    }
}

bool ProcessCore::isProtected()
{
    if (_hProcess)
    {
        _PROCESS_EXTENDED_BASIC_INFORMATION_T<DWORD64> info = { 0 };
        info.Size = sizeof( info );
        
        _native->QueryProcessInfoT( ProcessBasicInformation, &info, sizeof( info ) );
        return info.Flags.IsProtectedProcess;
    }

    return false;
}

}