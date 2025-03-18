#include "decrypt_provider.h"

#include <iostream>
#include <algorithm>

#include "cdm_host.h"

DecryptProvider::DecryptProvider()
{
    std::cout << "DecryptProvider::Constructor" << std::endl;
    CdmHost::ExtractWideVine();
}

DecryptProvider::~DecryptProvider()
{
    std::cout << "DecryptProvider::Destructor" << std::endl;
}

AbstractDecryptService* DecryptProvider::GetService(const key_id_t& key_id)
{
    std::unique_lock<std::mutex> locker(mutex_);
    int keyCount = 0;
    for (auto service : services_) {
        Keys      key_storage;
        service->GetKeyIds(key_storage);
        for (auto key : key_storage.keys) {
            if (key_id == key.key_id) {
                return service;
            }
            keyCount++;
        }
    }

    std::cout << "::GetService key_id: " << std::string((const char*)key_id.data(), key_id.size()) << " not found! Totally " << keyCount << " keys in " << services_.size() << " services." << std::endl;
    return NULL;
}

AbstractDecryptService* DecryptProvider::WaitForService(const key_id_t& key_id, int timeout /*= -1*/)
{
    AbstractDecryptService* service = GetService(key_id);

    while (service == NULL) {
        if (WaitForService(timeout)) {
            service = GetService(key_id);
        }
        else
            break;
    }

    return service;
}

bool DecryptProvider::WaitForService(int timeout /*= -1*/)
{
    std::unique_lock<std::mutex> locker(mutex_);

    if (timeout < 0) {
        cond_var_.wait(locker);
        return true;
    }
    else if (timeout > 0) {
        return (std::cv_status::no_timeout == cond_var_.wait_for(locker, std::chrono::milliseconds(timeout)));
    }
    return true;
}

DecryptProvider* DecryptProvider::Instance()
{
    static DecryptProvider      provider;
    return &provider;
}

void DecryptProvider::AddService(AbstractDecryptService* service) {
        
    std::unique_lock<std::mutex> locker(mutex_);
    Keys      key_storage;
    service->GetKeyIds(key_storage);
    for (auto key_id : key_storage.keys) {
        std::cout << "key_id: " << std::string((const char*)key_id.key_id.data(), key_id.key_id.size()) << std::endl;
    }

    services_.push_back(service);
    
    cond_var_.notify_all();

    auto callbacks = callbacks_;
    locker.unlock();

    std::cout << "Decryptor callbacks count: " << callbacks_.size() << std::endl;
    //Callbacks must be free of mutex;
    for (auto callback : callbacks) {
        callback->OnNewService(service);
    }
}

void DecryptProvider::RemoveService(AbstractDecryptService* service)
{
    std::unique_lock<std::mutex> locker(mutex_);
    std::cout << "Remove service: " << std::endl;
    Keys      key_storage;
    service->GetKeyIds(key_storage);
    for (auto key_id : key_storage.keys) {
        std::cout << "key_id: " << std::string((const char*)key_id.key_id.data(), key_id.key_id.size()) << std::endl;
    }

    services_.erase(std::remove(services_.begin(), services_.end(), service), services_.end());
        
    cond_var_.notify_all();
    auto callbacks = callbacks_;
    locker.unlock();

    for (auto callback : callbacks) {
        callback->OnReleaseService(service);
    }
}


void DecryptProvider::DestroyAll()
{
    std::unique_lock<std::mutex> locker(mutex_);

    for (auto& service : services_) {
        service->Finish();
    }
    services_.clear();
    if (hosts_.size() > 0) {
        session_cbs_.clear();
        active_session_ids_.clear();
        for (auto& h : hosts_)
            h.second->Destroy();
    }
}

void DecryptProvider::RegisterCallback(ServiceCallback* callback) {
    std::unique_lock<std::mutex> locker(mutex_);
    callbacks_.push_back(callback);
}

void DecryptProvider::UnregisterCallback(ServiceCallback* callback) {
    std::unique_lock<std::mutex> locker(mutex_);
    for (auto it = callbacks_.begin(); it != callbacks_.end(); ++it) {
        if (*it == callback) {
            callbacks_.erase(it);
            break;
        }
    }
}

void DecryptProvider::CreateSessionAndGenerateRequest(const uint8_t* init_data, int init_data_size, SessionMessageCallback* session_cb, bool verify_cdm, int* host_promise_id)
{
    std::cout << "DecryptProvider::CreateSessionAndGenerateRequest ..." << std::endl;
    static const std::string key_system = "com.widevine.alpha";
    static const std::string host_name = "loc_host";
    static uintptr_t host_this = 0;

    promise_id_++; // increment on each new session

    CdmIpcAbstract* host = nullptr;
    if (host_promise_id != nullptr && 0 < *host_promise_id && *host_promise_id < promise_id_)
    {
        const auto it = hosts_.find(*host_promise_id);
        if (it != hosts_.end()) {
            host = it->second; // create session with existent service
            std::cout << "DecryptProvider new session with existent service " << *host_promise_id << ", host: " << host << std::endl;
        }
    }

    if (host == nullptr) {
        host = (CdmIpcAbstract*)CdmHost::CreateCdmIpcInstance(key_system, host_name, host_this, verify_cdm);
        host->Initialize(0, 0, 0);
        hosts_[promise_id_] = host;
        if (host_promise_id != NULL)
            *host_promise_id = promise_id_;
        std::cout << "DecryptProvider new service " << promise_id_ << ", host: " << host << std::endl;
    }

    session_cbs_[promise_id_] = session_cb;
    active_session_ids_[promise_id_] = "";

    //std::cout << "DecryptProvider::CreateSessionAndGenerateRequest promise_id:" << promise_id_ << std::endl;

    cdm::SessionType session_type = cdm::SessionType::kTemporary;
    cdm::InitDataType init_data_type = cdm::InitDataType::kCenc;
    BufferHolder buf(init_data, init_data_size);
    BufferHolder stor;
    host->CreateSessionAndGenerateRequest(promise_id_, session_type, init_data_type, buf, 0, stor);
    // ...OnSessionMessage
}

void DecryptProvider::OnResolveNewSessionPromise(uint32_t promise_id, const char* session_id, uint32_t session_id_size)
{
    const auto session = std::string(session_id, session_id_size);
    active_session_ids_[promise_id] = session;
    std::cout << "DecryptProvider::OnResolveNewSessionPromise promise_id:" << promise_id << " session: " << session << std::endl;
}

void DecryptProvider::OnSessionMessage(const char* session_id, uint32_t session_id_size, uint32_t message_type, const char* message, uint32_t message_size)
{
    uint32_t promise_id = GetPromiseIdBySession(session_id);
    SessionMessageCallback* session_cb_ = promise_id >0 ? session_cbs_[promise_id]:  NULL;
    
    if (session_cb_)
    {
        //if (active_session_id_.size() == 0 || active_session_id_ == session_id)
        {
            std::cout << "DecryptProvider::OnSessionMessage InterceptorCallback:" << session_cb_ << ", session:" << session_id << "promise: " << promise_id << std::endl;
            //active_session_id_ = session_id;
            session_cb_->OnSessionMessage(session_id, session_id_size, message_type, message, message_size);
        }
    }
}

void DecryptProvider::UpdateSession(const char* session, int session_size, const char* license, int license_size, int host_promise_id)
{
    if (hosts_.size() > 0) {
        uint32_t promise_id = GetPromiseIdBySession(session);

        BufferHolder session_buf(session, session_size);
        BufferHolder license_buf(license, license_size);

        auto& h = hosts_.find(host_promise_id < 0 ? promise_id : host_promise_id);
        std::cout << "DecryptProvider::UpdateSession :" << session << ", promise:" << promise_id << "/" << host_promise_id << ", service: " << h->second << std::endl;

        if (h != hosts_.end())
            h->second->UpdateSession(promise_id, session_buf, license_buf);
    }
}

void DecryptProvider::CloseSession(const char* session_id, int, SessionMessageCallback* session_cb)
{
    uint32_t promise_id = GetPromiseIdBySession(session_id);
    std::cout << "DecryptProvider::CloseSession " << session_id << " promise: " << promise_id << " ..." << std::endl;
    auto& h = hosts_.find(promise_id);
    if (h != hosts_.end()) {
        h->second->Destroy();
    }
    if (promise_id > 0) {
        session_cbs_[promise_id] = NULL;
        active_session_ids_[promise_id] = "";
    }
}

void  DecryptProvider::OnSessionKeysChange(const char* session_id, uint32_t session_id_size)
{
    uint32_t promise_id = GetPromiseIdBySession(session_id);
    const auto sess_it = session_cbs_.find(promise_id);

    if (sess_it != session_cbs_.end() && sess_it->second != nullptr) {
        std::cout << "OnSessionKeysChange callback " << sess_it->second << " session: " << session_id << std::endl;
        sess_it->second->OnSessionKeysChange();
    }
    else
        std::cout << "DecryptProvider not found session_cb for " << session_id << std::endl;
}


/**************** Private Key support ***********************/
bool DecryptProvider::IsPrivateKeyUsed() {
    return pk_cb_ != nullptr;
}
void DecryptProvider::RegisterPKInterceptor(PrivateKeyInterceptorCallback* cb) {
    pk_cb_ = cb;
}
void DecryptProvider::UnRegisterPKInterceptor(PrivateKeyInterceptorCallback* cb) {
    pk_cb_ = nullptr;
}
void DecryptProvider::SetServerCertificate(CdmHostForPrivateKeys* cdm_host, uint32_t promise_id, const uint8_t* cert_data, int cert_data_size) {
    if (pk_cb_) {
        pk_servercert_.assign(cert_data, cert_data+cert_data_size);
    }
}
void DecryptProvider::OnSessionCreatedPK(CdmHostForPrivateKeys* cdm_host, uint32_t promise_id, const uint8_t* init_data, int init_data_size) {
    if (pk_cb_) {
        pk_session_promise_id_ = promise_id;
        pk_cdm_host_ = cdm_host;
        // send pssh to PK Server
        pk_cb_->OnSessionCreated(pk_servercert_.data(), pk_servercert_.size(), init_data, init_data_size);
    }
}
void DecryptProvider::OnSessionMessagePK(/*const char* session_id, uint32_t session_id_size,*/ const char* message, uint32_t message_size) {
    if (pk_cdm_host_) {
        // send challenge to browser -> license server
        const auto session = active_session_ids_[pk_session_promise_id_];
        pk_cdm_host_->OnSessionMessageFromPK(session.data(), session.size(), message, message_size);
    }
}
void DecryptProvider::UpdateSessionPK(const char* session, int session_size, const char* license, int license_size, int host_promise_id) {
    if (pk_cb_) {
        // send license to PK server
        pk_cb_->UpdateSession(license, license_size); 
    }
}
