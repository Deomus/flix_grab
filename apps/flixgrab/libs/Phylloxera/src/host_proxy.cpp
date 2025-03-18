#include "host_proxy.h"
#include "video_frame_container.h"

#include <chrono>
#include <ctime>

using namespace cdm;

HostProxy::HostProxy(int id, cdm::ContentDecryptionModule::Host* host) : host_(host)
, id_(id)
, buffer_pooling_(new BufferPooling())
, current_time_(0)
, expiry_time_(0)
, is_cdm_destroyed_(false)
, is_server_finished_(false)
{
    decrypt_server_ = std::make_shared<DecryptServer>(this);
    std::cout << "[" << id_ << "] ";
    std::cout << "Host::Constructor" << std::endl;
}

HostProxy::~HostProxy()
{
    std::cout << "[" << id_ << "] ";
    std::cout << "Host::Destructor" << std::endl;
    //decrypt_server_.Stop();
}

int32_t HostProxy::InitializeVideoDecoder(const VideoConfig& config)
{
    cdm::VideoDecoderConfig cdm_config;

    cdm_config.codec = config.codec;
    cdm_config.profile = config.profile;
    cdm_config.format = config.format;

    cdm_config.coded_size.width = config.width;
    cdm_config.coded_size.height = config.height;
   // frame_rate_ = config.frame_rate;

    std::cout << "Framerate: " << config.frame_rate << std::endl;

    cdm_->DeinitializeDecoder(kStreamTypeVideo);
    cdm_->ResetDecoder(kStreamTypeVideo);

    //Initialize New Config;

    cdm::Status status = cdm_->InitializeVideoDecoder(cdm_config);

    std::cout << "[" << id_ << "] ";
    std::cout << "Cdm::InitializeVideoDecoder status: " << GetStatusName(status) << " config: " << GetVideoConfigName(cdm_config) << std::endl;


    return status;
}

int32_t HostProxy::DecodeVideo(const EncryptedData& encrypted, FrameSource_fn& frame_sink)
{
    //Prepare enccrypt buffer;
    InputBuffer  encrypt_buffer;
    encrypt_buffer.data = encrypted.buffer.data();
    encrypt_buffer.data_size = encrypted.buffer.size();
    //Key
    encrypt_buffer.key_id = encrypted.key_id.data();
    encrypt_buffer.key_id_size = encrypted.key_id.size();

    //Init Vector;
    encrypt_buffer.iv = encrypted.iv.data();
    encrypt_buffer.iv_size = encrypted.iv.size();

    encrypt_buffer.timestamp = encrypted.timestamp;
    encrypt_buffer.subsamples = encrypted.subsamples.data();
    encrypt_buffer.num_subsamples = encrypted.subsamples.size();

    VideoFrameContainer     frame;

    //Fix Overloading of CDM. Empirically CDM restrict process of 332 frames per 2 seconds;
    //Here changing speed of timeline for current cdm_id;
    // hook_->LockTimeline(id_, std::round(1000 / frame_rate_));

    //host_proxy_->IncreaseTime(1.0f / frame_rate_);
    IncreaseTime(1.0 / 100.0);

    Status status = cdm_->DecryptAndDecodeFrame(encrypt_buffer, &frame);
    //After fixing timeline we return timespeed to previous value;
    //  hook_->UnlockTimeline();

    if (status == kSuccess) {
        if (!frame.Commit(frame_sink))
            status = kDecodeError;
    }
    else {
        std::cout << "[" << id_ << "] " << "Cdm::DecryptAndDecodeFrame Video Status: " << GetStatusName(status) << " | timestamp: " << encrypt_buffer.timestamp << " | subsamples: " << encrypt_buffer.num_subsamples << " | data_size: " << encrypt_buffer.data_size
            << " | key_id: " << cdm::hexStr(encrypt_buffer.key_id, encrypt_buffer.key_id_size)
            << " | iv: " << cdm::hexStr(encrypt_buffer.iv, encrypt_buffer.iv_size) << std::endl;
    }


    return status;
  
}
class DecryptedBlockContaner : public cdm::DecryptedBlock {
public:
    virtual void SetDecryptedBuffer(Buffer* buffer) { buffer_ = buffer; }
    virtual Buffer* DecryptedBuffer() { return buffer_; }

    // TODO(tomfinegan): Figure out if timestamp is really needed. If it is not,
    // we can just pass Buffer pointers around.
    virtual void SetTimestamp(int64_t timestamp) { ts_ = timestamp; }
    virtual int64_t Timestamp() const { return ts_; }

    DecryptedBlockContaner() {}
    virtual ~DecryptedBlockContaner() {}

private:
    int64_t                 ts_;
    cdm::Buffer*            buffer_;
};

int32_t HostProxy::Decrypt(const EncryptedData& encrypted, DecryptedData& decrypted)
{
    //Prepare enccrypt buffer;
    InputBuffer  encrypt_buffer;
    std::vector<uint8_t> clear_buffer = encrypted.buffer;
    encrypt_buffer.data = clear_buffer.data();
    encrypt_buffer.data_size = clear_buffer.size();
    //Key
    encrypt_buffer.key_id = encrypted.key_id.data();
    encrypt_buffer.key_id_size = encrypted.key_id.size();

    //Init Vector;
    encrypt_buffer.iv = encrypted.iv.data();
    encrypt_buffer.iv_size = encrypted.iv.size();

    encrypt_buffer.timestamp = encrypted.timestamp;
    encrypt_buffer.subsamples = encrypted.subsamples.data();
    encrypt_buffer.num_subsamples = encrypted.subsamples.size();

    //Clear buffer;
    int data_pos = 0;
    for (auto subsample : encrypted.subsamples) {
        memset(&clear_buffer[data_pos], 0, subsample.clear_bytes);
        data_pos += subsample.cipher_bytes + subsample.clear_bytes;
    }

    DecryptedBlockContaner     block;

    //Fix Overloading of CDM. Empirically CDM restrict process of 332 frames per 2 seconds;
    //Here changing speed of timeline for current cdm_id;
    // hook_->LockTimeline(id_, std::round(1000 / frame_rate_));

    //host_proxy_->IncreaseTime(1.0f / frame_rate_);
    IncreaseTime(1.0 / 100.0);

    Status status = cdm_->Decrypt(encrypt_buffer, &block);
    //After fixing timeline we return timespeed to previous value;
    //  hook_->UnlockTimeline();
    decrypted.status = status;
    if (status == kSuccess) {
        
        //Fill buffer;
        cdm::Buffer* buffer = block.DecryptedBuffer();
        decrypted.buffer.assign(buffer->Data(), buffer->Data() + buffer->Size());
        int data_pos = 0;
        for (auto subsample : encrypted.subsamples) {
            memcpy(&decrypted.buffer[data_pos], &encrypted.buffer[data_pos], subsample.clear_bytes);
            data_pos += subsample.cipher_bytes + subsample.clear_bytes;
        }
        decrypted.timestamp = block.Timestamp();
    }
    else {
        std::cout << "[" << id_ << "] " << "Cdm::Decrypt Video Status: " << GetStatusName(status) << " | timestamp: " << encrypt_buffer.timestamp << " | subsamples: " << encrypt_buffer.num_subsamples << " | data_size: " << encrypt_buffer.data_size
            << " | key_id: " << cdm::hexStr(encrypt_buffer.key_id, encrypt_buffer.key_id_size)
            << " | iv: " << cdm::hexStr(encrypt_buffer.iv, encrypt_buffer.iv_size) << std::endl;
    }

    std::cout << "[" << id_ << "] " << "Cdm::Decrypt Video Status: " << GetStatusName(status) << " | timestamp: " << encrypt_buffer.timestamp << " | subsamples: " << encrypt_buffer.num_subsamples << " | data_size: " << encrypt_buffer.data_size
        << " | key_id: " << cdm::hexStr(encrypt_buffer.key_id, encrypt_buffer.key_id_size)
        << " | iv: " << cdm::hexStr(encrypt_buffer.iv, encrypt_buffer.iv_size) << std::endl;


    return status;
}


void HostProxy::GetKeyIds(Keys& keys)
{
    keys = key_ids_;
}

void HostProxy::Finish()
{
    std::cout << "Finishing Host..." << std::endl;
    is_server_finished_ = true;
    Release();
}

//////////////////////////////////////////////////////////////////////////

Buffer* HostProxy::Allocate(uint32_t capacity)
{
    /*std::cout << "[" << id_ << "] ";
    std::cout << "Host::Allocate capacity: " << capacity << std::endl;*/

    //TODO: remove

   return buffer_pooling_->Allocate(capacity);

   //COMMENT: Needs for hulu'
   Buffer* pBuffer = host_->Allocate(capacity);
   return pBuffer;
}

void HostProxy::SetTimer(int64_t delay_ms, void* context)
{
     std::cout << "Host::SetTimer delay_ms: " << delay_ms << std::endl;
    host_->SetTimer(delay_ms, context);
}

cdm::Time HostProxy::GetCurrentWallTime()
{
    cdm::Time time = current_time_;

    if (current_time_ == 0)
        time = GetNowTime();

    //TESTS:
    //Time time = host_->GetCurrentWallTime();

    //std::cout << "Host::GetCurrentWallTime " << time << " expire_seconds: " << expiry_time_ - time << std::endl;
  


    return time;
}

void HostProxy::OnResolveNewSessionPromise(uint32_t promise_id, const char* session_id, uint32_t session_id_size)
{
    std::cout << "[" << id_ << "] ";
    std::cout << "Host::OnResolveNewSessionPromise promise_id: " << promise_id << " | session_id: " << std::string(session_id, session_id_size) << std::endl;

    session_id_ = std::string(session_id, session_id_size);

    if (!is_cdm_destroyed_)
        host_->OnResolveNewSessionPromise(promise_id, session_id, session_id_size);
}

void HostProxy::OnResolvePromise(uint32_t promise_id)
{
    std::cout << "[" << id_ << "] ";
    std::cout << "Host::OnResolvePromise promise_id: " << promise_id << std::endl;
    if (!is_cdm_destroyed_)
        host_->OnResolvePromise(promise_id);
}

void HostProxy::OnRejectPromise(uint32_t promise_id, Error error, uint32_t system_code, const char* error_message, uint32_t error_message_size)
{
    std::cout << "[" << id_ << "] ";
    std::cout << "Host::OnRejectPromise promise_id: " << promise_id << " | error: " << error <<
        " | system_code: " << system_code << " | error_message: " << std::string(error_message, error_message_size) << std::endl;

    if (!is_cdm_destroyed_)
        host_->OnRejectPromise(promise_id, error, system_code, error_message, error_message_size);
}

void HostProxy::OnSessionMessage(const char* session_id, uint32_t session_id_size, MessageType message_type, const char* message, uint32_t message_size, const char* legacy_destination_url, uint32_t legacy_destination_url_length)
{
    std::cout << "[" << id_ << "] ";
    std::cout << "Host::OnSessionMessage session_id: " << std::string(session_id, session_id_size) <<
        " | message_type: " << GetMessageTypeName(message_type) << " | message: " << std::string(message, message_size) <<
        " | legacy_destination_url: " << std::string(legacy_destination_url, legacy_destination_url_length) << " | url_length: " << legacy_destination_url_length << std::endl;

    if (!is_cdm_destroyed_)
        host_->OnSessionMessage(session_id, session_id_size, message_type, message, message_size, legacy_destination_url, legacy_destination_url_length);
}

void HostProxy::OnSessionKeysChange(const char* session_id, uint32_t session_id_size, bool has_additional_usable_key, const KeyInformation* keys_info, uint32_t keys_info_count)
{
    std::cout << "[" << id_ << "] ";
    std::cout << "Host::OnSessionKeysChange session_id: " << std::string(session_id, session_id_size) <<
        " | has_additional_usable_key: " << has_additional_usable_key << " | keys_info_count: " << keys_info_count << std::endl;

    for (int i = 0; i < keys_info_count; ++i) {
        std::cout << " key_id: " << hexStr(keys_info[i].key_id, keys_info[i].key_id_size) << " | key_status: " << GetKeyStatusName(keys_info[i].status) << std::endl;
    }

    if (!is_cdm_destroyed_)
        host_->OnSessionKeysChange(session_id, session_id_size, has_additional_usable_key, keys_info, keys_info_count);

    key_ids_.keys.clear();

    for (uint32_t key_num = 0; key_num < keys_info_count; ++key_num) {
        key_ids_.keys.push_back(KeyInfo(keys_info[key_num]));
    }
}

void HostProxy::OnExpirationChange(const char* session_id, uint32_t session_id_size, Time new_expiry_time)
{
    std::cout << "[" << id_ << "] ";
    std::cout << "Host::OnExpirationChange session_id: " << std::string(session_id, session_id_size) << " | new_expiry_time: Duration seconds " << new_expiry_time - GetNowTime() << std::endl;

    expiry_time_ = new_expiry_time;
    current_time_ = GetNowTime();

    if (!is_cdm_destroyed_)
        host_->OnExpirationChange(session_id, session_id_size, new_expiry_time);
}

void HostProxy::OnSessionClosed(const char* session_id, uint32_t session_id_size)
{
    std::cout << "[" << id_ << "] ";
    std::cout << "Host::OnSessionClosed session_id: " << std::string(session_id, session_id_size) << std::endl;

    if (!is_cdm_destroyed_)
        host_->OnSessionClosed(session_id, session_id_size);
}

void HostProxy::OnLegacySessionError(const char* session_id, uint32_t session_id_length, Error error, uint32_t system_code, const char* error_message, uint32_t error_message_length)
{
    std::cout << "[" << id_ << "] ";
    std::cout << "Host::OnLegacySessionError session_id: " << std::string(session_id, session_id_length) <<
        " | error: " << error << " | system_code: " << system_code << " | error_message: " << std::string(error_message, error_message_length) << std::endl;
    if (!is_cdm_destroyed_)
        host_->OnLegacySessionError(session_id, session_id_length, error, system_code, error_message, error_message_length);
}

void HostProxy::SendPlatformChallenge(const char* service_id, uint32_t service_id_size, const char* challenge, uint32_t challenge_size)
{
    std::cout << "[" << id_ << "] ";
    std::cout << "Host::SendPlatformChallenge service_id: " << std::string(service_id, service_id_size) << " | challenge: " << std::string(challenge, challenge_size) << std::endl;
    if (!is_cdm_destroyed_)
        host_->SendPlatformChallenge(service_id, service_id_size, challenge, challenge_size);
}

void HostProxy::EnableOutputProtection(uint32_t desired_protection_mask)
{
    std::cout << "[" << id_ << "] ";
    std::cout << "Host::EnableOutputProtection desired_protection_mask: " << desired_protection_mask << std::endl;

    if (!is_cdm_destroyed_)
        host_->EnableOutputProtection(desired_protection_mask);
}

void HostProxy::QueryOutputProtectionStatus()
{
    std::cout << "[" << id_ << "] ";
    std::cout << "Host::QueryOutputProtectionStatus: " << std::endl;

    if (!is_cdm_destroyed_)
        host_->QueryOutputProtectionStatus();
}

void HostProxy::OnDeferredInitializationDone(StreamType stream_type, Status decoder_status)
{
    std::cout << "[" << id_ << "] ";
    std::cout << "Host::OnDeferredInitializationDone stream_type: " << GetStreamTypeName(stream_type) << " | decoder_status: " << GetStatusName(decoder_status) << std::endl;
    if (!is_cdm_destroyed_)
        host_->OnDeferredInitializationDone(stream_type, decoder_status);
}

FileIO* HostProxy::CreateFileIO(FileIOClient* client)
{
    std::cout << "[" << id_ << "] ";
    std::cout << "Host::CreateFileIO client?" << std::endl;

    FileIO* file = host_->CreateFileIO(client);
    
    FileIOProxy* file_proxy = new FileIOProxy(this, file, client);

    return file_proxy;
}

//////////////////////////////////////////////////////////////////////////

void HostProxy::Init(cdm::ContentDecryptionModule* cdm)
{
    cdm_ = cdm;
}

void HostProxy::ServiceReady()
{
    //std::this_thread::sleep_for(std::chrono::milliseconds(10000));
    //if ( !decrypt_server_ )
    decrypt_server_->Start(session_id_);
    
}

cdm::Time HostProxy::GetNowTime() const
{
    auto time_point = std::chrono::system_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        time_point.time_since_epoch()).count();
    cdm::Time time = duration / 1000.0;
    return time;
}

void HostProxy::Destroy()
{
    is_cdm_destroyed_ = true;
    Release();
}

void HostProxy::Release()
{
    
    if (is_cdm_destroyed_ && is_server_finished_)
    {           
        if (decrypt_server_) {
            decrypt_server_->Stop();
            decrypt_server_.reset();
        }

        if (cdm_) 
            cdm_->Destroy();
        
        delete this;
    }
}

