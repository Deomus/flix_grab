#pragma once

#include <string>
#include <vector>
#include <map>
#include <list>

#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

#include "decrypt_protocol.h"

#include "service_callback.h"

#include "cdm_ipc.h"

typedef std::vector<uint8_t>        key_id_t;

//Singleton;
class DecryptProvider
{
public:
    
    AbstractDecryptService*     GetService(const key_id_t& key_id);

    AbstractDecryptService*     WaitForService(const key_id_t& key_id, int timeout = -1);

    bool                        WaitForService(int timeout = -1);

    void                        RegisterCallback(ServiceCallback* callback);
    void                        UnregisterCallback(ServiceCallback* callback);
    void                        CreateSessionAndGenerateRequest(const uint8_t* init_data, int init_data_size, SessionMessageCallback* session_cb, bool verify_cdm, int* host_promise_id = nullptr);
    void                        OnResolveNewSessionPromise(uint32_t promise_id, const char* session_id, uint32_t session_id_size);
    void                        OnSessionMessage(const char* session_id, uint32_t session_id_size, uint32_t message_type, const char* message, uint32_t message_size);
    void                        UpdateSession(const char* session, int session_size, const char* license, int license_size, int host_promise_id);
    void                        CloseSession(const char* session, int session_size, SessionMessageCallback* session_cb);
    void                        OnSessionKeysChange(const char* session_id, uint32_t session_id_size);

    // Private Key support
    bool                        IsPrivateKeyUsed();
    void                        RegisterPKInterceptor(PrivateKeyInterceptorCallback* cb);
    void                        UnRegisterPKInterceptor(PrivateKeyInterceptorCallback* cb);
    void                        SetServerCertificate(CdmHostForPrivateKeys* cdm_host, uint32_t promise_id, const uint8_t* cert_data, int cert_data_size);
    void                        OnSessionCreatedPK(CdmHostForPrivateKeys* cdm_host, uint32_t promise_id, const uint8_t* init_data, int init_data_size);
    void                        OnSessionMessagePK(const char* message, uint32_t message_size);
    void                        UpdateSessionPK(const char* session, int session_size, const char* license, int license_size, int host_promise_id);

public:

    static DecryptProvider*     Instance();

    bool                        Start() {
        cdm_server_ = std::make_unique<ipc::Server>(DEFAULT_CDM_IPC, SERVER_TABLE(CdmTable));
        return cdm_server_->status().Ok();
    }
    void                        Stop() {
        if (cdm_server_)
            cdm_server_->Cancel();
    }

public:
    void                        AddService(AbstractDecryptService*);
    void                        RemoveService(AbstractDecryptService*);
    void                        DestroyAll();

private:
    DecryptProvider();
    ~DecryptProvider();

    uint32_t GetPromiseIdBySession(const char* session) {
        for (const auto kv : active_session_ids_)
            if (kv.second == session)
                return kv.first;
        return 0; // no such session
    }


private:
            
    std::vector<AbstractDecryptService*>                services_;

    std::mutex                                          mutex_;
    std::condition_variable                             cond_var_;

    std::list<ServiceCallback*>                         callbacks_;
    std::map<uint32_t, SessionMessageCallback*>        session_cbs_; // promise -> session callback
    std::map<uint32_t, std::string>                     active_session_ids_; // promise -> session_id
    std::map<uint32_t, CdmIpcAbstract*>                 hosts_;
    uint32_t                                            promise_id_ = 1; // increment on each new session

    // private key server callback
    PrivateKeyInterceptorCallback*                      pk_cb_ = nullptr;
    CdmHostForPrivateKeys*                              pk_cdm_host_ = nullptr;
    uint32_t                                            pk_session_promise_id_;
    std::vector<uint8_t>                                pk_servercert_;

private:
//TODO: remove
    std::unique_ptr<ipc::Server>                        cdm_server_;
    
};

