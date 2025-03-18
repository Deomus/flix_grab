#pragma once 
#include <string>
#include <iostream>
#include <queue>
#include <tuple> 
#include <mutex>
#include <condition_variable>

#include "content_decryption_module.h"
#include "cdm_ipc.h"

//class CdmHost;

// name 'CdmProxy' is already used in cdm
class CdmProxyDS : public ContentDecryptionModule, private HostIpcAbstract
{
public:
    // Initializes the CDM instance, providing information about permitted
    // functionalities.
    // If |allow_distinctive_identifier| is false, messages from the CDM,
    // such as message events, must not contain a Distinctive Identifier,
    // even in an encrypted form.
    // If |allow_persistent_state| is false, the CDM must not attempt to
    // persist state. Calls to CreateFileIO() will fail.
    virtual void Initialize(bool allow_distinctive_identifier,
        bool allow_persistent_state,
        bool use_hw_secure_codecs) override;

    // Gets the key status if the CDM has a hypothetical key with the |policy|.
     // The CDM must respond by calling either Host::OnResolveKeyStatusPromise()
     // with the result key status or Host::OnRejectPromise() if an unexpected
     // error happened or this method is not supported.
    virtual void GetStatusForPolicy(uint32_t promise_id,
        const cdm::Policy& policy) override;

    // SetServerCertificate(), CreateSessionAndGenerateRequest(), LoadSession(),
    // UpdateSession(), CloseSession(), and RemoveSession() all accept a
    // |promise_id|, which must be passed to the completion Host method
    // (e.g. Host::OnResolveNewSessionPromise()).

    // Provides a server certificate to be used to encrypt messages to the
    // license server. The CDM must respond by calling either
    // Host::OnResolvePromise() or Host::OnRejectPromise().
    virtual void SetServerCertificate(uint32_t promise_id,
        const uint8_t* server_certificate_data,
        uint32_t server_certificate_data_size) override;

    // Creates a session given |session_type|, |init_data_type|, and |init_data|.
    // The CDM must respond by calling either Host::OnResolveNewSessionPromise()
    // or Host::OnRejectPromise().
    virtual void CreateSessionAndGenerateRequest(uint32_t promise_id,
        cdm::SessionType session_type,
        cdm::InitDataType init_data_type,
        const uint8_t* init_data,
        uint32_t init_data_size) override;

    // Loads the session of type |session_type| specified by |session_id|.
    // The CDM must respond by calling either Host::OnResolveNewSessionPromise()
    // or Host::OnRejectPromise(). If the session is not found, call
    // Host::OnResolveNewSessionPromise() with session_id = NULL.
    virtual void LoadSession(uint32_t promise_id,
        cdm::SessionType session_type,
        const char* session_id,
        uint32_t session_id_size) override;

    // Updates the session with |response|. The CDM must respond by calling
    // either Host::OnResolvePromise() or Host::OnRejectPromise().
    virtual void UpdateSession(uint32_t promise_id,
        const char* session_id,
        uint32_t session_id_size,
        const uint8_t* response,
        uint32_t response_size);

    // Requests that the CDM close the session. The CDM must respond by calling
    // either Host::OnResolvePromise() or Host::OnRejectPromise() when the request
    // has been processed. This may be before the session is closed. Once the
    // session is closed, Host::OnSessionClosed() must also be called.
    virtual void CloseSession(uint32_t promise_id,
        const char* session_id,
        uint32_t session_id_size) override;

    // Removes any stored session data associated with this session. Will only be
    // called for persistent sessions. The CDM must respond by calling either
    // Host::OnResolvePromise() or Host::OnRejectPromise() when the request has
    // been processed.
    virtual void RemoveSession(uint32_t promise_id,
        const char* session_id,
        uint32_t session_id_size) override;

    // Performs scheduled operation with |context| when the timer fires.
    virtual void TimerExpired(void* context) override;

    // Decrypts the |encrypted_buffer|.
    //
    // Returns kSuccess if decryption succeeded, in which case the callee
    // should have filled the |decrypted_buffer| and passed the ownership of
    // |data| in |decrypted_buffer| to the caller.
    // Returns kNoKey if the CDM did not have the necessary decryption key
    // to decrypt.
    // Returns kDecryptError if any other error happened.
    // If the return value is not kSuccess, |decrypted_buffer| should be ignored
    // by the caller.
    virtual cdm::Status Decrypt(const cdm::InputBuffer_2& encrypted_buffer,
        cdm::DecryptedBlock* decrypted_buffer) override;

    // Initializes the CDM audio decoder with |audio_decoder_config|. This
    // function must be called before DecryptAndDecodeSamples() is called.
    //
    // Returns kSuccess if the |audio_decoder_config| is supported and the CDM
    // audio decoder is successfully initialized.
    // Returns kSessionError if |audio_decoder_config| is not supported. The CDM
    // may still be able to do Decrypt().
    // Returns kDeferredInitialization if the CDM is not ready to initialize the
    // decoder at this time. Must call Host::OnDeferredInitializationDone() once
    // initialization is complete.
    virtual cdm::Status InitializeAudioDecoder(
        const AudioDecoderConfig& audio_decoder_config) override;

    // Initializes the CDM video decoder with |video_decoder_config|. This
    // function must be called before DecryptAndDecodeFrame() is called.
    //
    // Returns kSuccess if the |video_decoder_config| is supported and the CDM
    // video decoder is successfully initialized.
    // Returns kSessionError if |video_decoder_config| is not supported. The CDM
    // may still be able to do Decrypt().
    // Returns kDeferredInitialization if the CDM is not ready to initialize the
    // decoder at this time. Must call Host::OnDeferredInitializationDone() once
    // initialization is complete.
    virtual cdm::Status InitializeVideoDecoder(
        const VideoDecoderConfig& video_decoder_config) override;

    // De-initializes the CDM decoder and sets it to an uninitialized state. The
    // caller can initialize the decoder again after this call to re-initialize
    // it. This can be used to reconfigure the decoder if the configuration
    // changes.
    virtual void DeinitializeDecoder(cdm::StreamType decoder_type) override;

    // Resets the CDM decoder to an initialized clean state. All internal buffers
    // MUST be flushed.
    virtual void ResetDecoder(cdm::StreamType decoder_type) override;

    // Decrypts the |encrypted_buffer| and decodes the decrypted buffer into a
    // |video_frame|. Upon end-of-stream, the caller should call this function
    // repeatedly with empty |encrypted_buffer| (|data| == NULL) until only empty
    // |video_frame| (|format| == kEmptyVideoFrame) is produced.
    //
    // Returns kSuccess if decryption and decoding both succeeded, in which case
    // the callee will have filled the |video_frame| and passed the ownership of
    // |frame_buffer| in |video_frame| to the caller.
    // Returns kNoKey if the CDM did not have the necessary decryption key
    // to decrypt.
    // Returns kNeedMoreData if more data was needed by the decoder to generate
    // a decoded frame (e.g. during initialization and end-of-stream).
    // Returns kDecryptError if any decryption error happened.
    // Returns kDecodeError if any decoding error happened.
    // If the return value is not kSuccess, |video_frame| should be ignored by
    // the caller.
    virtual cdm::Status DecryptAndDecodeFrame(const cdm::InputBuffer_2& encrypted_buffer,
        cdm::VideoFrame* video_frame) override;

    // Decrypts the |encrypted_buffer| and decodes the decrypted buffer into
    // |audio_frames|. Upon end-of-stream, the caller should call this function
    // repeatedly with empty |encrypted_buffer| (|data| == NULL) until only empty
    // |audio_frames| is produced.
    //
    // Returns kSuccess if decryption and decoding both succeeded, in which case
    // the callee will have filled |audio_frames| and passed the ownership of
    // |data| in |audio_frames| to the caller.
    // Returns kNoKey if the CDM did not have the necessary decryption key
    // to decrypt.
    // Returns kNeedMoreData if more data was needed by the decoder to generate
    // audio samples (e.g. during initialization and end-of-stream).
    // Returns kDecryptError if any decryption error happened.
    // Returns kDecodeError if any decoding error happened.
    // If the return value is not kSuccess, |audio_frames| should be ignored by
    // the caller.
    virtual cdm::Status DecryptAndDecodeSamples(const InputBuffer& encrypted_buffer,
        cdm::AudioFrames* audio_frames) override;

    // Called by the host after a platform challenge was initiated via
    // Host::SendPlatformChallenge().
    virtual void OnPlatformChallengeResponse(
        const cdm::PlatformChallengeResponse& response) override;

    // Called by the host after a call to Host::QueryOutputProtectionStatus(). The
    // |link_mask| is a bit mask of OutputLinkTypes and |output_protection_mask|
    // is a bit mask of OutputProtectionMethods. If |result| is kQueryFailed,
    // then |link_mask| and |output_protection_mask| are undefined and should
    // be ignored.
    virtual void OnQueryOutputProtectionStatus(
        cdm::QueryResult result,
        uint32_t link_mask,
        uint32_t output_protection_mask) override;

    // Called by the host after a call to Host::RequestStorageId(). If the
    // version of the storage ID requested is available, |storage_id| and
    // |storage_id_size| are set appropriately. |version| will be the same as
    // what was requested, unless 0 (latest) was requested, in which case
    // |version| will be the actual version number for the |storage_id| returned.
    // If the requested version is not available, null/zero will be provided as
    // |storage_id| and |storage_id_size|, respectively, and |version| should be
    // ignored.
    virtual void OnStorageId(uint32_t version,
        const uint8_t* storage_id,
        uint32_t storage_id_size) override;

    // Destroys the object in the same context as it was created.
    virtual void Destroy() override;


private:
    // Called by the CDM with the result after the CDM instance was initialized.
    virtual void OnInitialized(bool success)override;

    // Called by the CDM when a key status is available in response to
    // GetStatusForPolicy().
    virtual void OnResolveKeyStatusPromise(uint32_t promise_id,
        cdm::KeyStatus key_status)override;

    // Called by the CDM when a session is created or loaded and the value for the
    // MediaKeySession's sessionId attribute is available (|session_id|).
    // This must be called before OnSessionMessage() or
    // OnSessionKeysChange() is called for the same session. |session_id_size|
    // should not include null termination.
    // When called in response to LoadSession(), the |session_id| must be the
    // same as the |session_id| passed in LoadSession(), or NULL if the
    // session could not be loaded.
    virtual void OnResolveNewSessionPromise(uint32_t promise_id, const BufferHolder& session_id) override;


    // Called by the CDM when a session is updated or released.
    virtual void OnResolvePromise(uint32_t promise_id) override;

    // Called by the CDM when an error occurs as a result of one of the
    // ContentDecryptionModule calls that accept a |promise_id|.
    // |error| must be specified, |error_message| and |system_code|
    // are optional. |error_message_size| should not include null termination.
    virtual void OnRejectPromise(uint32_t promise_id, cdm::Exception exception,
        uint32_t system_code, const BufferHolder& error_message) override;

    // Called by the CDM when it has a message for session |session_id|.
    // Size parameters should not include null termination.
    // |legacy_destination_url| is only for supporting the prefixed EME API and
    // is ignored by unprefixed EME. It should only be non-null if |message_type|
    // is kLicenseRenewal.
    virtual void OnSessionMessage(const BufferHolder& session_id,
        cdm::MessageType message_type,
        const BufferHolder& message // const BufferHolder& legacy_destination_url,
        ) override;

    // Called by the CDM when there has been a change in keys or their status for
    // session |session_id|. |has_additional_usable_key| should be set if a
    // key is newly usable (e.g. new key available, previously expired key has
    // been renewed, etc.) and the browser should attempt to resume playback.
    // |key_ids| is the list of key ids for this session along with their
    // current status. |key_ids_count| is the number of entries in |key_ids|.
    // Size parameter for |session_id| should not include null termination.

    //Change Only One Key;
    virtual void OnSessionKeyChange(const BufferHolder& session_id, const BufferHolder& key_id,
        cdm::KeyStatus key_status, uint32_t system_code) override;

    // Called by the CDM when there has been a change in the expiration time for
    // session |session_id|. This can happen as the result of an Update() call
    // or some other event. If this happens as a result of a call to Update(),
    // it must be called before resolving the Update() promise. |new_expiry_time|
    // can be 0 to represent "undefined". Size parameter should not include
    // null termination.
    virtual void OnExpirationChange(const BufferHolder& session_id,
        double new_expiry_time) override;

    // Called by the CDM when session |session_id| is closed. Size
    // parameter should not include null termination.
    virtual void OnSessionClosed(const BufferHolder& session_id) override;

    // Called by the CDM when an error occurs in session |session_id|
    // unrelated to one of the ContentDecryptionModule calls that accept a
    // |promise_id|. |error| must be specified, |error_message| and
    // |system_code| are optional. Length parameters should not include null
    // termination.
    // Note:
    // - This method is only for supporting prefixed EME API.
    // - This method will be ignored by unprefixed EME. All errors reported
    //   in this method should probably also be reported by one of other methods.
    //virtual void OnLegacySessionError(
    //    const BufferHolder& session_id,
    //    cdm::Exception exception,
    //    uint32_t system_code,
    //    const BufferHolder& error_message) override;

    // The following are optional methods that may not be implemented on all
    // platforms.

    // Sends a platform challenge for the given |service_id|. |challenge| is at
    // most 256 bits of data to be signed. Once the challenge has been completed,
    // the host will call ContentDecryptionModule::OnPlatformChallengeResponse()
    // with the signed challenge response and platform certificate. Size
    // parameters should not include null termination.
    virtual void SendPlatformChallenge(const BufferHolder& service_id,
        const BufferHolder& challenge) override;

    // Attempts to enable output protection (e.g. HDCP) on the display link. The
    // |desired_protection_mask| is a bit mask of OutputProtectionMethods. No
    // status callback is issued, the CDM must call QueryOutputProtectionStatus()
    // periodically to ensure the desired protections are applied.
    virtual void EnableOutputProtection(uint32_t desired_protection_mask) override;

    // Requests the current output protection status. Once the host has the status
    // it will call ContentDecryptionModule::OnQueryOutputProtectionStatus().
    /*  virtual void QueryOutputProtectionStatus() {
    std::cout << "[" << this << "] ";
    std::cout << "Host::QueryOutputProtectionStatus: " << std::endl;
    host_->QueryOutputProtectionStatus();
    }*/

    virtual void RequestStorageId(uint32_t version) override;
  /*  virtual void Destroy() {
        delete this;
    }
*/



public:
#ifdef FILELOGGER
    CdmProxyDS(ipc::Client& cdm_client, const std::string& key_system, ContentDecryptionModule::Host* host, std::ofstream& file_logger_);
#else
    CdmProxyDS(ipc::Client& cdm_client, const std::string& key_system, ContentDecryptionModule::Host* host);
#endif

    virtual ~CdmProxyDS();

private:
    void    InitializeInternal(const std::string& key_system);

private:
    void    ServiceReady();
    
    void    AddQueuedCall(std::function<void()> f, bool = true);
    void    QueuedCallEnd();
    void    MessageLoop4Callbacks();    // to ensure that callbacks to host will be in the same trade as calls to cdm
    //void  WaitComplete(std::condition_variable& cv);
private:
#ifdef FILELOGGER
    std::ofstream                           file_logger_;
#endif
    bool                                    is_initialized = false;
    ipc::Client&                            cdm_client_;
    uintptr_t                               cdm_this_; // pointer to CdmIpcAbstract on remote side. This pointer is sending on every remote call.
    
    ContentDecryptionModule::Host*     host_;

    std::queue<std::function<void()>>   q_;

    // notify caller:
    std::mutex mq_;
    std::condition_variable cv_;
    // to protect the queue for multithreaded calls:
    std::mutex mqT_;
    std::condition_variable cvT_;
    
    bool _differCSAndGR = false;
    uint32_t promise_id_req;
    cdm::SessionType session_type_req;
    cdm::InitDataType init_data_type_req;
    std::vector<uint8_t> _buffCSAndGR;
    bool                            allow_persistent_state_;
};


