#include "cdm_host.h"
#include "cdm_ipc.h"

#include "file_io.h"

#include "video_frame_container.h"

#include <chrono>
#include <ctime>

#include "decrypt_provider.h"

#include "resource\resource.h"

using namespace cdm;

std::mutex      CdmHost::lock_;

//#define DECRYPT_AND_DECODE_AUDIO

// host_this = address of HostIpcAbstract* 
// returns address of CdmIpcAbstract 
uintptr_t CdmHost::CreateCdmIpcInstance(const std::string& key_system, const std::string& host_name, uintptr_t host_this, bool verify_cdm) {
    std::cout << "CreateCdmIpcInstance: " << key_system << std::endl;
    CdmHost* host = new CdmHost(key_system, host_name, host_this, verify_cdm);
    return reinterpret_cast<uintptr_t>(static_cast<CdmIpcAbstract*>(host));
}

CdmHost::CdmHost(const std::string& key_system, const std::string& host_name, uintptr_t host_this, bool verify_cdm)
    : host_client_(host_name)
    , host_this_(host_this)
    , buffer_pooling_(new BufferPooling())
    , current_time_(0)
    , expiry_time_(0) {
   
    std::cout << "[" << host_this_ << "] " << "Host::Constructor, verify_cdm:" << verify_cdm << std::endl;
    is_ipc_host_valid_ = (host_this != 0);
    InitializeInternal(key_system, verify_cdm);
}

CdmHost::~CdmHost() {
    std::cout << "[" << host_this_ << "] ";
    std::cout << "Host::Destructor" << std::endl;
    //decrypt_server_.Stop();
}

int32_t CdmHost::InitializeVideoDecoder(const VideoConfig& config) {
    VideoDecoderConfig cdm_config;

    cdm_config.codec = config.codec;
    cdm_config.profile = config.profile;
    cdm_config.format = config.format;

    cdm_config.coded_size.width = config.width;
    cdm_config.coded_size.height = config.height;
    cdm_config.extra_data_size = 0; 
    cdm_config.extra_data = NULL; // new in cdm 10
   // frame_rate_ = config.frame_rate;

    std::cout << "Framerate: " << config.frame_rate << std::endl;

    //cdm_->DeinitializeDecoder(kStreamTypeVideo);
    //cdm_->ResetDecoder(kStreamTypeVideo);

    //Initialize New Config;

    cdm::Status status = cdm_->InitializeVideoDecoder(cdm_config);

   
    std::cout << "Cdm::InitializeVideoDecoder status: " << GetStatusName(status) << " config: " << GetVideoConfigName(cdm_config) << std::endl;
    
    cdm_->ResetDecoder(kStreamTypeVideo);
    std::cout << "ResetDecoder decoder_type = " << GetStreamTypeName(kStreamTypeVideo) << std::endl;

    return status;
}

int32_t CdmHost::DecodeVideo(const EncryptedData& encrypted, FrameSource_fn& frame_sink)
{
    std::unique_lock<std::mutex> lock(lock_);
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

    encrypt_buffer.encryption_scheme = cdm::EncryptionScheme::kCenc; // 1. todo: ?
    encrypt_buffer.pattern = {0,0}; // todo: CBCS EncryptionScheme
    //totalDataSize += encrypt_buffer.data_size;

    VideoFrameContainer     frame;

    //Fix Overloading of CDM. Empirically CDM restrict process of 332 frames per 2 seconds;
    //Here changing speed of timeline for current cdm_id;
    // hook_->LockTimeline(id_, std::round(1000 / frame_rate_));

    //host_proxy_->IncreaseTime(1.0f / frame_rate_);
    IncreaseTime(1.0 / 100.0);
    
    Status status = cdm::kNeedMoreData;
    for (int retry = 0; retry < 120 && cdm_ != NULL; retry++) {
        status = cdm_->DecryptAndDecodeFrame(encrypt_buffer, &frame);
        if (status != cdm::kNoKey)
            break;
        nokeys_detected_ = true;
        // do pray and wait
        //while (nokeys_detected_ && cdm_ != NULL) {
        //    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        //}
    }
    nokeys_detected_ = false;

    //After fixing timeline we return timespeed to previous value;
    //  hook_->UnlockTimeline();
    
    /*if (status == kNoKey) {
        DecryptProvider::Instance()->OnSessionKeysChange(NULL, 0);
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(10s);
        status = cdm_->DecryptAndDecodeFrame(encrypt_buffer, &frame);
    }*/
    if (status == kSuccess) {
        if (!frame.Commit(frame_sink))
            status = kDecodeError;
    }
    if (status != kSuccess) {
        auto b = encrypt_buffer;
        std::cout << "[" << host_this_ << "] " << "Cdm::DecryptAndDecodeFrame Video Status: " << GetStatusName(status)
            << ", data_size:" << encrypt_buffer.data_size << ", encryption_scheme:" << (uint32_t)encrypt_buffer.encryption_scheme
            << " | key_id: " << cdm::hexStr(encrypt_buffer.key_id, encrypt_buffer.key_id_size)
            << " | iv: " << cdm::hexStr(encrypt_buffer.iv, encrypt_buffer.iv_size) << std::endl;
        if (b.subsamples)
            std::cout << ",subsamples:" << b.subsamples->cipher_bytes << "," << b.subsamples->clear_bytes;
        //std::cout << ",num_subsamples:" << b.num_subsamples << ",pattern:" << b.pattern.crypt_byte_block << "," << b.pattern.skip_byte_block << ",timestamp:" << b.timestamp 
            //<< "totalDataSize: +" << encrypt_buffer.data_size << " = " << totalDataSize
        std::cout << std::endl;
        //std::cout << "VideoFrame:" << frame.Format() << "," << frame.Size().width << "*" << frame.Size().height << "," << frame.Timestamp() << std::endl; // << "," << f->Stride() << "," << f->PlaneOffset();
    }

    return status;
  
}

int32_t CdmHost::InitializeAudioDecoder(int codec, int channel_count, int bits_per_channel, int samples_per_second, std::vector<uint8_t> extra_data) {
    AudioDecoderConfig cdm_config;
    // convert media::AudioCodec => cdm::AudioCodec
    cdm_config.codec = codec == 1? cdm::AudioCodec::kCodecAac: 
        codec == 4? cdm::AudioCodec::kCodecVorbis: 
        cdm::AudioCodec::kUnknownAudioCodec;
    cdm_config.channel_count = channel_count;
    cdm_config.bits_per_channel = bits_per_channel;
    cdm_config.samples_per_second = samples_per_second;

    cdm_config.extra_data_size = extra_data.size();
    cdm_config.extra_data = const_cast<uint8_t*>(extra_data.data()); // new in cdm 10

    cdm_config.encryption_scheme = EncryptionScheme::kCenc;

    auto status = cdm::Status::kSuccess;

#ifdef DECRYPT_AND_DECODE_AUDIO
    //cdm_->DeinitializeDecoder(kStreamTypeAudio);
    //cdm_->ResetDecoder(kStreamTypeAudio);
    status = cdm_->InitializeAudioDecoder(cdm_config);

    std::cout << "Cdm::InitializeAudioDecoder status: " << GetStatusName(status) << " config: " << GetAudioConfigName(cdm_config) << std::endl;

    cdm_->ResetDecoder(kStreamTypeAudio);
//    std::cout << "ResetDecoder decoder_type = " << GetStreamTypeName(kStreamTypeAudio) << std::endl;
#endif
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

class AudioFramesContaner : public cdm::AudioFrames {
public:
    virtual void SetFrameBuffer(Buffer* buffer) override { buffer_ = buffer; }
    virtual Buffer* FrameBuffer() override { return buffer_; }

    virtual void SetFormat(AudioFormat format) override { format_ = format; };
    virtual AudioFormat Format() const override { return format_; };

    AudioFramesContaner() {}
    virtual ~AudioFramesContaner() {}

    Buffer* DecryptedBuffer() {
        return FrameBuffer();
    }
private:
    AudioFormat format_ = AudioFormat::kUnknownAudioFormat;
    cdm::Buffer* buffer_ = NULL;
};


int32_t CdmHost::Decrypt(const EncryptedData& encrypted, DecryptedData& decrypted)
{
    std::unique_lock<std::mutex> lock(lock_);

    //Prepare enccrypt buffer;
    InputBuffer  encrypt_buffer;
    std::vector<uint8_t> clear_buffer = encrypted.buffer;
    encrypt_buffer.data = clear_buffer.data();
    encrypt_buffer.data_size = clear_buffer.size();
    //Key
    //std::vector<uint8_t> k = { 0xE9,0xFA,0x50,0x8B,0x83,0xCF,0xBB,0xDD,0x8D,0xE7,0xCB,0xFE,0xFD,0xB7,0x3B,0xA4 };
    encrypt_buffer.key_id = encrypted.key_id.data(); // k.data(); // 
    encrypt_buffer.key_id_size = encrypted.key_id.size(); // k.size(); //

    //Init Vector;
    encrypt_buffer.iv = encrypted.iv.data();
    encrypt_buffer.iv_size = encrypted.iv.size();

    encrypt_buffer.timestamp = encrypted.timestamp;
    encrypt_buffer.subsamples = encrypted.subsamples.data();
    encrypt_buffer.num_subsamples = encrypted.subsamples.size();

    encrypt_buffer.encryption_scheme = cdm::EncryptionScheme::kCenc; // 1. todo: ?
    encrypt_buffer.pattern = { 0,0 }; // todo: ?

    //Clear buffer;
    int data_pos = 0;
    for (auto subsample : encrypted.subsamples) {
        memset(&clear_buffer[data_pos], 0, subsample.clear_bytes);
        data_pos += subsample.cipher_bytes + subsample.clear_bytes;
    }

#ifdef DECRYPT_AND_DECODE_AUDIO
    AudioFramesContaner block;
#else
    DecryptedBlockContaner     block;
#endif

    //Fix Overloading of CDM. Empirically CDM restrict process of 332 frames per 2 seconds;
    //Here changing speed of timeline for current cdm_id;
    // hook_->LockTimeline(id_, std::round(1000 / frame_rate_));

    //host_proxy_->IncreaseTime(1.0f / frame_rate_);
    IncreaseTime(1.0 / 100.0);

    Status status = cdm::kNeedMoreData;
    for (int retry = 0; retry < 120 && cdm_ != NULL; retry++) {
#ifdef DECRYPT_AND_DECODE_AUDIO
        status = cdm_->DecryptAndDecodeSamples(encrypt_buffer, &block);
#else
        //std::cout << " encr k: " << hexStr(encrypted.key_id.data(), encrypted.key_id.size()) << ": " << hexStr(encrypt_buffer.data, encrypt_buffer.data_size) << std::endl;
        status = cdm_->Decrypt(encrypt_buffer, &block);
        //std::cout << " decr: " << hexStr(block.DecryptedBuffer()->Data(), block.DecryptedBuffer()->Size()) << std::endl;
#endif

        if (status == cdm::kNoKey) {
            nokeys_detected_ = true;
            // do pray and wait
            /*while (nokeys_detected_ && cdm_ != NULL) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }
            continue;*/
        }
        nokeys_detected_ = false;
        break;
    }
    

    //After fixing timeline we return timespeed to previous value;
    //  hook_->UnlockTimeline();
    decrypted.status = status;
    if (status == kSuccess) {
        
        //Fill buffer;
        cdm::Buffer* buffer = block.DecryptedBuffer();
        decrypted.buffer.assign(buffer->Data(), buffer->Data() + buffer->Size());
        int pos = 0;
        for (auto subsample : encrypted.subsamples) {
            memcpy(&decrypted.buffer[pos], &encrypted.buffer[pos], subsample.clear_bytes);
            pos += subsample.cipher_bytes + subsample.clear_bytes;
        }
        decrypted.timestamp = encrypted.timestamp; // block.Timestamp();
        buffer->Destroy();
    }
    else {
        std::cout << "[" << host_this_ << "] " << "Cdm::Decrypt Status: " << GetStatusName(status) << " | timestamp: " << encrypt_buffer.timestamp << " | subsamples: " << encrypt_buffer.num_subsamples << " | data_size: " << encrypt_buffer.data_size
            << " | key_id: " << cdm::hexStr(encrypt_buffer.key_id, encrypt_buffer.key_id_size)
            << " | iv: " << cdm::hexStr(encrypt_buffer.iv, encrypt_buffer.iv_size) << std::endl;
    }

    //std::cout << "[" << host_this_ << "] " << "Cdm::Decrypt Status: " << GetStatusName(status) << " | timestamp: " << encrypt_buffer.timestamp << " | subsamples: " << encrypt_buffer.num_subsamples << " | data_size: " << encrypt_buffer.data_size
    //    << " | key_id: " << cdm::hexStr(encrypt_buffer.key_id, encrypt_buffer.key_id_size)
    //    << " | iv: " << cdm::hexStr(encrypt_buffer.iv, encrypt_buffer.iv_size) << std::endl;


    return status;
}


void CdmHost::GetKeyIds(Keys& keys)
{
    keys = key_ids_;
}

void CdmHost::Finish()
{
    std::cout << "Finishing Host..." << std::endl;
    
    DecryptProvider::Instance()->RemoveService(this);
    //Destroy if browser destroyed cdm or destroy when browser requested;
    if (destroy_flag_.test_and_set()) {
        std::cout << "Cdm::Destroy Handle from host" << std::endl;
        cdm_->Destroy();
        cdm_ = NULL;
        delete this;
    }
}

bool  CdmHost::KeysOK()
{
    return !nokeys_detected_;
}

//////////////////////////////////////////////////////////////////////////

Buffer* CdmHost::Allocate(uint32_t capacity)
{
    //std::cout << "[" << host_this_ << "] ";
    //std::cout << "Host::Allocate capacity: " << capacity << std::endl;

    //TODO: remove
   return buffer_pooling_->Allocate(capacity);

   //COMMENT: Needs for hulu'
   /*auto pBuffer = cdm_->Allocate(capacity);
   return pBuffer;*/
}

void CdmHost::SetTimer(int64_t , void* )
{
     //std::cout << "Host::SetTimer delay_ms: " << delay_ms << std::endl;
    
}

cdm::Time CdmHost::GetCurrentWallTime()
{
    cdm::Time time = current_time_;

    if (current_time_ == 0)
        time = GetNowTime();

    //TESTS:
    //Time time = host_->GetCurrentWallTime();

    //std::cout << "Host::GetCurrentWallTime " << time << " expire_seconds: " << expiry_time_ - time << std::endl;
    return time;
}

void CdmHost::OnInitialized(bool success)
{
    std::cout << "[" << host_this_ << "] " << "RemoteHost::OnInitialized success: " << success << " ..." << std::endl;
    if (is_ipc_host_valid_)
        INVOKE_REMOTE(&host_client_, HostTable, OnInitialized, host_this_, success);
    std::cout << "[" << host_this_ << "] " << "RemoteHost::OnInitialized done."  << std::endl;
}
void CdmHost::OnResolveKeyStatusPromise(uint32_t promise_id,
    cdm::KeyStatus key_status) {
    std::cout << "[" << host_this_ << "] ";
    std::cout << "Host::OnResolveKeyStatusPromise promise_id: " << promise_id << " | key_status: " << key_status << std::endl;
    if (is_ipc_host_valid_)
        INVOKE_REMOTE(&host_client_, HostTable, OnResolveKeyStatusPromise, host_this_, promise_id, key_status);
}
void CdmHost::OnResolveNewSessionPromise(uint32_t promise_id, const char* session_id, uint32_t session_id_size)
{
    std::cout << "[" << host_this_ << "] ";
    std::cout << "Host::OnResolveNewSessionPromise promise_id: " << promise_id << " | session_id: " << std::string(session_id, session_id_size) << std::endl;
    if (is_ipc_host_valid_)
        INVOKE_REMOTE(&host_client_, HostTable, OnResolveNewSessionPromise, host_this_, promise_id, BufferHolder(session_id, session_id_size));
    DecryptProvider::Instance()->OnResolveNewSessionPromise(promise_id, session_id, session_id_size);
}

void CdmHost::OnResolvePromise(uint32_t promise_id)
{
    std::cout << "[" << host_this_ << "] ";
    std::cout << "Host::OnResolvePromise promise_id: " << promise_id << std::endl;
    if (is_ipc_host_valid_)
        INVOKE_REMOTE(&host_client_, HostTable, OnResolvePromise, host_this_, promise_id);
    std::cout << "Host::OnResolvePromise done. " << std::endl;
}

void CdmHost::OnRejectPromise(uint32_t promise_id, Exception exception, uint32_t system_code, const char* error_message, uint32_t error_message_size)
{
    std::cout << "[" << host_this_ << "] ";
    std::cout << "Host::OnRejectPromise promise_id: " << promise_id << " | exception: " << exception <<
        " | system_code: " << system_code << " | error_message: " << error_message_size << std::string(error_message, error_message_size) << std::endl;
    if (is_ipc_host_valid_)
        INVOKE_REMOTE(&host_client_, HostTable, OnRejectPromise, host_this_, promise_id, exception, system_code, BufferHolder(error_message, error_message_size));
}
// , const char* legacy_destination_url, uint32_t legacy_destination_url_length
void CdmHost::OnSessionMessage(const char* session_id, uint32_t session_id_size, MessageType message_type, const char* message, uint32_t message_size)
{
    std::cout << "[" << host_this_ << "] ";   
    std::cout << "Host::OnSessionMessage session_id: " << std::string(session_id, session_id_size) << // std::string(message, message_size) <<
        " | message_type: " << GetMessageTypeName(message_type) << ", msg size: " << message_size << std::endl;

    if (is_ipc_host_valid_ && !DecryptProvider::Instance()->IsPrivateKeyUsed())
        INVOKE_REMOTE(&host_client_, HostTable, OnSessionMessage, host_this_, BufferHolder(session_id, session_id_size), message_type,
            BufferHolder(message, message_size)); //, BufferHolder(legacy_destination_url, legacy_destination_url_length)
    else
        std::cout << " Skip" << std::endl;
    DecryptProvider::Instance()->OnSessionMessage(session_id, session_id_size, message_type, message, message_size);
}

void CdmHost::OnSessionKeysChange(const char* session_id, uint32_t session_id_size, bool has_additional_usable_key, const KeyInformation* keys_info, uint32_t keys_info_count)
{
    std::cout << "[" << host_this_ << "] ";
    std::cout << "Host::OnSessionKeysChange session_id: " << std::string(session_id, session_id_size) <<
        " | has_additional_usable_key: " << has_additional_usable_key << " | keys_info_count: " << keys_info_count << std::endl;

    for (uint32_t i = 0; i < keys_info_count; ++i) {
        std::cout << " key_id: " << hexStr(keys_info[i].key_id, keys_info[i].key_id_size) << " | key_status: " << GetKeyStatusName(keys_info[i].status) << " | syscode: " << keys_info[i].system_code << std::endl;
    }

    if (is_ipc_host_valid_ && has_additional_usable_key) {

        for (uint32_t i = 0; i < keys_info_count; ++i) {
            INVOKE_REMOTE(&host_client_, HostTable, OnSessionKeyChange, host_this_, BufferHolder(session_id, session_id_size),
                BufferHolder(keys_info[i].key_id, keys_info[i].key_id_size), keys_info[i].status, keys_info[i].system_code);
        }
    }

    key_ids_.keys.clear();

    for (uint32_t key_num = 0; key_num < keys_info_count; ++key_num) {
        key_ids_.keys.push_back(KeyInfo(keys_info[key_num]));
    }
    //TODO: block host calls from this point
    is_ipc_host_valid_ = false;
    DecryptProvider::Instance()->AddService(this);
    DecryptProvider::Instance()->OnSessionKeysChange(session_id, session_id_size);
}

void CdmHost::OnExpirationChange(const char* session_id, uint32_t session_id_size, Time new_expiry_time)
{
    std::cout << "[" << host_this_ << "] ";
    std::cout << "Host::OnExpirationChange session_id: " << std::string(session_id, session_id_size) << " | new_expiry_time: Duration seconds " << new_expiry_time - GetNowTime() << std::endl;

    expiry_time_ = new_expiry_time;
    current_time_ = GetNowTime();
    if (is_ipc_host_valid_)
        INVOKE_REMOTE(&host_client_, HostTable, OnExpirationChange, host_this_, BufferHolder(session_id, session_id_size), new_expiry_time);
 
}

void CdmHost::OnSessionClosed(const char* session_id, uint32_t session_id_size)
{
    //std::cout << "[" << host_this_ << "] ";
    //std::cout << "Host::OnSessionClosed session_id: " << std::string(session_id, session_id_size) << std::endl;
    if (is_ipc_host_valid_)
        INVOKE_REMOTE(&host_client_, HostTable, OnSessionClosed, host_this_, BufferHolder(session_id, session_id_size));
      
}
// deprecated
//void CdmHost::OnLegacySessionError(const char* session_id, uint32_t session_id_length, Exception exception, uint32_t system_code, const char* error_message, uint32_t error_message_length)
//{
//    //std::cout << "[" << host_this_ << "] ";
//    //std::cout << "Host::OnLegacySessionError session_id: " << std::string(session_id, session_id_length) <<
//    //    " | error: " << error << " | system_code: " << system_code << " | error_message: " << std::string(error_message, error_message_length) << std::endl;
//
//    //if (is_ipc_host_valid_)
//    //    INVOKE_REMOTE(&host_client_, HostTable, OnLegacySessionError, host_this_, BufferHolder(session_id, session_id_length), exception, system_code, BufferHolder(error_message, error_message_length));
//    //    
//
//}

void CdmHost::SendPlatformChallenge(const char* service_id, uint32_t service_id_size, const char* challenge, uint32_t challenge_size)
{
    std::cout << "[" << host_this_ << "] ";
    std::cout << "Host::SendPlatformChallenge service_id: " << std::string(service_id, service_id_size) << " | challenge: " << std::string(challenge, challenge_size) << std::endl;
    if (is_ipc_host_valid_)
        INVOKE_REMOTE(&host_client_, HostTable, SendPlatformChallenge, host_this_, BufferHolder(service_id, service_id_size), BufferHolder(challenge, challenge_size));

}

void CdmHost::EnableOutputProtection(uint32_t desired_protection_mask)
{
    std::cout << "[" << host_this_ << "] ";
    std::cout << "Host::EnableOutputProtection desired_protection_mask: " << desired_protection_mask << std::endl;
    if (is_ipc_host_valid_)
        INVOKE_REMOTE(&host_client_, HostTable, EnableOutputProtection, host_this_, desired_protection_mask);
}

void CdmHost::QueryOutputProtectionStatus()
{
    std::cout << "[" << host_this_ << "] ";
    std::cout << "Host::QueryOutputProtectionStatus: " << std::endl;

    // notify cdm that hdcp is ok:
    // Called by the host after a call to Host::QueryOutputProtectionStatus().
    cdm_->OnQueryOutputProtectionStatus(QueryResult::kQuerySucceeded, OutputLinkTypes::kLinkTypeHDMI, OutputProtectionMethods::kProtectionHDCP);
}

void CdmHost::OnDeferredInitializationDone(StreamType stream_type, Status decoder_status)
{
    std::cout << "[" << host_this_ << "] ";
    std::cout << "Host::OnDeferredInitializationDone stream_type: " << GetStreamTypeName(stream_type) << " | decoder_status: " << GetStatusName(decoder_status) << std::endl;
  
}

FileIO* CdmHost::CreateFileIO(FileIOClient* client)
{
    //std::cout << "[" << host_this_ << "] ";
    std::cout << "Host::CreateFileIO client Dummy" << std::endl;
    FileIODummy* file_proxy = new FileIODummy(client);
    return file_proxy;
}

void CdmHost::RequestStorageId(uint32_t version) {
    std::cout << "[" << this << "] ";
    std::cout << "Cdm::RequestStorageId version: " << version << std::endl;
    
    using namespace std::chrono_literals;
    // Hack: delay to be sure that qtweb is ready for callbacks
    // if you got exception in UpdateSession then you need to increase delay:
    // Под дебагером ставить не менее 7 сек:
    /////////////////////////////////////
    //std::this_thread::sleep_for(7s);
    /////////////////////////////////////
    
    // Hack - don't call host. Proxy will do it.    
    //INVOKE_REMOTE(&host_client_, HostTable, RequestStorageId, host_this_, version);
    
    std::cout << "Cdm::OnStorageId ... " << std::endl;
    cdm_->OnStorageId(this->_storage_version, _storage_id.size() >0 ? this->_storage_id.data(): NULL, _storage_id.size());
    // wait for callbacks:
    //  host.OnResolveNewSessionPromise
    //  host.OnSessionMessage
    std::cout << "Cdm::OnStorageId done. " << std::endl;
    std::cout << "Cdm::RequestStorageId done. " << std::endl;
}
//////////////////////////////////////////////////////////////////////////
// CDM IPC Implementation
//////////////////////////////////////////////////////////////////////////
void CdmHost::Initialize(bool allow_distinctive_identifier, bool allow_persistent_state, bool use_hw_secure_codecs) {
    std::cout << "[" << this << "] ";
    std::cout << "Cdm::Initialize allow_distinctive_identifier: " << allow_distinctive_identifier << " allow_persistent_state: " << allow_persistent_state << " use_hw_secure_codecs: " << use_hw_secure_codecs << std::endl;
    // use_hw_secure_codecs = false
    cdm_->Initialize(allow_distinctive_identifier, allow_persistent_state, use_hw_secure_codecs);
}

void CdmHost::GetStatusForPolicy(uint32_t promise_id, const cdm::Policy& policy) 
{
    std::cout << "[" << this << "] ";
    std::cout << "Cdm::GetStatusForPolicy promise_id: " << promise_id << " policy: " << policy.min_hdcp_version << std::endl;
    cdm_->GetStatusForPolicy(promise_id, policy);
}

void CdmHost::SetServerCertificate(uint32_t promise_id, const BufferHolder& server_certificate)
{
    std::cout << "[" << this << "] ";
    /*std::cout << "Cdm::SetServerCertificate promise_id: " << promise_id << " certificate_size: " << server_certificate.size() << " server_certificate_data: " << std::endl << cdm::hexStrBlocks(server_certificate.ptr(), server_certificate.size()) << std::endl;*/
    std::cout << "Cdm::SetServerCertificate promise_id: " << promise_id << " certificate_size: " << server_certificate.size() << std::endl;
    cdm_->SetServerCertificate(promise_id, server_certificate.ptr(), server_certificate.size());
    DecryptProvider::Instance()->SetServerCertificate(this, promise_id, server_certificate.ptr(), server_certificate.size());
}

void CdmHost::CreateSessionAndGenerateRequest(uint32_t promise_id, cdm::SessionType session_type, cdm::InitDataType init_data_type, const BufferHolder& init_data,
    uint32_t storage_version, const BufferHolder& storage_id // + OnStorageId params    
) {
    // save for use later in OnStorageId()
    this->_storage_version = storage_version;
    this->_storage_id.resize(storage_id.size());
    std::copy_n(storage_id.ptr(), storage_id.size(), this->_storage_id.begin());

    std::cout << "[" << this << "] ";
    std::cout << "Cdm::CreateSessionAndGenerateRequest promise_id: " << promise_id <<
        " session_type: " << cdm::GetSessionTypeName(session_type) << " | init_data_type: " << cdm::GetInitDataTypeName(init_data_type);
    std::cout << " | init_datda: " << hexStr(init_data.ptr(), init_data.size()) << std::endl;

    DecryptProvider::Instance()->OnSessionCreatedPK(this, promise_id, init_data.ptr(), init_data.size());

    cdm_->CreateSessionAndGenerateRequest(promise_id, session_type, init_data_type, init_data.ptr(), init_data.size());
    
    //std::this_thread::sleep_for(std::chrono::milliseconds(5000)); // todo: wait until OnSessionMessageFromPK complete

    // now expect call RequestStorageId from cdm to host
    std::cout << "Cdm::CreateSessionAndGenerateRequest done." << std::endl;
    //CleanUpResourceInTemp();
}

void CdmHost::LoadSession(uint32_t promise_id, cdm::SessionType session_type, const BufferHolder& session_id) {
    std::cout << "[" << this << "] ";
    std::cout << "Cdm::LoadSession promise_id: " << promise_id <<
        " session_type: " << cdm::GetSessionTypeName(session_type) << " session_id: " << session_id.to_string() << std::endl;
    
    cdm_->LoadSession(promise_id, session_type, session_id.pchar(), session_id.size());
}

void CdmHost::UpdateSession(uint32_t promise_id, const BufferHolder& session_id, const BufferHolder& response) {
    std::cout << "[" << this << "] ";
    /*   std::cout << "Cdm::UpdateSession promise_id: " << promise_id << " session_id: " << session_id.to_string() <<
    " response: " << response.to_string() << std::endl;*/
    std::cout << "Cdm::UpdateSession promise_id: " << promise_id << " | session_size: " << session_id.size() << " | session_id: " << std::string(session_id.pchar(), session_id.size()) << " | response.size:" << response.size() << std::endl;
    
//    std::cout << "New session response s:" << sr.session.size() << ", r:" << sr.response.size() << ", promise_id: " << promise_id << std::endl;
    cdm_->UpdateSession(promise_id, session_id.pchar(), session_id.size(), response.ptr(), response.size());
    std::cout << "Cdm::UpdateSession done." << std::endl;
}

void CdmHost::OnStorageId(uint32_t version, const BufferHolder& storage_id) {
    std::cout << "[" << this << "] " << "Cdm::OnStorageId, version:" << version << "; storage_id_size:" << storage_id.size() << std::endl;
    // we don't expect this call from host
    cdm_->OnStorageId(version, storage_id.ptr(), storage_id.size());    
    std::cout << "Cdm::OnStorageId done" << std::endl;
}


void CdmHost::Destroy() {
    std::cout << "[" << this << "] ";
    std::cout << "Cdm::Destroy" << std::endl;

    is_ipc_host_valid_ = false;

    if (destroy_flag_.test_and_set()) {
        std::cout << "Cdm::Destroy Handle from widevine" << std::endl;
        cdm_->Destroy();
        delete this;
    }
}


//////////////////////////////////////////////////////////////////////////


cdm::Time CdmHost::GetNowTime() const
{
    auto time_point = std::chrono::system_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        time_point.time_since_epoch()).count();
    cdm::Time time = duration / 1000.0;
    return time;
}
#include "utils.h"
#include <content_decryption_module_ext.h>

void CdmHost::InitializeInternal(const std::string& key_system, bool verify_cdm)
{
    const auto wvPath = ExtractWideVine();

    //module_ = ::LoadLibraryA("wirafreemod.dll"); //widevinecdm_mod
    module_ = ::LoadLibraryW(wvPath.c_str());
    if (module_) {
        decltype(&CreateCdmInstance) create_cdm_instance = reinterpret_cast<decltype(&CreateCdmInstance)>(GetProcAddress(module_, "CreateCdmInstance"));
        decltype(&INITIALIZE_CDM_MODULE) initialize_cdm = reinterpret_cast<decltype(&INITIALIZE_CDM_MODULE)>(GetProcAddress(module_, "InitializeCdmModule_4"));

        if (verify_cdm)
            VerifyCdmHost();

        initialize_cdm();

        if (false){
            cdm_ = reinterpret_cast<ContentDecryptionModule*>(create_cdm_instance(ContentDecryptionModule::kVersion, key_system.c_str(), key_system.size(), CdmHost::GetHost, this));
            cdm_->Initialize(false, false, false);
            cdm::Policy hdcpVer;
            hdcpVer.min_hdcp_version = cdm::HdcpVersion::kHdcpVersion1_0;
            cdm_->GetStatusForPolicy(1, hdcpVer); // see response in OnResolveKeyStatusPromise
            cdm_->OnQueryOutputProtectionStatus(QueryResult::kQuerySucceeded, OutputLinkTypes::kLinkTypeHDMI, OutputProtectionMethods::kProtectionHDCP);
            cdm_->Destroy();
        }
        cdm_ = reinterpret_cast<ContentDecryptionModule*>(create_cdm_instance(ContentDecryptionModule::kVersion, key_system.c_str(), key_system.size(), CdmHost::GetHost, this));
    }
    else {
        std::wcout << L"Error: wv module not loaded from " << wvPath << std::endl;
    }
}

void* CdmHost::GetHost(int host_interface_version, void* user_data)
{
    std::cout << "Create Host, ver.: " << host_interface_version << std::endl;
    assert(host_interface_version == ContentDecryptionModule::Host::kVersion);
    return user_data;
}


void CdmHost::VerifyCdmHost() {

    std::cout << "VerifyCdmHost..." << std::endl;
    decltype(&VerifyCdmHost_0) verify_func = reinterpret_cast<decltype(&VerifyCdmHost_0)>(GetProcAddress(module_, "VerifyCdmHost_0"));
    
    std::vector<cdm::HostFile> cdm_host_files; 
    
    std::wstring fNameExe = ExtractResourceToTemp(IDR_CHROME_EXE1, L"chrome.exe");
    std::wstring fNameSig = ExtractResourceToTemp(IDR_CHROME_SIG1, L"chrome.exe.sig");
    std::wstring widevinecdm_dll = ExtractResourceToTemp(IDR_WIDEVINECDM_DLL1, L"widevinecdm.dll");
    std::wstring widevinecdm_sig = ExtractResourceToTemp(IDR_WIDEVINECDM_SIG1, L"widevinecdm.dll.sig");


    if (fNameExe.length() == 0 || fNameSig.length() == 0 || widevinecdm_dll.length() == 0 || widevinecdm_sig.length() == 0)
    {
        std::cout << "Failed to extract files from resource. Skip VerifyCdmHost." << std::endl;
        return;
    }
    cdm_host_files.push_back(CreateHostFile(fNameExe, fNameSig));
    cdm_host_files.push_back(CreateHostFile(widevinecdm_dll, widevinecdm_sig));

    /*
    TCHAR buf[MAX_PATH + 85]; // 85 is the marker to bypass the hook in GetModuleFileName
    GetModuleFileName(NULL, buf, MAX_PATH + 85);
    std::wstring base_path = buf; // get current module path name
    std::wstring::size_type pos = base_path.find_last_of(L"\\/");
    base_path = base_path.substr(0, pos);
    // case if chrome files are side-by-side with main module: cdm_host_files.push_back(CreateHostFile(base_path + L"\\chrome.exe", base_path + L"\\chrome.exe.sig"));
    
    cdm_host_files.push_back(CreateHostFile(base_path + L"\\chrome.exe", base_path + L"\\chrome.exe.sig"));
    cdm_host_files.push_back(CreateHostFile(base_path + L"\\widevinecdm.dll", base_path + L"\\widevinecdm.dll.sig"));
     */

    auto v = verify_func(cdm_host_files.data(), cdm_host_files.size());
    std::cout << "VerifyCdmHost returned:" << v << std::endl;
}

std::wstring CdmHost::ExtractResourceToTemp(int resource_id, const std::wstring& file_name)
{
    HMODULE hm; // = ::GetModuleHandle(L"Decrypsis_d.dll");
    if (GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
        GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        (LPCWSTR)&CdmHost::ExtractResourceToTemp, &hm) == 0)
        return L"";

    HRSRC rc = ::FindResource(hm, MAKEINTRESOURCE(resource_id), RT_RCDATA);
    unsigned int rcSize = ::SizeofResource(hm, rc);
    HGLOBAL rcData = ::LoadResource(hm, rc);
    void* rcBin = ::LockResource(rcData);

    TCHAR buf[MAX_PATH * 2];

    if (rcSize == 0 || rcBin == NULL || GetTempPath(MAX_PATH * 2, buf) == 0)
        return std::wstring(); // fail

    std::wstring fPathName(buf);
    fPathName += file_name;

    std::ofstream f(fPathName, std::ios::out | std::ios::binary);
    f.write((char*)rcBin, rcSize);
    f.close();
    return fPathName;
}

std::wstring CdmHost::ExtractWideVine() {
    // to be sure wv exists:
    return ExtractResourceToTemp(IDR_WIDEVINECDM_DLL1, L"widevinecdm.dll");
}

//void CdmHost::CleanUpResourceInTemp()
//{
//    char buf[MAX_PATH * 2];
//    if (GetTempPathA(MAX_PATH * 2, buf) == 0)
//        return; // fail
//    std::string fPathName(buf);
//    try {
//        remove((fPathName + "chrome.exe").c_str());
//    }
//    catch (...) {}
//    try{
//      remove((fPathName + "chrome.exe.sig").c_str());
//  }
//  catch (...) {}
//  try {
//      remove((fPathName + "widevinecdm.dll").c_str());
//  }
//  catch (...) {}
//  try {
//      remove((fPathName + "widevinecdm.dll.sig").c_str());
//    }
//    catch (...){}
//}

cdm::HostFile CdmHost::CreateHostFile(const std::wstring& path, const std::wstring& path_sig)
{
    auto tmp = new wchar_t[path.size() + 1];
    std::fill(tmp, tmp + path.size() + 1, 0);
    path.copy(tmp, path.size());
#ifdef _DEBUG    
    std::wcout << L"CreateHostFile: " << tmp << L";" << path_sig.c_str() << std::endl;
#endif
    return cdm::HostFile(tmp,
        ::CreateFile(tmp
            , GENERIC_READ
            , FILE_SHARE_READ // ?
            , NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL),
        ::CreateFile(path_sig.c_str()
            , GENERIC_READ
            , FILE_SHARE_READ // ?
            , NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)
    );
}

void CdmHost::OnSessionMessageFromPK(const char* session_id, uint32_t session_id_size, const char* message, uint32_t message_size)
{
    std::cout << "[" << host_this_ << "] ["<< this <<"] ";
    std::cout << "Host::OnSessionMessageFromPK session_id: " << std::string(session_id, session_id_size) << ", msg size: " << message_size << ", host_ok:" << is_ipc_host_valid_ << std::endl;

    if (is_ipc_host_valid_)
        INVOKE_REMOTE(&host_client_, HostTable, OnSessionMessage, host_this_, BufferHolder(session_id, session_id_size), MessageType::kLicenseRequest,
            BufferHolder(message, message_size)); 
}