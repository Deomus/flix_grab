#pragma once
//#include "named_pipe.h"

#if defined(WIN32)

#if defined(PHYLLOXERA_IMPLEMENTATION)
#define PX_API __declspec(dllexport)
#else
#define PX_API __declspec(dllimport)
#endif  // defined(CDM_IMPLEMENTATION)

#else  // defined(WIN32)
#define PX_API __attribute__((visibility("default")))

#endif  // defined(WIN32)

#include "abstract_service.h"
#include "service_callback.h"
#include "decrypt_protocol.h"


extern "C"
{
    enum PxStatus {
        kPx_Success = 0,
        kPx_NotInitialized,
        kPx_InterprocessError,
        kPx_InternalError,
        kPx_Timeout,
    };

    PX_API const char*  GetPhylloxeraVersion();


    PX_API PxStatus     InstallPhylloxera();

    PX_API PxStatus     GetDecryptService(const uint8_t* key_id, int key_id_size, AbstractDecryptService** service, int timeout = 0);

    PX_API PxStatus     RegisterCallback(ServiceCallback* callback);
    PX_API PxStatus     UnregisterCallback(ServiceCallback* callback);

    PX_API PxStatus     UninstallPhylloxera();

    //Internal Funtion;
    PX_API bool         DetourPoint();

    //PX_API int TestDecrypt(void* pCdm, int trackid, const unsigned char* key_id, unsigned int key_id_size);
    //PX_API int TestRead();
}
