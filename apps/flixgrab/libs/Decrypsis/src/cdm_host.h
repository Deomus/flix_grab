#pragma once 

#include <windows.h>

#include <string>
#include <vector>
#include <cstdint>
#include <atomic>
#include <mutex>

#include "content_decryption_module.h"
#include "content_decryption_module_ext.h"

//#include <limits.h>
//#include <iostream>
#include "struct_strings.h"

#include "buffer_pooling.h"

#include "cdm_ipc.h"
#include "abstract_service.h"

// The host is created on widewide.dll side

class CdmHost : 
    private ContentDecryptionModule::Host,
    private CdmIpcAbstract,
    public AbstractDecryptService,
    public CdmHostForPrivateKeys
{

public:
    static uintptr_t CreateCdmIpcInstance(  const std::string& key_system, 
                                            const std::string& host_name, 
                                            uintptr_t host_this,
                                            bool verify_cdm = false);

private:
    CdmHost(const std::string& key_system, const std::string& host_name, uintptr_t host_this, bool verify_cdm = false);
    virtual ~CdmHost();

private:
    // ContentDecryptionModule::Host methods:
    virtual cdm::Buffer* Allocate(uint32_t capacity);

    // Requests the host to call ContentDecryptionModule::TimerFired() |delay_ms|
    // from now with |context|.
    virtual void SetTimer(int64_t delay_ms, void* context);

    // Returns the current wall time in seconds.
    virtual cdm::Time GetCurrentWallTime();
    
    // Called by the CDM with the result after the CDM instance was initialized.
    virtual void OnInitialized(bool success) override;

    // Called by the CDM when a key status is available in response to
    // GetStatusForPolicy().
    virtual void OnResolveKeyStatusPromise(uint32_t promise_id,
        cdm::KeyStatus key_status) override;

    // Called by the CDM when a session is created or loaded and the value for the
    // MediaKeySession's sessionId attribute is available (|session_id|).
    // This must be called before OnSessionMessage() or
    // OnSessionKeysChange() is called for the same session. |session_id_size|
    // should not include null termination.
    // When called in response to LoadSession(), the |session_id| must be the
    // same as the |session_id| passed in LoadSession(), or NULL if the
    // session could not be loaded.
    virtual void OnResolveNewSessionPromise(uint32_t promise_id,
        const char* session_id,
        uint32_t session_id_size);

    // Called by the CDM when a session is updated or released.
    virtual void OnResolvePromise(uint32_t promise_id);

    // Called by the CDM when an error occurs as a result of one of the
    // ContentDecryptionModule calls that accept a |promise_id|.
    // |error| must be specified, |error_message| and |system_code|
    // are optional. |error_message_size| should not include null termination.
    virtual void OnRejectPromise(uint32_t promise_id,
        cdm::Exception exception,
        uint32_t system_code,
        const char* error_message,
        uint32_t error_message_size);

    // Called by the CDM when it has a message for session |session_id|.
    // Size parameters should not include null termination.
    // |legacy_destination_url| is only for supporting the prefixed EME API and
    // is ignored by unprefixed EME. It should only be non-null if |message_type|
    // is kLicenseRenewal.
    virtual void OnSessionMessage(const char* session_id,
        uint32_t session_id_size,
        cdm::MessageType message_type,
        const char* message,
        uint32_t message_size);
    //,const char* legacy_destination_url,
        //uint32_t legacy_destination_url_length

    // Called by the CDM when there has been a change in keys or their status for
    // session |session_id|. |has_additional_usable_key| should be set if a
    // key is newly usable (e.g. new key available, previously expired key has
    // been renewed, etc.) and the browser should attempt to resume playback.
    // |key_ids| is the list of key ids for this session along with their
    // current status. |key_ids_count| is the number of entries in |key_ids|.
    // Size parameter for |session_id| should not include null termination.
    virtual void OnSessionKeysChange(const char* session_id,
        uint32_t session_id_size,
        bool has_additional_usable_key,
        const cdm::KeyInformation* keys_info,
        uint32_t keys_info_count);

    // Called by the CDM when there has been a change in the expiration time for
    // session |session_id|. This can happen as the result of an Update() call
    // or some other event. If this happens as a result of a call to Update(),
    // it must be called before resolving the Update() promise. |new_expiry_time|
    // can be 0 to represent "undefined". Size parameter should not include
    // null termination.
    virtual void OnExpirationChange(const char* session_id,
        uint32_t session_id_size,
        cdm::Time new_expiry_time);

    // Called by the CDM when session |session_id| is closed. Size
    // parameter should not include null termination.
    virtual void OnSessionClosed(const char* session_id,
        uint32_t session_id_size);

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
    //    const char* session_id, uint32_t session_id_length,
    //    cdm::Exception exception,
    //    uint32_t system_code,
    //    const char* error_message, uint32_t error_message_length);

    // The following are optional methods that may not be implemented on all
    // platforms.

    // Sends a platform challenge for the given |service_id|. |challenge| is at
    // most 256 bits of data to be signed. Once the challenge has been completed,
    // the host will call ContentDecryptionModule::OnPlatformChallengeResponse()
    // with the signed challenge response and platform certificate. Size
    // parameters should not include null termination.
    virtual void SendPlatformChallenge(const char* service_id,
        uint32_t service_id_size,
        const char* challenge,
        uint32_t challenge_size);

    // Attempts to enable output protection (e.g. HDCP) on the display link. The
    // |desired_protection_mask| is a bit mask of OutputProtectionMethods. No
    // status callback is issued, the CDM must call QueryOutputProtectionStatus()
    // periodically to ensure the desired protections are applied.
    virtual void EnableOutputProtection(uint32_t desired_protection_mask);

    // Requests the current output protection status. Once the host has the status
    // it will call ContentDecryptionModule::OnQueryOutputProtectionStatus().
    virtual void QueryOutputProtectionStatus();

    // Must be called by the CDM if it returned kDeferredInitialization during
    // InitializeAudioDecoder() or InitializeVideoDecoder().
    virtual void OnDeferredInitializationDone(cdm::StreamType stream_type,
        cdm::Status decoder_status);

    // Creates a FileIO object from the host to do file IO operation. Returns NULL
    // if a FileIO object cannot be obtained. Once a valid FileIO object is
    // returned, |client| must be valid until FileIO::Close() is called. The
    // CDM can call this method multiple times to operate on different files.
    virtual cdm::FileIO* CreateFileIO(cdm::FileIOClient* client);
    
    // Requests a specific version of the storage ID. A storage ID is a stable,
    // device specific ID used by the CDM to securely store persistent data. The
    // ID will be returned by the host via ContentDecryptionModule::OnStorageId().
    // If |version| is 0, the latest version will be returned. All |version|s
    // that are greater than or equal to 0x80000000 are reserved for the CDM and
    // should not be supported or returned by the host. The CDM must not expose
    // the ID outside the client device, even in encrypted form.
    virtual void RequestStorageId(uint32_t version);
    // ContentDecryptionModule::Host methods end


public:
    // AbstractDecryptService Interface:
    virtual int32_t             InitializeVideoDecoder(const VideoConfig& config);
    virtual int32_t             InitializeAudioDecoder(int codec, int channel_count, int bits_per_channel, int samples_per_second, std::vector<uint8_t> extra_data);
    virtual int32_t             DecodeVideo(const EncryptedData& encrypted, FrameSource_fn& frame_sink);
    virtual int32_t             Decrypt(const EncryptedData& encrypted, DecryptedData& decrypted);
    virtual void                GetKeyIds(Keys& keys);
    virtual void                Finish();
    virtual bool                KeysOK();

private:
    // CdmIpcAbstract Interface, looks like ContentDecryptionModule but not the same:
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

    virtual void GetStatusForPolicy(uint32_t promise_id, const cdm::Policy& policy) override;

    // SetServerCertificate(), CreateSessionAndGenerateRequest(), LoadSession(),
    // UpdateSession(), CloseSession(), and RemoveSession() all accept a
    // |promise_id|, which must be passed to the completion Host method
    // (e.g. Host::OnResolveNewSessionPromise()).
    // Provides a server certificate to be used to encrypt messages to the
    // license server. The CDM must respond by calling either
    // Host::OnResolvePromise() or Host::OnRejectPromise().
    virtual void SetServerCertificate(uint32_t promise_id, const BufferHolder& server_certificate) override;

    // Creates a session given |session_type|, |init_data_type|, and |init_data|.
    // The CDM must respond by calling either Host::OnResolveNewSessionPromise()
    // or Host::OnRejectPromise().
    virtual void CreateSessionAndGenerateRequest(uint32_t promise_id,
        cdm::SessionType session_type,
        cdm::InitDataType init_data_type, const BufferHolder& init_data,
        uint32_t version, const BufferHolder& storage_id // + OnStorageId params
    ) override;

    // Loads the session of type |session_type| specified by |session_id|.
    // The CDM must respond by calling either Host::OnResolveNewSessionPromise()
    // or Host::OnRejectPromise(). If the session is not found, call
    // Host::OnResolveNewSessionPromise() with session_id = NULL.
    virtual void LoadSession(uint32_t promise_id,
        cdm::SessionType session_type, const BufferHolder& session_id) override;

    // Updates the session with |response|. The CDM must respond by calling
    // either Host::OnResolvePromise() or Host::OnRejectPromise().
    virtual void UpdateSession(uint32_t promise_id,
        const BufferHolder& session_id,
        const BufferHolder& response) override;


    //// Requests that the CDM close the session. The CDM must respond by calling
    //// either Host::OnResolvePromise() or Host::OnRejectPromise() when the request
    //// has been processed. This may be before the session is closed. Once the
    //// session is closed, Host::OnSessionClosed() must also be called.
    //virtual void CloseSession(uint32_t promise_id, const BufferHolder& session_id) = 0;

    //// Removes any stored session data associated with this session. Will only be
    //// called for persistent sessions. The CDM must respond by calling either
    //// Host::OnResolvePromise() or Host::OnRejectPromise() when the request has
    //// been processed.
    //virtual void RemoveSession(uint32_t promise_id, const BufferHolder& session_id) = 0;

    virtual void OnStorageId(uint32_t version, const BufferHolder&) override;
    virtual void Destroy() override;
    virtual void VerifyCdmHost() override;

private:
    cdm::Time   GetNowTime() const;

        //Need for timeline concistency;
    void        IncreaseTime(cdm::Time  delta) { current_time_ += delta; }

public: 
    static std::wstring ExtractWideVine();

public:
    // Private Key support
    void OnSessionMessageFromPK(const char* session_id, uint32_t session_id_size, const char* message, uint32_t message_size);

private:
    void            InitializeInternal(const std::string& key_system, bool verify_cdm = false);
    static void*    GetHost(int host_interface_version, void* user_data);

    static cdm::HostFile CreateHostFile(const std::wstring& path, const std::wstring& path_sig);
    static std::wstring ExtractResourceToTemp(int resource_id, const std::wstring& file_name);
    static void CdmHost::CleanUpResourceInTemp();
private:
    cdm::Time                                       expiry_time_;
    cdm::Time                                       current_time_;

    ContentDecryptionModule*                        cdm_;
    ipc::Client                                     host_client_;
    uintptr_t                                       host_this_; // pointer to HostIpcAbstract on remote side.
 
    //TrueCdmHost                                       true_host_;

    Keys                                            key_ids_;
    std::shared_ptr<BufferPooling>                  buffer_pooling_;
    static std::mutex                               lock_;
private:
    bool                                            is_ipc_host_valid_ = true;
    std::atomic_flag                                destroy_flag_ = ATOMIC_FLAG_INIT;

    //std::thread                                   tOnStorage;
    
    //int totalDataSize = 0;
    bool nokeys_detected_ = false;
    uint32_t _storage_version; // params for differed OnStorageId call
    std::vector<uint8_t> _storage_id; // params for differed OnStorageId call

    HMODULE                                         module_;
/*
    struct SessionResponse {
        std::vector<char> session;
        int promise_id;
        std::vector<uint8_t> response;
    };
    std::vector<SessionResponse>                    session_response_; // vector<session, promise, responce>
    bool                                            reloading_keys_ = false;*/
};