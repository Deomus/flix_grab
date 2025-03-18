#include "host.h"

#include <chrono>
#include <ctime>

using namespace cdm;

HostProxy::HostProxy() : buffer_pooling_(new BufferPooling())
{
    std::cout << "Host::Constructor" << std::endl;
}

HostProxy::~HostProxy()
{
    std::cout << "Host::Destructor" << std::endl;
    //decrypt_server_.Stop();
}

Buffer* HostProxy::Allocate(uint32_t capacity)
{
    
    std::cout << "Host::Allocate capacity: " << capacity << std::endl;

    //TODO: remove

   return buffer_pooling_->Allocate(capacity);

  
}

void HostProxy::SetTimer(int64_t delay_ms, void* context)
{
     std::cout << "Host::SetTimer delay_ms: " << delay_ms << std::endl;
    
}

cdm::Time HostProxy::GetCurrentWallTime()
{
    cdm::Time time = GetNowTime();

    return time;
}

void HostProxy::OnResolveNewSessionPromise(uint32_t promise_id, const char* session_id, uint32_t session_id_size)
{
  
    std::cout << "Host::OnResolveNewSessionPromise promise_id: " << promise_id << " | session_id: " << std::string(session_id, session_id_size) << std::endl;

    std::ofstream session("session.bin", std::ios::binary);

    session.write(session_id, session_id_size);

}

void HostProxy::OnResolvePromise(uint32_t promise_id)
{
 
    std::cout << "Host::OnResolvePromise promise_id: " << promise_id << std::endl;
   
}

void HostProxy::OnRejectPromise(uint32_t promise_id, Error error, uint32_t system_code, const char* error_message, uint32_t error_message_size)
{
  
    std::cout << "Host::OnRejectPromise promise_id: " << promise_id << " | error: " << error <<
        " | system_code: " << system_code << " | error_message: " << std::string(error_message, error_message_size) << std::endl;

  
}

void HostProxy::OnSessionMessage(const char* session_id, uint32_t session_id_size, MessageType message_type, const char* message, uint32_t message_size, const char* legacy_destination_url, uint32_t legacy_destination_url_length)
{
 
    std::cout << "Host::OnSessionMessage session_id: " << std::string(session_id, session_id_size) <<
        " | message_type: " << GetMessageTypeName(message_type) << " | message: " << std::string(message, message_size) <<
        " | legacy_destination_url: " << std::string(legacy_destination_url, legacy_destination_url_length) << " | url_length: " << legacy_destination_url_length << std::endl;


    if (message_type == cdm::kLicenseRequest) {
        std::ofstream request_file("request.bin", std::ios::binary);

        request_file.write(message, message_size);
    }


}

void HostProxy::OnSessionKeysChange(const char* session_id, uint32_t session_id_size, bool has_additional_usable_key, const KeyInformation* keys_info, uint32_t keys_info_count)
{
   
    std::cout << "Host::OnSessionKeysChange session_id: " << std::string(session_id, session_id_size) <<
        " | has_additional_usable_key: " << has_additional_usable_key << " | keys_info_count: " << keys_info_count << std::endl;

    for (int i = 0; i < keys_info_count; ++i) {
        std::cout << " key_id: " << hexStr(keys_info[i].key_id, keys_info[i].key_id_size) << " | key_status: " << GetKeyStatusName(keys_info[i].status) << std::endl;
    }

    
}

void HostProxy::OnExpirationChange(const char* session_id, uint32_t session_id_size, Time new_expiry_time)
{
    std::cout << "Host::OnExpirationChange session_id: " << std::string(session_id, session_id_size) << " | new_expiry_time: Duration seconds " << new_expiry_time - GetNowTime() << std::endl;

}

void HostProxy::OnSessionClosed(const char* session_id, uint32_t session_id_size)
{
    std::cout << "Host::OnSessionClosed session_id: " << std::string(session_id, session_id_size) << std::endl;

}

void HostProxy::OnLegacySessionError(const char* session_id, uint32_t session_id_length, Error error, uint32_t system_code, const char* error_message, uint32_t error_message_length)
{
    std::cout << "Host::OnLegacySessionError session_id: " << std::string(session_id, session_id_length) <<
        " | error: " << error << " | system_code: " << system_code << " | error_message: " << std::string(error_message, error_message_length) << std::endl;
    
}

void HostProxy::SendPlatformChallenge(const char* service_id, uint32_t service_id_size, const char* challenge, uint32_t challenge_size)
{
    std::cout << "Host::SendPlatformChallenge service_id: " << std::string(service_id, service_id_size) << " | challenge: " << std::string(challenge, challenge_size) << std::endl;
    
}

void HostProxy::EnableOutputProtection(uint32_t desired_protection_mask)
{
    std::cout << "Host::EnableOutputProtection desired_protection_mask: " << desired_protection_mask << std::endl;

}

void HostProxy::QueryOutputProtectionStatus()
{
    std::cout << "Host::QueryOutputProtectionStatus: " << std::endl;

}

void HostProxy::OnDeferredInitializationDone(StreamType stream_type, Status decoder_status)
{
    std::cout << "Host::OnDeferredInitializationDone stream_type: " << GetStreamTypeName(stream_type) << " | decoder_status: " << GetStatusName(decoder_status) << std::endl;
    
}

FileIO* HostProxy::CreateFileIO(FileIOClient* client)
{
    std::cout << "Host::CreateFileIO client?" << std::endl;

    return nullptr;
}

//////////////////////////////////////////////////////////////////////////

cdm::Time HostProxy::GetNowTime() const
{
    auto time_point = std::chrono::system_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        time_point.time_since_epoch()).count();
    cdm::Time time = duration / 1000.0;
    return time;
}
