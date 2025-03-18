#include "cdm_proxy.h"
#include "struct_strings.h"
#include "host_proxy.h"


using namespace cdm;

/*

From ISO 23001-7 2nd edition:
aligned(8) class ProtectionSystemSpecificHeaderBox extends FullBox(‘pssh’, version, flags=0)
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
// 'cenc' (AES­CTR), 'cbc1' (AES­CBC),
// 'cens' (AES­CTR subsample), 'cbcs' (AES­CBC subsample).
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

00000048 70737368	//Size + pssh
00000000		//Box Version + flags  (4bytes)
edef8ba979d64acea3c827dcd51d21ed	//System ID (16 bytes)
00000028	//DataSize
//ProtoBuffer Data
08011210	//08 - Varint, Field 1, = 1 ; 1210 = 2- string, Field 2, size 10 (16) =
2689984b4c705437a9819021fcb10ae0	//Key ID
1a04 68756c75				// 1a 04 = Type 2, Field 3, size 4: provider = 68756c75
2208 3531303039333836			// 22   Type 2 Field 4 Size 8: content_id = 3531303039333836
2a02 4844 				// 2a   Type 2 Field 5 Size 2: track_type_deprecated = 4844

*/


CdmProxy::CdmProxy(cdm::ContentDecryptionModule* cdm, HostProxy* host_proxy) : cdm_(cdm)
, host_proxy_(host_proxy)
, is_offline_(false)
{
    host_proxy_->Init(cdm);
    id_ = host_proxy_->id();
    std::cout << "[" << id_ << "] ";
    std::cout << "Cdm::Constructor" << std::endl;
}

CdmProxy::~CdmProxy()
{
    std::cout << "[" << id_ << "] ";
    std::cout << "Cdm::Destructor" << std::endl;
}

//Cdm Interface;
//////////////////////////////////////////////////////////////////////////

void CdmProxy::Initialize(bool allow_distinctive_identifier, bool allow_persistent_state)
{
    std::cout << "[" << id_ << "] ";
    std::cout << "Cdm::Initialize allow_distinctive_identifier: " << allow_distinctive_identifier << " allow_persistent_state: " << allow_persistent_state << std::endl;

    cdm_->Initialize(allow_distinctive_identifier, allow_persistent_state);
}

void CdmProxy::SetServerCertificate(uint32_t promise_id, const uint8_t* server_certificate_data, uint32_t server_certificate_data_size)
{
    std::cout << "[" << id_ << "] ";
    
    //std::cout << "Cdm::SetServerCertificate promise_id: " << promise_id << " server_certificate_data: " << std::string((const char*)server_certificate_data, server_certificate_data_size) << std::endl;
    std::cout << "Cdm::SetServerCertificate promise_id: " << promise_id << " certificate_size: " << server_certificate_data_size << " server_certificate_data: " << std::endl << cdm::hexStrBlocks(server_certificate_data, server_certificate_data_size) << std::endl;

    cdm_->SetServerCertificate(promise_id, server_certificate_data, server_certificate_data_size);
}

void CdmProxy::CreateSessionAndGenerateRequest(uint32_t promise_id, SessionType session_type, InitDataType init_data_type, const uint8_t* init_data, uint32_t init_data_size)
{
    std::cout << "[" << id_ << "] ";
    std::cout << "Cdm::CreateSessionAndGenerateRequest promise_id: " << promise_id <<
        " session_type: " << GetSessionTypeName(session_type) << " | init_data_type: " << GetInitDataTypeName(init_data_type);

        std::cout << " | init_datda: " << hexStr(init_data, init_data_size);
        //std::cout << " | init_datda: " << std::string((const char*)init_data, init_data_size);
    //if (init_data_type == kCenc)
    //{
    //    cenc_init* pHeader = (cenc_init*)init_data;

    //    std::cout << " | header: " << std::string(pHeader->header, 8) << " | version: " << pHeader->version << " | system_id: " << cdm::hexStr(pHeader->system_id, 16) << " | kid_count: " << pHeader->kid_count;

    //    /*for (int i = 0; i < pHeader->kid_count; ++i)
    //    {
    //    std::cout << " | kid: " << hexStr(init_data + sizeof(cenc_init), 16);
    //    }*/
    //}

    std::cout << std::endl;

    cdm_->CreateSessionAndGenerateRequest(promise_id, session_type, init_data_type, init_data, init_data_size);
}

void CdmProxy::LoadSession(uint32_t promise_id, SessionType session_type, const char* session_id, uint32_t session_id_size)
{
    std::cout << "[" << id_ << "] ";
    std::cout << "Cdm::LoadSession promise_id: " << promise_id <<
        " session_type: " << GetSessionTypeName(session_type) << " session_id: " << std::string(session_id, session_id_size) << std::endl;

    cdm_->LoadSession(promise_id, session_type, session_id, session_id_size);
}

void CdmProxy::UpdateSession(uint32_t promise_id, const char* session_id, uint32_t session_id_size, const uint8_t* response, uint32_t response_size)
{
    std::cout << "[" << id_ << "] ";
    std::cout << "Cdm::UpdateSession promise_id: " << promise_id << " session_id: " << std::string(session_id, session_id_size) <<
        " response: " << std::string((const char*)response, response_size) << std::endl;

    cdm_->UpdateSession(promise_id, session_id, session_id_size, response, response_size);
}

void CdmProxy::CloseSession(uint32_t promise_id, const char* session_id, uint32_t session_id_size)
{
    std::cout << "[" << id_ << "] ";
    std::cout << "Cdm::CloseSession promise_id: " << promise_id << " session_id: " << std::string(session_id, session_id_size) << std::endl;
    if (!is_offline_)
        cdm_->CloseSession(promise_id, session_id, session_id_size);
    else
        host_proxy_->OnResolvePromise(promise_id);
}

void CdmProxy::RemoveSession(uint32_t promise_id, const char* session_id, uint32_t session_id_size)
{
    std::cout << "[" << id_ << "] ";
    std::cout << "Cdm::RemoveSession promise_id: " << promise_id << " session_id: " << std::string(session_id, session_id_size) << std::endl;
    if (!is_offline_)
        cdm_->RemoveSession(promise_id, session_id, session_id_size);
}

void CdmProxy::TimerExpired(void* context)
{
    std::cout << "::TimerExpired " << std::endl;

    //cdm_->TimerExpired(context);
}

Status CdmProxy::Decrypt(const InputBuffer& encrypted_buffer, DecryptedBlock* decrypted_buffer)
{
    std::cout << "[" << id_ << "] ";
    std::cout << "Cdm::Decrypt Buffer! " << std::endl;

    //return kDecryptError;

   // ((uint8_t*)(encrypted_buffer.iv))[0] = 0;

    //"2a7fddffe2b426ac0000000000000000"

    InputBuffer ib = encrypted_buffer;

 
    std::vector<uint8_t>        data(encrypted_buffer.data, encrypted_buffer.data + encrypted_buffer.data_size);

    uint8_t* data_ptr = data.data();
    for (int i = 0; i < encrypted_buffer.num_subsamples; ++i) {
        memset(data_ptr, 0, encrypted_buffer.subsamples[i].clear_bytes);
        data_ptr += encrypted_buffer.subsamples[i].cipher_bytes + encrypted_buffer.subsamples[i].clear_bytes;
               
    }

    ib.data = data.data();
    //((SubsampleEntry*)ib.subsamples)[0].cipher_bytes = ib.subsamples[0].cipher_bytes + ib.subsamples[0].clear_bytes;
    //((SubsampleEntry*)ib.subsamples)[0].clear_bytes = 0;
    
 /*   if (encrypted_buffer.iv[0] != 0x2a)
        return kNoKey;*/

    Status status = cdm_->Decrypt(/*encrypted_buffer*/ib, decrypted_buffer);
        
    std::cout << "[" << id_ << "] " << "Cdm::Decrypt Status: " << GetStatusName(status) << " | timestamp: " << encrypted_buffer.timestamp << " | subsamples: " << encrypted_buffer.num_subsamples << " | data_size: " << encrypted_buffer.data_size
        << " | key_id: " << cdm::hexStr(encrypted_buffer.key_id, encrypted_buffer.key_id_size)
        << " | iv: " << cdm::hexStr(encrypted_buffer.iv, encrypted_buffer.iv_size) << std::endl;

    for (int i = 0; i < encrypted_buffer.num_subsamples; ++i) {
        std::cout << "subsample clear: " << encrypted_buffer.subsamples[i].clear_bytes << " cypher: " << encrypted_buffer.subsamples[i].cipher_bytes << std::endl;
    }

    if (status == kSuccess) {
        uint8_t* dst_ptr = decrypted_buffer->DecryptedBuffer()->Data();
        const uint8_t* src_ptr = encrypted_buffer.data;
        for (int i = 0; i < encrypted_buffer.num_subsamples; ++i) {
            memcpy(dst_ptr, src_ptr, encrypted_buffer.subsamples[i].clear_bytes);
            src_ptr += encrypted_buffer.subsamples[i].cipher_bytes + encrypted_buffer.subsamples[i].clear_bytes;
            dst_ptr += encrypted_buffer.subsamples[i].cipher_bytes + encrypted_buffer.subsamples[i].clear_bytes;

        }
    }
    

    return status;
}

Status CdmProxy::InitializeAudioDecoder(const AudioDecoderConfig& audio_decoder_config)
{
    Status status = kSessionError;

    std::cout << "[" << id_ << "] ";
    std::cout << "Cdm::InitializeAudioDecoder status: " << GetStatusName(status) << " config: " << GetAudioConfigName(audio_decoder_config) << std::endl;

   // return status;
    if (!is_offline_) {
        status = cdm_->InitializeAudioDecoder(audio_decoder_config);
    }

    ServiceReady();

    std::cout << "[" << id_ << "] ";
    std::cout << "Cdm::InitializeAudioDecoder status: " << GetStatusName(status) << " config: " << GetAudioConfigName(audio_decoder_config) << std::endl;
    return status;
}

Status CdmProxy::InitializeVideoDecoder(const VideoDecoderConfig& video_decoder_config)
{
    Status status = kSessionError;

   // status = kSuccess;
    std::cout << "[" << id_ << "] ";
    std::cout << "Cdm::InitializeVideoDecoder status: " << GetStatusName(status) << " config: " << GetVideoConfigName(video_decoder_config) << std::endl;


    //return status;
    if (!is_offline_) {
        status = cdm_->InitializeVideoDecoder(video_decoder_config);
    }

    ServiceReady();

    std::cout << "[" << id_ << "] ";
    std::cout << "Cdm::InitializeVideoDecoder status: " << GetStatusName(status) << " config: " << GetVideoConfigName(video_decoder_config) << std::endl;

    //Need To Return Not Decodec Frames;
    //return kSessionError;
    return status;
}

void CdmProxy::DeinitializeDecoder(StreamType decoder_type)
{
    std::cout << "[" << id_ << "] ";
    std::cout << "Cdm::DeinitializeDecoder decoder_type: " << GetStreamTypeName(decoder_type) << std::endl;

    if (!is_offline_)
        cdm_->DeinitializeDecoder(decoder_type);
}

void CdmProxy::ResetDecoder(StreamType decoder_type)
{
    std::cout << "[" << id_ << "] ";
    std::cout << "Cdm::ResetDecoder decoder_type: " << GetStreamTypeName(decoder_type) << std::endl;
    if (!is_offline_)
        cdm_->ResetDecoder(decoder_type);
}

class MyDecryptedBlock : public cdm::DecryptedBlock {
public:
    virtual void SetDecryptedBuffer(Buffer* buffer) { buffer_ = buffer; }
    virtual Buffer* DecryptedBuffer() { return buffer_; }

    // TODO(tomfinegan): Figure out if timestamp is really needed. If it is not,
    // we can just pass Buffer pointers around.
    virtual void SetTimestamp(int64_t timestamp) { ts_ = timestamp; }
    virtual int64_t Timestamp() const { return ts_; }

    MyDecryptedBlock() {}
    virtual ~MyDecryptedBlock() {}

private:
    int64_t                 ts_;
    cdm::Buffer*            buffer_;
};

Status CdmProxy::DecryptAndDecodeFrame(const InputBuffer& encrypted_buffer, VideoFrame* video_frame)
{
    Status status = kDecodeError;
 /*   MyDecryptedBlock block;

    status = Decrypt(encrypted_buffer, &block);

    return status;*/
    
    if (!is_offline_)
        status = cdm_->DecryptAndDecodeFrame(encrypted_buffer, video_frame);

    std::cout << "[" << id_ << "] " << "Cdm::DecryptAndDecodeFrame Status: " << GetStatusName(status) << " | timestamp: " << encrypted_buffer.timestamp << " | subsamples: " << encrypted_buffer.num_subsamples << " | data_size: " << encrypted_buffer.data_size
        << " | key_id: " << cdm::hexStr(encrypted_buffer.key_id, encrypted_buffer.key_id_size)
        << " | iv: " << cdm::hexStr(encrypted_buffer.iv, encrypted_buffer.iv_size) << std::endl;

  /*  std::cout << "[" << id_ << "] ";
    std::cout << "Cdm::DecryptAndDecodeFrame Video: " << GetStatusName(status) << std::endl;*/

    
    return status;
}

Status CdmProxy::DecryptAndDecodeSamples(const InputBuffer& encrypted_buffer, AudioFrames* audio_frames)
{
    Status status = kDecodeError;

    if (!is_offline_)
        status = cdm_->DecryptAndDecodeSamples(encrypted_buffer, audio_frames);

    std::cout << "[" << id_ << "] ";
    std::cout << "Cdm::DecryptAndDecodeSamples Audio: " << GetStatusName(status) << std::endl;
    return status;
}

void CdmProxy::OnPlatformChallengeResponse(const PlatformChallengeResponse& response)
{
    std::cout << "[" << id_ << "] ";
    std::cout << "Cdm::OnPlatformChallengeResponse" << std::endl;
    cdm_->OnPlatformChallengeResponse(response);
}

void CdmProxy::OnQueryOutputProtectionStatus(QueryResult result, uint32_t link_mask, uint32_t output_protection_mask)
{
    std::cout << "[" << id_ << "] ";
    std::cout << "Cdm::OnQueryOutputProtectionStatus result: " << GetQueryResultName(result) <<
        " | link_mask: " << GetLinkMaskNames(link_mask) << " | output_protection: " << output_protection_mask << std::endl;
    cdm_->OnQueryOutputProtectionStatus(result, link_mask, output_protection_mask);
}

void CdmProxy::Destroy()
{
    std::cout << "[" << id_ << "] ";
    std::cout << "Cdm::Destroy Cdm " << is_offline_ << std::endl;

    host_proxy_->Destroy();

    delete this;
}

void CdmProxy::ServiceReady()
{
    if (!is_offline_) {
        is_offline_ = true;
        host_proxy_->ServiceReady();
    }
}
