#include "widevine_hook.h"
#include "cdm_proxy.h"
#include "cdm_host.h"
#include "cdm_ipc.h"

using ContentDecryptionModule = cdm::ContentDecryptionModule_10;

WidevineHook::WidevineHook() 
    : fn_create_original_(NULL)
    , fn_initialize_cdm_(NULL)
    , fn_deinitialize_cdm_(NULL)
    , is_widevine_initialized_(false)
{
#ifdef FILELOGGER
    file_logger.open("C:\\temp\\WidevineHook.log", std::ofstream::app);
    file_logger << std::endl;
    file_logger << "WidevineHook... ";
    //file_logger.close();
#endif
    NTSTATUS st = current_process_.Attach(GetCurrentProcessId());
    std::wcout << L"HookWidevine Constructed! Attach Status: " << st << std::endl;
#ifdef FILELOGGER
    file_logger << "HookWidevine Attach Status: " << st << std::endl;
#endif
    host_server_ = std::make_unique<ipc::Server>(DEFAULT_CDM_HOST_IPC, SERVER_TABLE(HostTable));
    if (!host_server_->status().Ok()) {
        std::cout << "Cant Start HOST Server " << host_server_->name() << std::endl;
#ifdef FILELOGGER
        file_logger << "Cant Start HOST Server " << host_server_->name() << std::endl;
#endif
    }
    cdm_client_ = new ipc::Client(DEFAULT_CDM_IPC);
}

WidevineHook::~WidevineHook()
{
    std::wcout << L"HookWidevine Destructed!" << std::endl;
#ifdef FILELOGGER
    file_logger << "HookWidevine Destructed!" << std::endl;
#endif
}


void WidevineHook::hkInitializeCdmModule_4()
{
    std::wcout << L"HookWidevine::hkInitializeCdmModule_4 called." << std::endl;
#ifdef FILELOGGER
    file_logger << "HookWidevine::hkInitializeCdmModule_4 called." << std::endl;
#endif
    //host_server_ = std::make_unique<ipc::Server>(DEFAULT_CDM_HOST_IPC, SERVER_TABLE(HostTable));
    //if (!host_server_->status().Ok()) {
    //    std::cout << "Cant Start HOST Server " << host_server_->name() << std::endl;
//#ifdef FILELOGGER
    //  file_logger << "Cant Start HOST Server " << host_server_->name() << std::endl;
//#endif
    //}

    fn_initialize_cdm_();
    is_widevine_initialized_ = true;
    std::wcout << L"HookWidevine::hkInitializeCdmModule_4 completed." << std::endl;
}

void WidevineHook::hkDeinitializeCdmModule()
{
    std::wcout << L"HookWidevine::hkDeinitializeCdmModule called." << std::endl;
#ifdef FILELOGGER
    file_logger << "HookWidevine::hkDeinitializeCdmModule called." << std::endl;
#endif
        
    is_widevine_initialized_ = false;
    fn_deinitialize_cdm_();

    host_server_->Cancel();
}

bool WidevineHook::hkVerifyCdmhost(const cdm::HostFile*& host_files, uint32_t& num_files)
{
    bool ret;
    std::wcout << L"WidevineHook::hkVerifyCdmhost host_files: " << host_files->file_path << " num_files: " << num_files;
    std::wcout << "ret = " << (ret = fn_verify_cdmhost_(host_files, num_files)) << std::endl;
#ifdef FILELOGGER
    file_logger << "WidevineHook::hkVerifyCdmhost host_files: " << host_files->file_path << " num_files: " << num_files;
#endif
    return ret;
}
void* WidevineHook::hkGetHandleVerifier()
{
    std::wcout << L"WidevineHook::hkGetHandleVerifier ";
    auto ret = fn_get_handle_verifier_();
    std::wcout << " ret = " << ret  << std::endl;
    return ret;
}

void* WidevineHook::hkCreateCdmInstance(int& cdm_interface_version, const char*& key_system, uint32_t& key_system_size, GetCdmHostFunc& get_cdm_host_func, void*& user_data)
{
    std::wcout << L"WidevineHook::hkCreateCdmInstance version: " << cdm_interface_version << " KeySystem: " << key_system << std::endl;
#ifdef FILELOGGER
    file_logger << "WidevineHook::hkCreateCdmInstance version: " << cdm_interface_version << " KeySystem: " << key_system << std::endl;
#endif
    
    //Chromium 56 cdm==9
    //Skip all other version (higher 8)
    //if (cdm_interface_version != 8)
    //Chromium 59 cdm==10 ?
    if (cdm_interface_version != 10)
        return nullptr;
    
    using host_type = ContentDecryptionModule::Host;
    host_type* host = 
        reinterpret_cast<host_type*>(get_cdm_host_func(host_type::kVersion, user_data));
    
#ifndef NO_CDM_PROXY
#ifdef FILELOGGER
    auto cdm_proxy = new CdmProxyDS(*cdm_client_, std::string(key_system, key_system_size), host, file_logger);
#else
    //auto cdm_proxy = new CdmProxyDS(DEFAULT_CDM_IPC, std::string(key_system, key_system_size), host);
    auto cdm_proxy = new CdmProxyDS(*cdm_client_, std::string(key_system, key_system_size), host);
#endif
#else
    // for debugging purpose. 
    // You need a second copy of cdm with name: widevinecdm_2.dll
    auto cdm_proxy = CdmTrueModule::CreateInstance(std::string(key_system, key_system_size), DEFAULT_CDM_IPC, host);
#endif    
    return (void*)cdm_proxy;
}

#ifdef FILELOGGER
bool WidevineHook::Apply(std::wofstream& file_logger)
#else
bool WidevineHook::Apply()
#endif
{
#ifdef FILELOGGER
    file_logger << "HookWidevine Apply..." << std::endl;
#endif
    //   current_process_.Detach();
    NTSTATUS st = current_process_.Attach(GetCurrentProcessId());
    std::wcout << L"HookWidevine Constructed! Attach Status: " << st << std::endl;
#ifdef FILELOGGER
    file_logger << "HookWidevine Attach Status: " << st << std::endl;
#endif
    std::wcout << L"HookWidevine Apply" << std::endl;
    bool result = true;
    
    // Get function
    const blackbone::ModuleData* widevine_module = current_process_
        .modules()
        //.GetModule(L"wirafreemod.dll"); //widevinecdm_mod
        .GetModule(L"widevinecdm.dll");
    
    if (widevine_module == nullptr) {
        std::wcout << L"Not found widevinecdm.dll, aborting\n";
#ifdef FILELOGGER
        file_logger << L"Not found widevinecdm.dll, aborting\n";
#endif
        result = false;
    }

    if (result) {
        auto pHookFn = current_process_.modules().GetExport(
            widevine_module,
            "InitializeCdmModule_4"
            );

        if (pHookFn.procAddress != 0) {
            fn_initialize_cdm_ = (InitializeCdmFunc)pHookFn.procAddress;
            result = detour_init_.Hook((void(*)())pHookFn.procAddress, &WidevineHook::hkInitializeCdmModule_4, this,
                blackbone::HookType::Int3, blackbone::CallOrder::NoOriginal, blackbone::ReturnMethod::UseNew);
                //blackbone::HookType::HWBP, blackbone::CallOrder::NoOriginal, blackbone::ReturnMethod::UseNew);
#ifdef FILELOGGER
            file_logger << "InitializeCdmModule_4 hooked: " << (result ? "ok" : "fail") << ", orig: " << fn_initialize_cdm_ << std::endl;
#endif
        }
        else {
            std::wcout << L"Not found InitializeCdmModule_4, aborting\n";
#ifdef FILELOGGER
            file_logger << "Not found InitializeCdmModule_4, aborting\n";
#endif
        }
    }
    

    if (result) {
        auto pHookFn = current_process_.modules().GetExport(
            widevine_module,
            "DeinitializeCdmModule"
            );

        if (pHookFn.procAddress != 0) {
            fn_deinitialize_cdm_ = (DeinitializeCdmFunc)pHookFn.procAddress;
            result = detour_deinit_.Hook((void(*)())pHookFn.procAddress, &WidevineHook::hkDeinitializeCdmModule, this,
                blackbone::HookType::Int3, blackbone::CallOrder::NoOriginal, blackbone::ReturnMethod::UseNew);
                //blackbone::HookType::HWBP, blackbone::CallOrder::NoOriginal, blackbone::ReturnMethod::UseNew);
#ifdef FILELOGGER
            file_logger << "DeinitializeCdmModule hooked: " << (result ? "ok" : "fail") << ", orig: " << fn_deinitialize_cdm_ << std::endl;
#endif
        }
        else {
            std::wcout << L"Not found DeinitializeCdmModule, aborting\n";
#ifdef FILELOGGER
            file_logger << "Not found DeinitializeCdmModule, aborting\n";
#endif
        }
    }

    if (result) {
        auto pHookFn = current_process_.modules().GetExport(
            widevine_module,
            "CreateCdmInstance"
            );
        
        if (pHookFn.procAddress != 0)
        {
            fn_create_original_ = (CreateCdmInstanceFunc)pHookFn.procAddress;
            result = detour_create_.Hook(fn_create_original_, &WidevineHook::hkCreateCdmInstance, this,
                blackbone::HookType::Int3, blackbone::CallOrder::NoOriginal, blackbone::ReturnMethod::UseNew);
                //blackbone::HookType::HWBP, blackbone::CallOrder::NoOriginal, blackbone::ReturnMethod::UseNew);
#ifdef FILELOGGER
            file_logger << "CreateCdmInstance hooked: " << (result ? "ok" : "fail") << ", orig: " << fn_create_original_ << std::endl;
#endif
        }
        else {
            std::wcout << L"Not found CreateCdmInstance, aborting\n";
#ifdef FILELOGGER
            file_logger << "Not found CreateCdmInstance, aborting\n";
#endif
        }
    }

    if (result) {
        auto pHookFn = current_process_.modules().GetExport(
            widevine_module,
            "VerifyCdmHost_0"
        );

        if (pHookFn.procAddress != 0)
        {
            fn_verify_cdmhost_ = (VerifyCdmHostFunc)pHookFn.procAddress;
            detour_verify_.Hook(fn_verify_cdmhost_, &WidevineHook::hkVerifyCdmhost, this,
                blackbone::HookType::Int3, blackbone::CallOrder::NoOriginal, blackbone::ReturnMethod::UseNew);
                //blackbone::HookType::HWBP, blackbone::CallOrder::NoOriginal, blackbone::ReturnMethod::UseNew);
        }
        else {
            std::wcout << L"Not found VerifyCdmHost_0\n";
#ifdef FILELOGGER
            file_logger << "Not found VerifyCdmHost_0\n";
#endif
        }
    }
    if (result) {
        auto pHookFn = current_process_.modules().GetExport(
            widevine_module,
            "GetHandleVerifier"
        );

        if (pHookFn.procAddress != 0)
        {
            fn_get_handle_verifier_ = (GetHandleVerifierFunc)pHookFn.procAddress;
            detour_get_handler_verify_.Hook(fn_get_handle_verifier_, &WidevineHook::hkGetHandleVerifier, this,
                blackbone::HookType::Int3, blackbone::CallOrder::NoOriginal, blackbone::ReturnMethod::UseNew);
                //blackbone::HookType::HWBP, blackbone::CallOrder::NoOriginal, blackbone::ReturnMethod::UseNew);
        }
        else {
            std::wcout << L"Not found GetHandleVerifier, aborting\n";
#ifdef FILELOGGER
            file_logger << "Not found GetHandleVerifier, aborting\n";
#endif
        }
    }

    std::wcout << L"Widevine Hooked " << result << std::endl;
#ifdef FILELOGGER
    file_logger << "Widevine Hooked " << (result ? "ok" : "fail") << std::endl;
#endif
    return result;
}
