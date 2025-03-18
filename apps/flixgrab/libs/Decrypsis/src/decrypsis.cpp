#include "decrypsis.h"
#include "widevine_loader.h"
#include <iostream>

#include "process_injector.h"
#include "decrypt_provider.h"

#include "cdm_ipc.h"


static const char       decrypsis_version[] = "2.1";
static int              decrypsis_refs = 0;
#if FILELOGGER
    static std::wofstream file_logger;
#endif

PX_API const char* GetDecrypsisVersion()
{
    return decrypsis_version;
}

PX_API PxStatus  InstallDecrypsis()
{
#if FILELOGGER
    file_logger.open(L"C:\\temp\\Decrypsis.log", std::ofstream::out | std::ofstream::app | std::ofstream::binary);
    file_logger << std::endl;
#endif

    PxStatus status = kPx_Success;
    if (decrypsis_refs == 0) {
        //TODO: Add Cdm Ipc Server;
        
        if (DecryptProvider::Instance()->Start()) {
            if (!ProcessInjector::Install())
                status = kPx_InternalError;
            else {
                std::cout << "Decrypsis is initialized." << std::endl;
            }

        }
        else
            status = kPx_InterprocessError;
    }

    decrypsis_refs += status == kPx_Success ? 1 : 0;
    
#if FILELOGGER
    file_logger << "InstallDecrypsis status: " << status << std::endl;
#endif
    return status;
    
}

PX_API PxStatus  UninstallDecrypsis()
{
    PxStatus status = kPx_Success;
    if (decrypsis_refs > 0) {
        decrypsis_refs--;
        if (decrypsis_refs == 0) {
            if (!ProcessInjector::Uninstall())
                status = kPx_InternalError;

            DecryptProvider::Instance()->Stop();
            std::cout << "Decrypsis not initialized." << std::endl;
        }
    }
    
    return status;
}


PX_API bool  DetourPoint()
{   
#if FILELOGGER
    file_logger << "Detour Point!" << std::endl;
#endif
    std::cout << "Detour Point!" << std::endl;
    return WidevineLoader::Instance()->Hook();
	//return true; //anti-HACK  
}

PX_API PxStatus GetDecryptService(const uint8_t* key_id, int key_id_size, AbstractDecryptService** service, int timeout /*= 0*/)
{
    PxStatus status = kPx_Success;
    if (decrypsis_refs > 0) {
        key_id_t key(key_id, key_id + key_id_size);

        *service = DecryptProvider::Instance()->WaitForService(key, timeout);
        if (*service == NULL)
            status = kPx_Timeout;
    }
    else
        status = kPx_NotInitialized;

    return status;
}

PX_API PxStatus  RegisterDecrypsisCallback(ServiceCallback* callback) {
    PxStatus status = kPx_Success;
    assert(decrypsis_refs > 0);
    if (decrypsis_refs > 0) {
        DecryptProvider::Instance()->RegisterCallback(callback);
    }
    else
        status = kPx_NotInitialized;

    return status;
}

PX_API PxStatus  UnregisterDecrypsisCallback(ServiceCallback* callback) {
    PxStatus status = kPx_Success;

    assert(decrypsis_refs > 0);
    if (decrypsis_refs > 0) {
        DecryptProvider::Instance()->UnregisterCallback(callback);
    }
    else
        status = kPx_NotInitialized;

    return status;
}

PX_API PxStatus  DecryptCreateSessionAndGenerateRequest(const uint8_t* init_data, int init_data_size, SessionMessageCallback* session_cb, bool verify_cdm_host, int* host_primise_id)
{
    PxStatus status = kPx_Success;
    assert(decrypsis_refs > 0);
    if (decrypsis_refs > 0) {
        DecryptProvider::Instance()->CreateSessionAndGenerateRequest(init_data, init_data_size, session_cb, verify_cdm_host, host_primise_id);
    }
    else
        status = kPx_NotInitialized;
    return status;
}

PX_API PxStatus     DecryptUpdateSession(const char* session, int session_size, const char* license, int license_size, int host_primise_id)
{
    PxStatus status = kPx_Success;
    assert(decrypsis_refs > 0);
    if (decrypsis_refs > 0) {
        DecryptProvider::Instance()->UpdateSession(session, session_size, license, license_size, host_primise_id);
    }
    else
        status = kPx_NotInitialized;
    return status;
}

PX_API PxStatus     DecryptCloseSession(const char* session, int session_size, SessionMessageCallback* session_cb)
{
    PxStatus status = kPx_Success;
    assert(decrypsis_refs > 0);
    if (decrypsis_refs > 0) {
        DecryptProvider::Instance()->CloseSession(session, session_size, session_cb);
    }
    else
        status = kPx_NotInitialized;
    return status;
}

PX_API PxStatus  RegisterPKInterceptor(PrivateKeyInterceptorCallback* pk) {
    PxStatus status = kPx_Success;
    assert(decrypsis_refs > 0);
    if (decrypsis_refs > 0) {
        DecryptProvider::Instance()->RegisterPKInterceptor(pk);
    }
    else
        status = kPx_NotInitialized;

    return status;
}
PX_API PxStatus  OnSessionMessagePK(const char* message, uint32_t message_size) {
    PxStatus status = kPx_Success;
    assert(decrypsis_refs > 0);
    if (decrypsis_refs > 0) {
        DecryptProvider::Instance()->OnSessionMessagePK(message, message_size);
    }
    else
        status = kPx_NotInitialized;

    return status;
}
