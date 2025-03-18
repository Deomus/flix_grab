#include "cdm_proxy.h"
#include "struct_strings.h"
#include <iomanip>
#include <future>
#include <chrono>
#include <thread>

using namespace cdm;

/*

From ISO 23001-7 2nd edition:
aligned(8) class ProtectionSystemSpecificHeaderBox extends FullBox(�pssh�, version, flags=0)
{
unsigned int(8)[16] SystemID;
if (version > 0) {
unsigned int(32) KID_count;
{
unsigned int(8)[16] KID;
} [KID_count];
}
unsigned int(32) DataSize;
unsigned int(8)[DataSize] Data;
}


message WidevineCencHeader {
enum Algorithm {
UNENCRYPTED = 0;
AESCTR = 1;
};
optional Algorithm algorithm = 1;
repeated bytes key_id = 2;
// Content provider name.
optional string provider = 3;
// A content identifier, specified by content provider.
optional bytes content_id = 4;
// Track type. Acceptable values are SD, HD and AUDIO. Used to
// differentiate content keys used by an asset.
optional string track_type_deprecated = 5;
// The name of a registered policy to be used for this asset.
optional string policy = 6;
// Crypto period index, for media using key rotation.
optional uint32 crypto_period_index = 7;
// Optional protected context for group content. The grouped_license is a
// serialized SignedMessage.
optional bytes grouped_license = 8;
// Protection scheme identifying the encryption algorithm.
// Represented as one of the following 4CC values:
// 'cenc' (AES�CTR), 'cbc1' (AES�CBC),
// 'cens' (AES�CTR subsample), 'cbcs' (AES�CBC subsample).
optional uint32 protection_scheme = 9;
// Optional. For media using key rotation, this represents the duration
// of each crypto period in seconds.
optional uint32 crypto_period_seconds = 10;
}


https://developers.google.com/protocol-buffers/docs/encoding


//HULU
000000487073736800000000edef8ba979d64acea3c827dcd51d21ed00000028080112102689984b4c705437a9819021fcb10ae01a0468756c75220835313030393338362a024844

//netflix
000000347073736800000000edef8ba979d64acea3c827dcd51d21ed0000001408011210 00000000393f2c470000000000000000

00000048 70737368   //Size + pssh
00000000        //Box Version + flags  (4bytes)
edef8ba979d64acea3c827dcd51d21ed    //System ID (16 bytes)
00000028    //DataSize
//ProtoBuffer Data
08011210    //08 - Varint, Field 1, = 1 ; 1210 = 2- string, Field 2, size 10 (16) =
2689984b4c705437a9819021fcb10ae0    //Key ID
1a04 68756c75               // 1a 04 = Type 2, Field 3, size 4: provider = 68756c75
2208 3531303039333836           // 22   Type 2 Field 4 Size 8: content_id = 3531303039333836
2a02 4844               // 2a   Type 2 Field 5 Size 2: track_type_deprecated = 4844

*/


/*


inline const uint8* ReadVarint32FromArray(const uint8* buffer, uint32* value) {
static const int kMaxVarintBytes = 10;
static const int kMaxVarint32Bytes = 5;

// Fast path:  We have enough bytes left in the buffer to guarantee that
// this read won't cross the end, so we can skip the checks.
const uint8* ptr = buffer;
uint32 b;
uint32 result;

b = *(ptr++); result  = (b & 0x7F)      ; if (!(b & 0x80)) goto done;
b = *(ptr++); result |= (b & 0x7F) <<  7; if (!(b & 0x80)) goto done;
b = *(ptr++); result |= (b & 0x7F) << 14; if (!(b & 0x80)) goto done;
b = *(ptr++); result |= (b & 0x7F) << 21; if (!(b & 0x80)) goto done;
b = *(ptr++); result |=  b         << 28; if (!(b & 0x80)) goto done;

// If the input is larger than 32 bits, we still need to read it all
// and discard the high-order bits.
for (int i = 0; i < kMaxVarintBytes - kMaxVarint32Bytes; i++) {
b = *(ptr++); if (!(b & 0x80)) goto done;
}

// We have overrun the maximum size of a varint (10 bytes).  Assume
// the data is corrupt.
return NULL;

done:
*value = result;
return ptr;
}


*/

// This proxy is created on Hook side (qtweb)
// Класс перехватывает вызовы в cdm из qtweb. 
// Начиная с 10 версии вызовы в cdm сопровождаются колбэками обратно в qtweb, например:
// cdm.CreateSessionAndGenerateRequest ->
//      < -host.RequestStorageId
//      cdm.OnStorageId ->
//          < -host.OnResolveNewSessionPromise
//          < -host.OnSessionMessage
// 
// Qtweb ожидает что прямой вызов и колбэк будет в одном потоке. Поэтому те вызовы в которых ожидаются колбэки должны выполнятся через messageLoop.
// Для этого прямой вызов в cdm делается в отдельном потоке, а в основном потоке вызывается MessageLoop4Callbacks()
// По завершении прямого вызова надо дернуть QueuedCallEnd, чтоб выйти из messageLoop.
// Колбэки должны вызвать AddQueuedCall, который перенаправит колбэк в основной поток.
// Кроме этого внутри вызова CreateSessionAndGenerateRequest происходит еще один прямой вызов из колбэка: OnStorageId. Двойные вызовы не поддерживаются в нашем ipc.
// Поэтому вызовы CreateSessionAndGenerateRequest и OnStorageId объединены в один вызов. А вызовы RequestStorageId эмулируются. 
// В результате этого костыля есть побочный эффект: если на стороне qtweb эмуляция RequestStorageId произойдет позже, то из cdm полетят колбэки, которые qtweb еще не ожидает.
// Для решения добавлена задержка в CdmHost::RequestStorageId, которая притормаживает cdm чтоб qtweb успел подготовится к приему колбэков.

#ifdef FILELOGGER
CdmProxyDS::CdmProxyDS(ipc::Client& cdm_client, const std::string& key_system, ContentDecryptionModule::Host* host, std::ofstream& file_logger_)
#else
CdmProxyDS::CdmProxyDS(ipc::Client& cdm_client, const std::string& key_system, ContentDecryptionModule::Host* host)
#endif
    : host_(host)
    , cdm_client_(cdm_client)
{    
    InitializeInternal(key_system);
    std::cout << "[" << cdm_this_ << std::setprecision(12) << "], thread:" << std::this_thread::get_id() << ", time: " << host->GetCurrentWallTime() << ", ";
    std::cout << "Cdm::Constructor " << is_initialized << std::endl;
#ifdef FILELOGGER
    file_logger_ << "Cdm::Constructor " << is_initialized << std::endl;
#endif
}

CdmProxyDS::~CdmProxyDS()
{
    std::cout << "[" << cdm_this_ << "] ";
    std::cout << "Cdm::Destructor" << std::endl;
}



void CdmProxyDS::InitializeInternal(const std::string& key_system)
{
    if (cdm_client_.status().Ok() ) {
   
        cdm_this_ = INVOKE_REMOTE(&cdm_client_, CdmTable, CreateCdmIpc, 
            key_system, DEFAULT_CDM_HOST_IPC, reinterpret_cast<uintptr_t>(static_cast<HostIpcAbstract*>(this)));

        is_initialized = (cdm_this_ > 0);
    }
}

// Вызывается в основном потоке и дергает колбэки из очереди, помещенные туда с помощью AddQueuedCall
void CdmProxyDS::MessageLoop4Callbacks() {
    std::cout << " waiting callbacks [" << &q_ << "]..." << std::endl;
    while (true) {
        std::unique_lock<std::mutex> lkT(mqT_);
        while (q_.empty())
        {
            cvT_.wait(lkT);
        }
        //std::cout << "q_";
        auto f = q_.front();
        //auto f = std::get<0>(*fq.get()); // function
        q_.pop();
        if (f == nullptr)
            break;
        f(); //here is callback
        // notify...
        std::cout << " callback ended. Notify caller ..." << std::endl;
        cv_.notify_one();
    }
}

// Добавляет колбэк в очередь и ждет когда он выполнится в основном потоке
void    CdmProxyDS::AddQueuedCall(std::function<void()> f, bool toWait) {
    std::cout << "[" << std::this_thread::get_id() << "]  add callback to the queue..." << std::endl;
    {
        std::unique_lock<std::mutex> lkT(mqT_);
        q_.emplace(f); //   q_.push(f); 
        lkT.unlock();
        cvT_.notify_one();
    }
    if (toWait) {
        //std::cout << "  added queued call. [" << &q_ << "] Wait to complete ..." << std::endl;
        // wait for function to complete..
        std::unique_lock<std::mutex> lk(mq_);
        cv_.wait(lk);
        lk.unlock();
    }
};
void CdmProxyDS::QueuedCallEnd()
{
    AddQueuedCall(nullptr, false);
}

//Cdm Interface;
//////////////////////////////////////////////////////////////////////////

void CdmProxyDS::Initialize(bool allow_distinctive_identifier, bool allow_persistent_state, bool use_hw_secure_codecs)
{
    allow_persistent_state_ = allow_persistent_state;
    std::cout << "[" << cdm_this_ << "], thread:" << std::this_thread::get_id() << ",";
    std::cout << "Cdmproxy::Initialize allow_distinctive_identifier: " << allow_distinctive_identifier << " allow_persistent_state: " << allow_persistent_state << " bool use_hw_secure_codecs: " << use_hw_secure_codecs << std::endl;

    if (!is_initialized) {
        std::cout << "Not initialized! " << std::endl;
        return;
    }

    auto t = std::thread(
    [=]()
    {
        std::cout << "Remote Initialize, thread:" << std::this_thread::get_id() << " ..." << std::endl;
        INVOKE_REMOTE(&this->cdm_client_, CdmTable, Initialize, this->cdm_this_, allow_distinctive_identifier, allow_persistent_state, use_hw_secure_codecs);
        std::cout << "Remote Initialized ok." << std::endl;
        QueuedCallEnd();
    });
    
    MessageLoop4Callbacks();
    t.join();

    std::cout << "Initialized end." << std::endl;
}

void CdmProxyDS::GetStatusForPolicy(uint32_t promise_id,
    const cdm::Policy& policy) {
    std::cout << "[" << cdm_this_ << "] ";
    std::cout << "Cdmproxy::GetStatusForPolicy promise_id: " << promise_id << " policy: " << policy.min_hdcp_version << std::endl;
    // todo:
}


void CdmProxyDS::SetServerCertificate(uint32_t promise_id, const uint8_t* server_certificate_data, uint32_t server_certificate_data_size)
{
    std::cout << "[" << cdm_this_ << "] ";
    //std::cout << "Cdm::SetServerCertificate promise_id: " << promise_id << " certificate_size: " << server_certificate_data_size << " server_certificate_data: " << std::endl << cdm::hexStrBlocks(server_certificate_data, server_certificate_data_size) << std::endl;
    std::cout << "Cdmproxy::SetServerCertificate promise_id: " << promise_id << " certificate_size: " << server_certificate_data_size << " server_certificate_data: " << std::endl;

    if (!is_initialized) {
        std::cout << "Not initialized! " << std::endl;
        return;
    }
    auto t = std::thread([=]()
    {
        INVOKE_REMOTE(&cdm_client_, CdmTable, SetServerCertificate, cdm_this_, promise_id, BufferHolder(server_certificate_data, server_certificate_data_size));
        std::cout << "SetServerCertificate ok." << std::endl;
        QueuedCallEnd();
    });
    MessageLoop4Callbacks();
    t.join();
    std::cout << "SetServerCertificate done." << std::endl;
}

void CdmProxyDS::CreateSessionAndGenerateRequest(uint32_t promise_id, SessionType session_type, InitDataType init_data_type, const uint8_t* init_data, uint32_t init_data_size)
{
#ifdef FILELOGGER
    std::wofstream file_logger;
    file_logger.open(L"C:\\temp\\CdmProxy.log", std::ofstream::out | std::ofstream::app | std::ofstream::binary);
    file_logger << std::endl;
    file_logger << L"CreateSessionAndGenerateRequest: promise_id=" << promise_id << std::endl;
#endif  
    
    std::cout << "[" << cdm_this_ << "] ";
    std::cout << "Cdmproxy::CreateSessionAndGenerateRequest promise_id: " << promise_id <<
        " session_type: " << GetSessionTypeName(session_type) << " | init_data_type: " << GetInitDataTypeName(init_data_type);

    std::cout << " | init_datda: " << hexStr(init_data, init_data_size) << std::endl;
   
    if (!is_initialized) {
        std::cout << "Not initialized! " << std::endl;
        return;
    }
    _differCSAndGR = true;
    this->promise_id_req = promise_id;
    this->session_type_req = session_type;
    this->init_data_type_req = init_data_type;
    
    this->_buffCSAndGR.resize(init_data_size);
    std::copy_n(init_data, init_data_size, this->_buffCSAndGR.begin());
    
    if (allow_persistent_state_) 
    {
        // Emulating the call to RequestStorageId. This call leads to OnStorageId()
        uint32_t storage_version = 0; //TODO: subject to change. Currently cdm use version=0
        std::cout << "Cdmproxy::RequestStorageId version: " << storage_version << std::endl;
        host_->RequestStorageId(storage_version); // here we wait until OnStorageId complete
        std::cout << "Cdmproxy::RequestStorageId done." << std::endl;
        _differCSAndGR = false; 
    }
    else {
        auto t = std::thread([=]()
        {
                INVOKE_REMOTE(&cdm_client_, CdmTable, CreateSessionAndGenerateRequest, cdm_this_,
                    this->promise_id_req, this->session_type_req, this->init_data_type_req,
                    BufferHolder(_buffCSAndGR.data(), _buffCSAndGR.size()), 0, BufferHolder((uint8_t*)NULL, 0));
                //INVOKE_REMOTE(&cdm_client_, CdmTable, CreateSessionAndGenerateRequest, cdm_this_, promise_id, session_type, init_data_type, BufferHolder(init_data, init_data_size));
            std::cout << "CreateSessionAndGenerateRequest ok." << std::endl;
            QueuedCallEnd();
        });
    
        MessageLoop4Callbacks();
        t.join();
    }
    std::cout << "CreateSessionAndGenerateRequest done." << std::endl;
}

void CdmProxyDS::LoadSession(uint32_t promise_id, SessionType session_type, const char* session_id, uint32_t session_id_size)
{
    std::cout << "[" << cdm_this_ << "] ";
    std::cout << "Cdmproxy::LoadSession promise_id: " << promise_id <<
        " session_type: " << GetSessionTypeName(session_type) << " session_id: " << std::string(session_id, session_id_size) << std::endl;

    if (!is_initialized) {
        std::cout << "Not initialized! " << std::endl;
        return;
    }
    INVOKE_REMOTE(&cdm_client_, CdmTable, LoadSession, cdm_this_, promise_id, session_type, BufferHolder(session_id, session_id_size));
}

void CdmProxyDS::UpdateSession(uint32_t promise_id, const char* session_id, uint32_t session_id_size, const uint8_t* response, uint32_t response_size)
{
    std::cout << "[" << cdm_this_ << "] ";
    /*std::cout << "Cdm::UpdateSession promise_id: " << promise_id << " session_id: " << std::string(session_id, session_id_size) <<
        " response: " << std::string((const char*)response, response_size) << std::endl;*/
    std::cout << "Cdmproxy::UpdateSession promise_id: " << promise_id << " session_id: " << std::string(session_id, session_id_size) << std::endl;

    if (!is_initialized) {
        std::cout << "Not initialized! " << std::endl;
        return;
    }
    auto t = std::thread([=]()
    {
        INVOKE_REMOTE(&cdm_client_, CdmTable, UpdateSession, cdm_this_, promise_id, BufferHolder(session_id, session_id_size), BufferHolder(response, response_size));
        std::cout << "UpdateSession ok." << std::endl;
        QueuedCallEnd();
    });
    MessageLoop4Callbacks();
    t.join();
    std::cout << "UpdateSession done." << std::endl;
}

void CdmProxyDS::CloseSession(uint32_t promise_id, const char* session_id, uint32_t session_id_size)
{
    //this->AddQueuedCall([=] {
        std::cout << "[" << cdm_this_ << "] ";
        std::cout << "Cdmproxy::CloseSession promise_id: " << promise_id << " session_id: " << std::string(session_id, session_id_size) << std::endl;
        host_->OnResolvePromise(promise_id);
    //});    
}

void CdmProxyDS::RemoveSession(uint32_t promise_id, const char* session_id, uint32_t session_id_size)
{
    std::cout << "[" << cdm_this_ << "] ";
    std::cout << "Cdmproxy::RemoveSession promise_id: " << promise_id << " session_id: " << std::string(session_id, session_id_size) << std::endl;
    
}

void CdmProxyDS::TimerExpired(void* )
{
    std::cout << "Cdmproxy::TimerExpired " << std::endl;

    //cdm_->TimerExpired(context);
}

Status CdmProxyDS::Decrypt(const InputBuffer_2& , DecryptedBlock* )
{
    std::cout << "[" << cdm_this_ << "] ";
    std::cout << "Cdmproxy::Decrypt Buffer! " << std::endl;

    return kDecryptError;

}

Status CdmProxyDS::InitializeAudioDecoder(const AudioDecoderConfig& audio_decoder_config)
{
    Status status = kInitializationError; // kSessionError;

    std::cout << "[" << cdm_this_ << "] ";
    std::cout << "Cdmproxy::InitializeAudioDecoder status: " << GetStatusName(status) << " config: " << GetAudioConfigName(audio_decoder_config) << std::endl;
      
    return status;
}

Status CdmProxyDS::InitializeVideoDecoder(const VideoDecoderConfig& video_decoder_config)
{
    Status status = kInitializationError; // kSessionError;

   // status = kSuccess;
    std::cout << "[" << cdm_this_ << "] ";
    std::cout << "Cdmproxy::InitializeVideoDecoder status: " << GetStatusName(status) << " config: " << GetVideoConfigName(video_decoder_config) << std::endl;

    return status;
}

void CdmProxyDS::DeinitializeDecoder(StreamType decoder_type)
{
    std::cout << "[" << cdm_this_ << "] ";
    std::cout << "Cdmproxy::DeinitializeDecoder decoder_type: " << GetStreamTypeName(decoder_type) << std::endl;
     
}

void CdmProxyDS::ResetDecoder(StreamType decoder_type)
{
    std::cout << "[" << cdm_this_ << "] ";
    std::cout << "Cdmproxy::ResetDecoder decoder_type: " << GetStreamTypeName(decoder_type) << std::endl;
    
}

Status CdmProxyDS::DecryptAndDecodeFrame(const InputBuffer& encrypted_buffer, VideoFrame* )
{
    Status status = kDecodeError;
 
    std::cout << "[" << cdm_this_ << "] " << "Cdmproxy::DecryptAndDecodeFrame Status: " << GetStatusName(status) << " | timestamp: " << encrypted_buffer.timestamp << " | subsamples: " << encrypted_buffer.num_subsamples << " | data_size: " << encrypted_buffer.data_size
        << " | key_id: " << cdm::hexStr(encrypted_buffer.key_id, encrypted_buffer.key_id_size)
        << " | iv: " << cdm::hexStr(encrypted_buffer.iv, encrypted_buffer.iv_size) << std::endl;

  /*  std::cout << "[" << id_ << "] ";
    std::cout << "Cdm::DecryptAndDecodeFrame Video: " << GetStatusName(status) << std::endl;*/

    
    return status;
}

Status CdmProxyDS::DecryptAndDecodeSamples(const InputBuffer& , AudioFrames* )
{
    Status status = kDecodeError;

    std::cout << "[" << cdm_this_ << "] ";
    std::cout << "Cdmproxy::DecryptAndDecodeSamples Audio: " << GetStatusName(status) << std::endl;
    return status;
}

void CdmProxyDS::OnPlatformChallengeResponse(const PlatformChallengeResponse& )
{
    std::cout << "[" << cdm_this_ << "] ";
    std::cout << "Cdmproxy::OnPlatformChallengeResponse" << std::endl;
 
}

void CdmProxyDS::OnQueryOutputProtectionStatus(QueryResult result, uint32_t link_mask, uint32_t output_protection_mask)
{
    std::cout << "[" << cdm_this_ << "] ";
    std::cout << "Cdmproxy::OnQueryOutputProtectionStatus result: " << GetQueryResultName(result) <<
        " | link_mask: " << GetLinkMaskNames(link_mask) << " | output_protection: " << output_protection_mask << std::endl;
    
}

void CdmProxyDS::Destroy()
{
    std::cout << "[" << cdm_this_ << "] ";
    std::cout << "Cdmproxy::Destroy " << std::endl;

    if (is_initialized)
        INVOKE_REMOTE(&cdm_client_, CdmTable, Destroy, cdm_this_);
        
    delete this;
}

//////////////////////////////////////////////////////////////////////////
// Host Ipc Implementation
//////////////////////////////////////////////////////////////////////////
    // Called by the CDM with the result after the CDM instance was initialized.
void CdmProxyDS::OnInitialized(bool success) {
    auto f = [this,success] (void){
        try {
        std::cout << "[" << this << "], thread:" << std::this_thread::get_id() << ", ";
        std::cout << "Host::OnInitialized success: " << success << std::endl;
        host_->OnInitialized(success);
        std::cout << "Host::OnInitialized ok " << std::endl;
        }
        catch (...)
        {
            std::cout << "Host::OnInitialized Failed" << std::endl;
        }
    };
    std::cout << "Host::OnInitialized add to queue ..." << std::endl;
    AddQueuedCall(f);   
    std::cout << "Host::OnInitialized done." << std::endl;
}

// Called by the CDM when a key status is available in response to
// GetStatusForPolicy().
void CdmProxyDS::OnResolveKeyStatusPromise(uint32_t promise_id, cdm::KeyStatus key_status) {
    std::cout << "[" << this << "] ";
    std::cout << "Host::OnResolveKeyStatusPromise promise_id: " << promise_id << ", key_status: " << key_status << std::endl;
    host_->OnResolveKeyStatusPromise(promise_id, key_status);
    std::cout << "Host::OnResolveKeyStatusPromise done." << std::endl;
}

// Called by qtweb after RequestStorageId
void CdmProxyDS::OnStorageId(uint32_t version,
    const uint8_t* storage_id,
    uint32_t storage_id_size) {
    using namespace std::chrono_literals;
    
    std::cout << "[" << cdm_this_ << "] ";
    std::cout << "Cdmproxy::OnStorageId version: " << version << " storage_id_size: " << storage_id_size << std::endl;
    //std::cout << " wait for ..." << std::endl;
    //INVOKE_REMOTE(&cdm_client_, CdmTable, OnStorageId, cdm_this_, version, BufferHolder(storage_id, storage_id_size));

    auto t = std::thread([=]()
    {
        std::cout << "Cdmproxy::CreateSessionAndGenerateRequest+OnStorageId ..." << std::endl;
        // integrated call CreateSessionAndGenerateRequest + OnStorageId
        INVOKE_REMOTE(&cdm_client_, CdmTable, CreateSessionAndGenerateRequest, cdm_this_, 
            this->promise_id_req, this->session_type_req, this->init_data_type_req, BufferHolder(_buffCSAndGR.data(), _buffCSAndGR.size()), version, BufferHolder(storage_id, storage_id_size));
        std::cout << "Cdmproxy::CreateSessionAndGenerateRequest+OnStorageId ok." << std::endl;
        QueuedCallEnd();
    });

    MessageLoop4Callbacks();
    t.join();
    std::cout << "Cdmproxy::OnStorageId done." << std::endl;    
}

void CdmProxyDS::OnResolveNewSessionPromise(uint32_t promise_id, const BufferHolder& session_id) {
    auto f = [this, promise_id, session_id](void) {
        std::cout << "[" << this << "] ";
        std::cout << "Host::OnResolveNewSessionPromise promise_id: " << promise_id << " | session_id: " << session_id.to_string() << std::endl;
        host_->OnResolveNewSessionPromise(promise_id, session_id.pchar(), session_id.size());
        std::cout << "Host::OnResolveNewSessionPromise ok" << std::endl;
    };
    std::cout << "Host::OnResolveNewSessionPromise ..." << std::endl;
    AddQueuedCall(f);
    std::cout << "Host::OnResolveNewSessionPromise done." << std::endl;
}

void CdmProxyDS::OnResolvePromise(uint32_t promise_id) {
    auto f = [this, promise_id] {
        std::cout << "[" << this << "] ";
        std::cout << "Host::OnResolvePromise promise_id: " << promise_id << std::endl;
        host_->OnResolvePromise(promise_id); 
        std::cout << "Host::OnResolvePromise  ok" << std::endl;
    };
    std::cout << "Host::OnResolvePromise  ..." << std::endl;
    AddQueuedCall(f);
    std::cout << "Host::OnResolvePromise  done." << std::endl;
}

void CdmProxyDS::OnRejectPromise(uint32_t promise_id, cdm::Exception exception, uint32_t system_code, const BufferHolder& error_message) {
    auto f = [=] {
        std::cout << "[" << this << "] ";
        std::cout << "Host::OnRejectPromise promise_id: " << promise_id << " | error: " << exception <<
            " | system_code: " << system_code << " | error_message: " << error_message.to_string() << std::endl;

        host_->OnRejectPromise(promise_id, exception, system_code, error_message.pchar(), error_message.size());
        std::cout << "Host::OnRejectPromise ok ((." << std::endl;
    };
    std::cout << "Host::OnRejectPromise ..." << std::endl;
    AddQueuedCall(f);
    std::cout << "Host::OnRejectPromise done." << std::endl;
}
// , const BufferHolder& legacy_destination_url
void CdmProxyDS::OnSessionMessage(const BufferHolder& session_id, cdm::MessageType message_type, const BufferHolder& message) {
    auto f = [=] {
        std::cout << "[" << this << "] ";
        /*  std::cout << "Host::OnSessionMessage session_id: " << session_id.to_string() <<
        " | message_type: " << cdm::GetMessageTypeName(message_type) << " | message: " << message.to_string() <<
        " | legacy_destination_url: " << std::string(legacy_destination_url.pchar(), legacy_destination_url.size()) << std::endl;*/

        std::cout << "Host::OnSessionMessage session_id: " << session_id.to_string() <<
            " | message_type: " << cdm::GetMessageTypeName(message_type) << std::endl;

        host_->OnSessionMessage(session_id.pchar(), session_id.size(), message_type, message.pchar(), message.size());
    };
    AddQueuedCall(f);
    std::cout << "Host::OnSessionMessage done." << std::endl;
}

void CdmProxyDS::OnSessionKeyChange(const BufferHolder& session_id, const BufferHolder& key_id, cdm::KeyStatus key_status, uint32_t system_code) {
    auto f = [this, session_id, key_id, key_status, system_code] {
        std::cout << "[" << this << "] ";
        std::cout << "Host::OnSessionKeysChange session_id: " << session_id.to_string() <<
            " | key_id: " << cdm::hexStr(key_id.ptr(), key_id.size()) << " | key_status: " << cdm::GetKeyStatusName(key_status) << std::endl;


        cdm::KeyInformation     info;
        info.key_id = key_id.ptr();
        info.key_id_size = key_id.size();
        info.status = key_status;
        info.system_code = system_code;

        host_->OnSessionKeysChange(session_id.pchar(), session_id.size(), (key_status == cdm::kUsable), &info, 1);
    };
    AddQueuedCall(f);
    std::cout << "Host::OnSessionKeyChange done." << std::endl;
}

void CdmProxyDS::OnExpirationChange(const BufferHolder& session_id, double new_expiry_time) {
    auto f = [this, session_id, new_expiry_time] {
        std::cout << "[" << this << "] ";
        std::cout << "Host::OnExpirationChange session_id: " << session_id.to_string() << " | new_expiry_time: " << new_expiry_time << std::endl;
        host_->OnExpirationChange(session_id.pchar(), session_id.size(), new_expiry_time);
    };
    AddQueuedCall(f);
    std::cout << "Host::OnExpirationChange done." << std::endl;
}

void CdmProxyDS::OnSessionClosed(const BufferHolder& session_id) {
    std::cout << "[" << this << "] ";
    std::cout << "Host::OnSessionClosed session_id: " << session_id.to_string() << std::endl;

    host_->OnSessionClosed(session_id.pchar(), session_id.size());
}
// deprecated
//void CdmProxyDS::OnLegacySessionError(const BufferHolder& session_id, cdm::Exception exception, uint32_t system_code, const BufferHolder& error_message) {
//    std::cout << "[" << this << "] ";
//    std::cout << "Host::OnLegacySessionError session_id: " << session_id.to_string() <<
//        " | exception: " << exception << " | system_code: " << system_code << " | error_message: " << error_message.to_string() << std::endl;
//    //host_->OnLegacySessionError(session_id.pchar(), session_id.size(), exception, system_code, error_message.pchar(), error_message.size());
//}

void CdmProxyDS::SendPlatformChallenge(const BufferHolder& service_id, const BufferHolder& challenge) {
    std::cout << "[" << this << "] ";
    std::cout << "Host::SendPlatformChallenge service_id: " << service_id.to_string() << " | challenge: " << challenge.to_string() << std::endl;
    host_->SendPlatformChallenge(service_id.pchar(), service_id.size(), challenge.pchar(), challenge.size());
}

void CdmProxyDS::EnableOutputProtection(uint32_t desired_protection_mask) {
    std::cout << "[" << this << "] ";
    std::cout << "Host::EnableOutputProtection desired_protection_mask: " << desired_protection_mask << std::endl;
    host_->EnableOutputProtection(desired_protection_mask);
}

void CdmProxyDS::RequestStorageId(uint32_t version) {
    //auto f = [this, version](void) {
        std::cout << "[" << this << "] ";
        std::cout << "Host::RequestStorageId version: " << version << std::endl;
        host_->RequestStorageId(version);
        std::cout << "Host::RequestStorageId ok" << std::endl;
    //};
    //std::cout << "Host::RequestStorageId ..." << std::endl;
    //AddQueuedCall(f);
    //std::cout << "Host::RequestStorageId done." << std::endl;
}