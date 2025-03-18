#pragma once 
#include "content_decryption_module.h"

#include <string>
#include <iostream>

class HostProxy;

class CdmProxy : public cdm::ContentDecryptionModule
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
        bool allow_persistent_state);

    // SetServerCertificate(), CreateSessionAndGenerateRequest(), LoadSession(),
    // UpdateSession(), CloseSession(), and RemoveSession() all accept a
    // |promise_id|, which must be passed to the completion Host method
    // (e.g. Host::OnResolveNewSessionPromise()).

    // Provides a server certificate to be used to encrypt messages to the
    // license server. The CDM must respond by calling either
    // Host::OnResolvePromise() or Host::OnRejectPromise().
    virtual void SetServerCertificate(uint32_t promise_id,
        const uint8_t* server_certificate_data,
        uint32_t server_certificate_data_size);

    // Creates a session given |session_type|, |init_data_type|, and |init_data|.
    // The CDM must respond by calling either Host::OnResolveNewSessionPromise()
    // or Host::OnRejectPromise().
    virtual void CreateSessionAndGenerateRequest(uint32_t promise_id,
        cdm::SessionType session_type,
        cdm::InitDataType init_data_type,
        const uint8_t* init_data,
        uint32_t init_data_size);

    // Loads the session of type |session_type| specified by |session_id|.
    // The CDM must respond by calling either Host::OnResolveNewSessionPromise()
    // or Host::OnRejectPromise(). If the session is not found, call
    // Host::OnResolveNewSessionPromise() with session_id = NULL.
    virtual void LoadSession(uint32_t promise_id,
        cdm::SessionType session_type,
        const char* session_id,
        uint32_t session_id_size);

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
        uint32_t session_id_size);

    // Removes any stored session data associated with this session. Will only be
    // called for persistent sessions. The CDM must respond by calling either
    // Host::OnResolvePromise() or Host::OnRejectPromise() when the request has
    // been processed.
    virtual void RemoveSession(uint32_t promise_id,
        const char* session_id,
        uint32_t session_id_size);

    // Performs scheduled operation with |context| when the timer fires.
    virtual void TimerExpired(void* context);

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
    virtual cdm::Status Decrypt(const cdm::InputBuffer& encrypted_buffer,
        cdm::DecryptedBlock* decrypted_buffer);

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
        const cdm::AudioDecoderConfig& audio_decoder_config);

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
        const cdm::VideoDecoderConfig& video_decoder_config);

    // De-initializes the CDM decoder and sets it to an uninitialized state. The
    // caller can initialize the decoder again after this call to re-initialize
    // it. This can be used to reconfigure the decoder if the configuration
    // changes.
    virtual void DeinitializeDecoder(cdm::StreamType decoder_type);

    // Resets the CDM decoder to an initialized clean state. All internal buffers
    // MUST be flushed.
    virtual void ResetDecoder(cdm::StreamType decoder_type);

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
    virtual cdm::Status DecryptAndDecodeFrame(const cdm::InputBuffer& encrypted_buffer,
        cdm::VideoFrame* video_frame);

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
    virtual cdm::Status DecryptAndDecodeSamples(const cdm::InputBuffer& encrypted_buffer,
        cdm::AudioFrames* audio_frames);

    // Called by the host after a platform challenge was initiated via
    // Host::SendPlatformChallenge().
    virtual void OnPlatformChallengeResponse(
        const cdm::PlatformChallengeResponse& response);

    // Called by the host after a call to Host::QueryOutputProtectionStatus(). The
    // |link_mask| is a bit mask of OutputLinkTypes and |output_protection_mask|
    // is a bit mask of OutputProtectionMethods. If |result| is kQueryFailed,
    // then |link_mask| and |output_protection_mask| are undefined and should
    // be ignored.
    virtual void OnQueryOutputProtectionStatus(
        cdm::QueryResult result,
        uint32_t link_mask,
        uint32_t output_protection_mask);

    // Destroys the object in the same context as it was created.
    virtual void Destroy();



public:
    CdmProxy(cdm::ContentDecryptionModule* cdm, HostProxy* host_proxy);

    virtual ~CdmProxy();

private:
    void    ServiceReady();
   
private:
    bool                                    is_offline_;
    cdm::ContentDecryptionModule*           cdm_;
    HostProxy*                              host_proxy_;

    int                                     id_;

};


