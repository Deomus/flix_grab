#include "widevine_hook.h"
#include "cdm_proxy.h"
#include "host_proxy.h"


extern "C" {

    static void* hkGetCdmHost(int host_interface_version, void* user_data)
    {
        if (host_interface_version == HostProxy::kVersion)
            return user_data;
        return NULL;
    }
}

WidevineHook::WidevineHook() 
    : fn_create_original_(NULL)
    , fn_initialize_cdm_(NULL)
    , fn_deinitialize_cdm_(NULL)
    , next_id_(0)
    , is_widevine_initialized_(false)
{
    current_process_.Attach(GetCurrentProcessId());
    std::wcout << L"HookWidevine Constructed!" << std::endl;
}

WidevineHook::~WidevineHook()
{
    std::wcout << L"HookWidevine Destructed!" << std::endl;
    if (is_widevine_initialized_ && fn_deinitialize_cdm_) {
        fn_deinitialize_cdm_();
    }
}

void WidevineHook::hkInitializeCdmModule_4()
{
    std::wcout << L"HookWidevine::hkInitializeCdmModule_4 called." << std::endl;
    if (!is_widevine_initialized_) {
        fn_initialize_cdm_();
        is_widevine_initialized_ = true;
    }

}

void WidevineHook::hkDeinitializeCdmModule()
{
    std::wcout << L"HookWidevine::hkDeinitializeCdmModule called." << std::endl;

    if (!is_widevine_initialized_) {
        std::wcout << L"HookWidevine Deinitializing" << std::endl;
        fn_deinitialize_cdm_();
    }

}

void* WidevineHook::hkCreateCdmInstance(int& cdm_interface_version, const char*& key_system, uint32_t& key_system_size, GetCdmHostFunc& get_cdm_host_func, void*& user_data)
{
    std::wcout << L"WidevineHook::hkCreateCdmInstance id: " << next_id_ << " version: " << cdm_interface_version << " KeySystem: " << key_system << std::endl;
    HostProxy* host_proxy = new HostProxy(next_id_, (cdm::ContentDecryptionModule::Host*)get_cdm_host_func(HostProxy::kVersion, user_data));

    cdm::ContentDecryptionModule* cdm = (cdm::ContentDecryptionModule*)fn_create_original_(cdm_interface_version, key_system, key_system_size, hkGetCdmHost, host_proxy);

    CdmProxy* cdm_proxy = new CdmProxy(cdm, host_proxy);

    next_id_++;
    return cdm_proxy;
}

bool WidevineHook::Apply()
{
    bool result = true;
    // Get function
    const blackbone::ModuleData* widevine_module = current_process_.modules().GetModule(L"widevinecdm.dll");
    
    if (widevine_module == nullptr) {
        std::wcout << L"Not found widevinecdm.dll, aborting\n";
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
                blackbone::HookType::HWBP, blackbone::CallOrder::NoOriginal, blackbone::ReturnMethod::UseNew);
        }
        else
            std::wcout << L"Not found InitializeCdmModule_4, aborting\n";
    }
    

    if (result) {
        auto pHookFn = current_process_.modules().GetExport(
            widevine_module,
            "DeinitializeCdmModule"
            );

        if (pHookFn.procAddress != 0){
            fn_deinitialize_cdm_ = (DeinitializeCdmFunc)pHookFn.procAddress;
            result = detour_deinit_.Hook((void(*)())pHookFn.procAddress, &WidevineHook::hkDeinitializeCdmModule, this, 
                blackbone::HookType::HWBP, blackbone::CallOrder::NoOriginal, blackbone::ReturnMethod::UseNew);
        }
        else
            std::wcout << L"Not found DeinitializeCdmModule, aborting\n";
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
                blackbone::HookType::HWBP, blackbone::CallOrder::NoOriginal, blackbone::ReturnMethod::UseNew);
        }
        else
            std::wcout << L"Not found CreateCdmInstance, aborting\n";
    }


    std::wcout << L"Widevine Hooked\n";

    return result;
}
