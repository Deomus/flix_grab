#include "widevine_loader.h"
#include "widevine_hook.h"
#include "decrypsis.h"

using namespace blackbone;


WidevineLoader::WidevineLoader()
{
    current_process_.Attach(GetCurrentProcessId());
    std::wcout << L"HookCreateProcess Constructed!" << std::endl;

    auto r = WidevineHook::Instance();

#if FILELOGGER
    file_logger.open(L"C:\\temp\\WidevineLoader.log", std::ofstream::out | std::ofstream::app | std::ofstream::binary);
    file_logger << L"WidevineLoader Constructed!" << std::endl;
    file_logger << std::endl;
#endif
}

WidevineLoader::~WidevineLoader()
{
    std::wcout << L"HookCreateProcess Destructed!" << std::endl;
#if FILELOGGER
    file_logger << L"HookCreateProcess Destructed!" << std::endl;
#endif
}

std::wstring dirnameOf_(const std::wstring& fname)
{
    size_t pos = fname.find_last_of(L"\\/");
    return (std::wstring::npos == pos)
        ? L""
        : fname.substr(0, pos);
}

HMODULE WidevineLoader::hkLoadLibraryW(_In_ LPCWSTR& lpLibFileName)
{
    HMODULE hModule;
    bool is_widevine = wcsstr(lpLibFileName, L"widevine");
    if (is_widevine)
    {
        WCHAR temp[MAX_PATH + 20] = { 0 };
        GetTempPathW(MAX_PATH, temp);
        auto wv_path = dirnameOf_(temp);

        //  //wcscat(temp, L"\\wirafreemod.dll"); //widevinecdm_mod
        wv_path += L"\\widevinecdm.dll";

        hModule = LoadLibraryW(wv_path.c_str());
#if FILELOGGER        
        file_logger << "::LoadLibraryW wv: " << hModule << ", from " << wv_path << std::endl;
#endif
    }
    else {
        hModule = LoadLibraryW(lpLibFileName);

#if FILELOGGER
        file_logger << "::LoadLibraryW: " << hModule << ", from " << lpLibFileName << std::endl;
#endif
    }

    if (is_widevine)
    {
#ifdef FILELOGGER
        auto st = WidevineHook::Instance()->Apply(file_logger);
        file_logger << "WidevineHook::Apply" << std::endl;
#else
        auto st = WidevineHook::Instance()->Apply();
#endif
    }

    if (hModule == NULL) {
        std::wcout << L"::LoadLibraryW " << lpLibFileName << L" Error: " << GetLastError() << std::endl;
    }

    return hModule;
}


HMODULE WidevineLoader::hkLoadLibraryExW(_In_ LPCWSTR& lpLibFileName, _Reserved_ HANDLE& hFile, _In_ DWORD& dwFlags) {
    
    //WCHAR pid[10] = {0};
    HMODULE hModule;
    //_snwprintf(pid, 10, L"%d", GetCurrentProcessId());

    bool is_widevine = wcsstr(lpLibFileName, L"widevine");

    if (is_widevine)
    {
        WCHAR temp[MAX_PATH + 20] = { 0 };
        GetTempPathW(MAX_PATH, temp);
        auto wv_path = dirnameOf_(temp);
    //  //wcscat(temp, L"\\wirafreemod.dll"); //widevinecdm_mod
        wv_path += L"\\widevinecdm.dll";
        hModule = LoadLibraryExW(wv_path.c_str(), hFile, dwFlags);
#if FILELOGGER
        file_logger << "::LoadLibraryExW: " << hModule << ", from: " << wv_path << std::endl;
#endif
    }
    else
    { 
#if FILELOGGER
        file_logger << "::LoadLibraryExW: " << lpLibFileName << std::endl;
#endif
        hModule = LoadLibraryExW(lpLibFileName, hFile, dwFlags);
    }

    if (is_widevine) {
        //DWORD dwBytesWritten = 0;
        //auto  hFile = CreateFile(TEXT("C:\\temp\\wvhook.log"), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        //WriteFile(hFile, "LoadLibraryExW Widevine ...   \r\n", 30, &dwBytesWritten, NULL);
        //CloseHandle(hFile);
#ifdef FILELOGGER
        auto r = WidevineHook::Instance()->Apply(file_logger);
        file_logger << "WidevineHook::Apply " << (r ? "ok" : "fail") << std::endl;
#else
        auto r = WidevineHook::Instance()->Apply();
#endif
    }

    if (hModule == NULL) {
        std::wcout << L"::LoadLibraryExW " << lpLibFileName << L" Error: " << GetLastError() << std::endl;
    }

    return hModule;

}

BOOL WidevineLoader::hkFreeLibrary(_In_ HMODULE& hLibModule)
{
    wchar_t module_name[MAX_PATH];

    GetModuleFileNameW(hLibModule, module_name, MAX_PATH);

    if (wcsstr(module_name, L"widevine")) {
        std::wcout << L"::FreeLibrary Widevine: " << module_name << std::endl;
    }
    return FreeLibrary(hLibModule);
}


HANDLE WidevineLoader::hkCreateThread(_In_opt_ LPSECURITY_ATTRIBUTES& lpThreadAttributes, _In_ SIZE_T& dwStackSize, _In_ LPTHREAD_START_ROUTINE& lpStartAddress, _In_opt_ __drv_aliasesMem LPVOID& lpParameter, _In_ DWORD& dwCreationFlags, _Out_opt_ LPDWORD& lpThreadId)
{
    DWORD tid = 0;
    HANDLE handle = ::CreateThread(lpThreadAttributes, dwStackSize, lpStartAddress, lpParameter, dwCreationFlags | CREATE_SUSPENDED, &tid);

    if (lpThreadId) *lpThreadId = tid;

    std::wcout << L"WidevineLoader::hkCreateThread called. ID: " << tid << std::endl;
#ifdef FILELOGGER
    file_logger << L"WidevineLoader::hkCreateThread called. ID: " << tid << std::endl;
#endif

   // UpdateHWBPHooks(tid);

    ResumeThread(handle);

    return handle;
}

FARPROC
WidevineLoader::hkGetProcAddress(
    _In_ HMODULE& hModule,
    _In_ LPCSTR& lpProcName) {
#ifdef FILELOGGER
    file_logger << L"WidevineLoader::GetProcAddress called: " << lpProcName << std::endl;
#endif
    return GetProcAddress(hModule, lpProcName);
}

bool WidevineLoader::Hook()
{
    bool result = true;

    result &= detour_load_library_w_.Hook(&::LoadLibraryW, &WidevineLoader::hkLoadLibraryW, this, HookType::Int3, CallOrder::NoOriginal, ReturnMethod::UseNew);
        
    result &= detour_load_library_ex_w_.Hook(&::LoadLibraryExW, &WidevineLoader::hkLoadLibraryExW, this, HookType::Int3, CallOrder::NoOriginal, ReturnMethod::UseNew);
    result &= detour_free_library_.Hook(&::FreeLibrary, &WidevineLoader::hkFreeLibrary, this, HookType::Int3, CallOrder::NoOriginal, ReturnMethod::UseNew);
       
    //result &= detour_get_procaddress_.Hook(&::GetProcAddress, &WidevineLoader::hkGetProcAddress, this, HookType::Int3, CallOrder::NoOriginal, ReturnMethod::UseNew);

    //_detCreateThread.Hook(&::CreateThread, &HookCreateProcess::hkCreateThread, this, HookType::Int3, CallOrder::NoOriginal, ReturnMethod::UseNew);

    //     result &= _detSetToken.Hook(&::SetTokenInformation, &WidevineLoader::hkSetTokenInformation, this, HookType::Int3, CallOrder::HookFirst, ReturnMethod::UseNew);

    //   result &= _detSetMitigation.Hook(&::SetProcessMitigationPolicy, &WidevineLoader::hkSetProcessMitigationPolicy, this, HookType::Int3, CallOrder::HookFirst, ReturnMethod::UseNew);

    //Initiate WideVine Loading;
    //LoadLibraryW(L"widevinecdm.dll");
#if FILELOGGER
    file_logger << "WidevineLoader Hook : " << result << std::endl;
#endif
    std::wcout << L"WidevineLoader Hook : " << result << std::endl;

    return result;
}

bool WidevineLoader::Unhook()
{
    bool result = true;

    result &= detour_load_library_w_.Restore();
    result &= detour_free_library_.Restore();
    //_detCreateThread.Hook(&::CreateThread, &HookCreateProcess::hkCreateThread, this, HookType::Int3, CallOrder::NoOriginal, ReturnMethod::UseNew);

    //result &= detour_set_token_.Restore();
    //result &= detour_set_mitigation_.Restore();

    std::wcout << L"WidevineLoader Unhook : " << result << std::endl;

    return result;
}

WidevineLoader* WidevineLoader::Instance()
{
    static WidevineLoader        hk;
    return &hk;
}
