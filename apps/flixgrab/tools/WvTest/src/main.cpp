
#include "host.h"
#include "content_decryption_module.h"
#include <windows.h>


HostProxy* host = NULL;

void* GetCdmHost(int host_interface_version, void* user_data) {
    
    host = new HostProxy();
    return host;
}


typedef void* (*CreateCdmInstanceFunc)(
    int cdm_interface_version,
    const char* key_system, uint32_t key_system_size,
    GetCdmHostFunc get_cdm_host_func, void* user_data);

typedef void(*InitializeCdmFunc)();
typedef void(*DeinitializeCdmFunc)();

int main(int argc, char *argv[])
{
    HMODULE hm = ::LoadLibraryA("widevinecdm.dll");

    if (hm == NULL)
        return -1;

    InitializeCdmFunc initCdm = (InitializeCdmFunc)::GetProcAddress(hm, "InitializeCdmModule_4");
    DeinitializeCdmFunc deinitCdm = (DeinitializeCdmFunc)::GetProcAddress(hm, "DeinitializeCdmModule");
    CreateCdmInstanceFunc createCdm = (CreateCdmInstanceFunc)::GetProcAddress(hm, "CreateCdmInstance");

    initCdm();

    std::string key_system = "com.widevine.alpha";
    cdm::ContentDecryptionModule* cdm = (cdm::ContentDecryptionModule*)
        createCdm(cdm::ContentDecryptionModule::kVersion, key_system.data(), key_system.size(), GetCdmHost, NULL);

    cdm->Initialize(false, false);
    
    std::ifstream lic_file("lic.bin", std::ios::binary);

    std::vector<char> lic_buffer((
        std::istreambuf_iterator<char>(lic_file)),
        (std::istreambuf_iterator<char>()));
    
    cdm->SetServerCertificate(1, (uint8_t*)lic_buffer.data(), lic_buffer.size());


    std::ifstream init_file("init.bin", std::ios::binary);

    std::vector<char> init_buffer((
        std::istreambuf_iterator<char>(init_file)),
        (std::istreambuf_iterator<char>()));

    cdm->CreateSessionAndGenerateRequest(2, cdm::kTemporary, cdm::kCenc, (uint8_t*)init_buffer.data(), init_buffer.size());

    std::string session = host->GetSession();

    std::ifstream response_file("response.bin", std::ios::binary);
    std::vector<char> response_buffer((
        std::istreambuf_iterator<char>(response_file)),
        (std::istreambuf_iterator<char>()));

    cdm->UpdateSession(3, session.data(), session.size(), (uint8_t*)response_buffer.data(), response_buffer.size());


    cdm->Destroy();

    deinitCdm();
    

    return 0;
}
