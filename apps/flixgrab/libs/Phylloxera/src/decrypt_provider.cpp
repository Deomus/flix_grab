#include "decrypt_provider.h"

#include <iostream>


DecryptProvider::DecryptProvider()
{
    std::cout << "DecryptProvider::Constructor" << std::endl;
}

DecryptProvider::~DecryptProvider()
{
    std::cout << "DecryptProvider::Destructor" << std::endl;
}

bool DecryptProvider::Start()
{
    messenger_.AddHandler(kMid_NewServer, std::bind(&DecryptProvider::NewService, this, std::placeholders::_1, std::placeholders::_2));
    messenger_.AddHandler(kMid_DeleteServer, std::bind(&DecryptProvider::DeleteService, this, std::placeholders::_1, std::placeholders::_2));
    messenger_.AddHandler(kMid_Error, std::bind(&DecryptProvider::ErrorService, this, std::placeholders::_1, std::placeholders::_2));

    return messenger_.StartServer();
}

DecryptService* DecryptProvider::GetService(const key_id_t& key_id)
{
    std::unique_lock<std::mutex> locker(mutex_);
    
    auto it = service_keys_.find(key_id);
    if (it != service_keys_.end())
        return services_[it->second].get();

    std::cout << "::GetService key_id: " << std::string((const char*)key_id.data(), key_id.size()) << " not found!" << std::endl;
    return NULL;
}

DecryptService* DecryptProvider::WaitForService(const key_id_t& key_id, int timeout /*= -1*/)
{
    DecryptService* service = GetService(key_id);

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

void DecryptProvider::Stop()
{
    //Send Finish Scope;
    messenger_.StopServer();
    callbacks_.clear();
}

DecryptProvider* DecryptProvider::Instance()
{
    static DecryptProvider      provider;
    return &provider;
}

void DecryptProvider::NewService(const std::string& name, const std::string&)
{
    std::unique_lock<std::mutex> locker(mutex_);
    Keys      key_storage;
    int service_id = services_.size();
    services_.push_back(std::make_unique<DecryptService>(name));

    if (services_.back()->Reset()) {
        services_.back()->GetKeyIds(key_storage);
        std::cout << "Message New Service: " << name << std::endl;

        for (auto key_id : key_storage.keys) {
            service_keys_.insert(std::make_pair(key_id.key_id, service_id));
            std::cout << "key_id: " << std::string((const char*)key_id.key_id.data(), key_id.key_id.size()) << std::endl;
        }

        //TODO: Callback;
        cond_var_.notify_all();

        for (auto callback : callbacks_) {
            callback->OnNewService(services_.back().get());
        }
    }
    else
        services_.pop_back();
}

void DecryptProvider::DeleteService(const std::string& name, const std::string&)
{
    std::unique_lock<std::mutex> locker(mutex_);
    std::cout << "Message Delete Service: " << name << std::endl;
    for (uint32_t service_id = 0; service_id < services_.size(); ++service_id) {
        if (services_[service_id] && services_[service_id]->name() == name) {

            for (auto it = service_keys_.begin(); it != service_keys_.end();) {
                if (it->second == (int)service_id)
                    service_keys_.erase(it++);
                else
                    it++;
            }

            //TODO: remove service;
            //services_[service_id].reset();

            //TODO: callback;
            cond_var_.notify_all();
            if (!callbacks_.empty())
                for (auto callback : callbacks_) {
                    callback->OnReleaseService(services_.back().get());
                }

            break;
        }
    }
}

void DecryptProvider::ErrorService(const std::string& name, const std::string& message)
{
    std::cout << "Service " << name << " Detected Error: " << message << std::endl;

    for (auto callback : callbacks_) {
        callback->OnError("Service '" + name + "' error", message);
    }
}

void DecryptProvider::DestroyAll()
{
    std::unique_lock<std::mutex> locker(mutex_);

    for (auto& service : services_) {
        service->Finish();
    }
    services_.clear();
    service_keys_.clear();
}
