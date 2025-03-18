#pragma once

#include "BlackBone/Config.h"
#include "BlackBone/Process/Process.h"
//#include "BlackBone/PE/PEImage.h"
//#include "BlackBone/Misc/Utils.h"
//#include "BlackBone/Misc/DynImport.h"
//
#include "BlackBone/localHook/LocalHook.hpp"
//
//#include <iostream>
//
//#include <iomanip>
#ifdef FILELOGGER
    #include <fstream>
#endif

#include "content_decryption_module_ext.h"
#include "content_decryption_module.h"
#include "cdm_ipc.h"


typedef void* (*CreateCdmInstanceFunc)(
    int cdm_interface_version,
    const char* key_system, uint32_t key_system_size,
    GetCdmHostFunc get_cdm_host_func, void* user_data);

typedef void (*InitializeCdmFunc)();
typedef void(*DeinitializeCdmFunc)();

typedef bool (*VerifyCdmHostFunc)(const cdm::HostFile* host_files,  uint32_t num_files);

typedef void* (*GetHandleVerifierFunc)();

extern "C" {
    __declspec(dllimport) void* GetHandleVerifier();
}

class WidevineHook
{

public:
    //Apply hook
#ifdef FILELOGGER
    bool                    Apply(std::wofstream& file_logger);
#else
    bool                    Apply();
#endif
    //Singleton;
    static WidevineHook     hk;

    static WidevineHook*    Instance()
    {
        static WidevineHook        hk;
        return &hk;
        //if (hk == nullptr)
        //    hk = new WidevineHook();
        //return hk;
    }
    
private:
    WidevineHook();
    ~WidevineHook();

  
    void hkInitializeCdmModule_4();
    void hkDeinitializeCdmModule();

    void* hkCreateCdmInstance(int& cdm_interface_version, const char*& key_system, uint32_t& key_system_size,
        GetCdmHostFunc& get_cdm_host_func, void*& user_data);

    bool hkVerifyCdmhost(const cdm::HostFile*& host_files, uint32_t& num_files);
    void* hkGetHandleVerifier();

private:
    CreateCdmInstanceFunc                                               fn_create_original_;
    InitializeCdmFunc                                                   fn_initialize_cdm_;
    DeinitializeCdmFunc                                                 fn_deinitialize_cdm_;
    VerifyCdmHostFunc                                                   fn_verify_cdmhost_;
    GetHandleVerifierFunc                                               fn_get_handle_verifier_;

    //Protected Functions:
    blackbone::Detour<decltype(&::INITIALIZE_CDM_MODULE), WidevineHook> detour_init_;
    blackbone::Detour<decltype(&::DeinitializeCdmModule), WidevineHook> detour_deinit_;

    blackbone::Detour<decltype(&::CreateCdmInstance), WidevineHook>     detour_create_;
    blackbone::Detour<decltype(&::VerifyCdmHost_0), WidevineHook>       detour_verify_;
    blackbone::Detour<decltype(&::GetHandleVerifier), WidevineHook>     detour_get_handler_verify_;

    blackbone::Process                                                  current_process_;

    bool                                                                is_widevine_initialized_;
#ifdef FILELOGGER
    std::ofstream                                                       file_logger;
#endif
    std::unique_ptr<ipc::Server>                                        host_server_;
    ipc::Client*                                                        cdm_client_;
};