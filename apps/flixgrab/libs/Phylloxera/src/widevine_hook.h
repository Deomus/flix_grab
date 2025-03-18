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

#include "content_decryption_module.h"


typedef void* (*CreateCdmInstanceFunc)(
    int cdm_interface_version,
    const char* key_system, uint32_t key_system_size,
    GetCdmHostFunc get_cdm_host_func, void* user_data);

typedef void (*InitializeCdmFunc)();
typedef void(*DeinitializeCdmFunc)();

class WidevineHook
{

public:
    //Apply hook
    bool                    Apply();
    //Singleton;
    static WidevineHook*    Instance()
    {
        static WidevineHook        hk;
        return &hk;
    }
    
private:
    WidevineHook();
    ~WidevineHook();

  
    void hkInitializeCdmModule_4();
    void hkDeinitializeCdmModule();

    void* hkCreateCdmInstance(int& cdm_interface_version, const char*& key_system, uint32_t& key_system_size,
        GetCdmHostFunc& get_cdm_host_func, void*& user_data);


private:
    CreateCdmInstanceFunc                     fn_create_original_;
    InitializeCdmFunc                         fn_initialize_cdm_;
    DeinitializeCdmFunc                       fn_deinitialize_cdm_;

    //Protected Functions:
    blackbone::Detour<decltype(&::INITIALIZE_CDM_MODULE), WidevineHook> detour_init_;
    blackbone::Detour<decltype(&::DeinitializeCdmModule), WidevineHook> detour_deinit_;

    blackbone::Detour<decltype(&::CreateCdmInstance), WidevineHook> detour_create_;
        
    int                                                 next_id_;

    blackbone::Process                                  current_process_;

    bool                                                is_widevine_initialized_;

};