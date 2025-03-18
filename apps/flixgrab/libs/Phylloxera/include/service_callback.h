#pragma once

#include <string>
#include <cstdint>
#include "abstract_service.h"

class ServiceCallback
{
public:
    virtual void                OnNewService(AbstractDecryptService* service) = 0;
    virtual void                OnReleaseService(AbstractDecryptService* service) = 0;
    virtual void                OnError(const std::string& status, const std::string& message) = 0;
    virtual void                OnRemove() = 0;
    
protected:

    virtual ~ServiceCallback() {}


};
