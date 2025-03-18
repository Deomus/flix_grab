#include "phylloxera.h"
#include "widevine_loader.h"
#include <iostream>

#include "process_injector.h"
#include "decrypt_provider.h"


static const char       phylloxera_version[] = "1.1";
static int              phylloxera_refs = 0;

PX_API const char* GetPhylloxeraVersion()
{
    return phylloxera_version;
}

PX_API PxStatus  InstallPhylloxera()
{
    PxStatus status = kPx_Success;
    if (phylloxera_refs == 0) {
        
        if (DecryptProvider::Instance()->Start()) {
            if (!ProcessInjector::Install())
                status = kPx_InternalError;
        }
        else
            status = kPx_InterprocessError;
    }

    phylloxera_refs += status == kPx_Success ? 1 : 0;
    
    return status;
    
}

PX_API PxStatus  UninstallPhylloxera()
{
    PxStatus status = kPx_Success;
    if (phylloxera_refs > 0) {
        phylloxera_refs--;
        if (phylloxera_refs == 0) {
            if (!ProcessInjector::Uninstall())
                status = kPx_InternalError;

            DecryptProvider::Instance()->Stop();
        }
    }
    std::cout << "Phylloxera not initialized." << std::endl;
    return status;
}


PX_API bool  DetourPoint()
{   
    std::cout << "Detour Point!" << std::endl;
    return WidevineLoader::Instance()->Hook();
}

PX_API PxStatus GetDecryptService(const uint8_t* key_id, int key_id_size, AbstractDecryptService** service, int timeout /*= 0*/)
{
    PxStatus status = kPx_Success;
    if (phylloxera_refs > 0) {
        key_id_t key(key_id, key_id + key_id_size);

        *service = DecryptProvider::Instance()->WaitForService(key, timeout);
        if (*service == NULL)
            status = kPx_Timeout;
    }
    else
        status = kPx_NotInitialized;

    return status;
}

PX_API PxStatus  RegisterCallback(ServiceCallback* callback) {
    PxStatus status = kPx_Success;
    if (phylloxera_refs > 0) {
        DecryptProvider::Instance()->RegisterCallback(callback);
    }
    else
        status = kPx_NotInitialized;

    return status;
}

PX_API PxStatus  UnregisterCallback(ServiceCallback* callback) {
    PxStatus status = kPx_Success;
    if (phylloxera_refs > 0) {
        DecryptProvider::Instance()->UnregisterCallback(callback);
    }
    else
        status = kPx_NotInitialized;

    return status;
}
