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
#include "messenger.h"

#include "decrypt_service.h"
#include "service_callback.h"

#define PIPE_MESSAGE_NAME               "decrypt_service_messages"

//Singleton;
class DecryptProvider
{
public:
    bool                        Start();

    DecryptService*             GetService(const key_id_t& key_id);

    DecryptService*             WaitForService(const key_id_t& key_id, int timeout = -1);

    bool                        WaitForService(int timeout = -1);

    void                        RegisterCallback(ServiceCallback* callback) {

        callbacks_.push_back(callback);

    }
    void                        UnregisterCallback(ServiceCallback* callback)
    {
        for (auto it = callbacks_.begin(); it != callbacks_.end(); ++it) {
            if (*it == callback) {
                callbacks_.erase(it);
                break;
            }
        }
    }

    void                        Stop();

public:

    static DecryptProvider*     Instance();

private:
    void                        NewService(const std::string& name, const std::string& );
    void                        DeleteService(const std::string& name, const std::string& );
    void                        ErrorService(const std::string& name, const std::string& message);
    void                        DestroyAll();

private:
    DecryptProvider();
    ~DecryptProvider();


private:
    Messenger                                           messenger_;
        
    std::vector<std::unique_ptr<DecryptService>>        services_;
    std::map<key_id_t, int>                             service_keys_;

    std::mutex                                          mutex_;
    std::condition_variable                             cond_var_;

    std::list<ServiceCallback*>                         callbacks_;
    
};

