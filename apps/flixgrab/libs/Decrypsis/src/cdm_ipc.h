#pragma once

#include "content_decryption_module.h"
#include "ipc/ipc.h"

#include "struct_strings.h"

#define DEFAULT_CDM_IPC                 "getflix_cdm_ipc"
#define DEFAULT_CDM_HOST_IPC            "getflix_cdm_host_ipc"


struct BufferHolder {
    BufferHolder(uint8_t const* ptr, uint32_t size)
        : ptr_(ptr)
        , size_(size) {}

    BufferHolder(char const* ptr, uint32_t size)
        : ptr_(ptr)
        , size_(size) {}

    BufferHolder()
        : ptr_(nullptr)
        , size_(0) {}

    BufferHolder(const std::string& buffer)
        : ptr_(buffer.data())
        , size_(buffer.size()) {}

    const uint8_t*  ptr() const { return reinterpret_cast<const uint8_t*>(ptr_); }
    const char*     pchar() const  { return reinterpret_cast<const char*>(ptr_); }
    uint32_t        size() const { return size_; }

    std::string     to_string() const { return std::string(pchar(), size_); }

private:

    void const*             ptr_;
    uint32_t                size_;
};

template<>
struct ipc::ValueTraits<BufferHolder> {
    template<typename B>
    static bool Read(B& b, BufferHolder&& v) {
        const uint8_t* ptr;
        size_t size = 0;
        if (b.GetBlock(ptr, size)) {
            v = BufferHolder(ptr, size);
            return true;
        }
        return false;
    }

    template<typename B>
    static bool Write(B& b, const BufferHolder&& v) {
        if (b.Pod(v.size()) &&
            b.AppendArray((int8_t*)v.ptr(), v.size()))
            return true;

        return false;
    }
};


class CdmIpcAbstract {
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
        bool use_hw_secure_codecs) = 0;
    // SetServerCertificate(), CreateSessionAndGenerateRequest(), LoadSession(),
    // UpdateSession(), CloseSession(), and RemoveSession() all accept a
    // |promise_id|, which must be passed to the completion Host method
    // (e.g. Host::OnResolveNewSessionPromise()).

  // Gets the key status if the CDM has a hypothetical key with the |policy|.
  // The CDM must respond by calling either Host::OnResolveKeyStatusPromise()
  // with the result key status or Host::OnRejectPromise() if an unexpected
  // error happened or this method is not supported.
    virtual void GetStatusForPolicy(uint32_t promise_id,
        const cdm::Policy& policy) = 0;

    // Provides a server certificate to be used to encrypt messages to the
    // license server. The CDM must respond by calling either
    // Host::OnResolvePromise() or Host::OnRejectPromise().
    virtual void SetServerCertificate(uint32_t promise_id, const BufferHolder& server_certificate) = 0;

    // Creates a session given |session_type|, |init_data_type|, and |init_data|.
    // The CDM must respond by calling either Host::OnResolveNewSessionPromise()
    // or Host::OnRejectPromise().
    virtual void CreateSessionAndGenerateRequest(uint32_t promise_id,
        cdm::SessionType session_type,
        cdm::InitDataType init_data_type, const BufferHolder& init_data,
        uint32_t version, const BufferHolder& storage_id // + OnStorageId params
    ) = 0;

    // Loads the session of type |session_type| specified by |session_id|.
    // The CDM must respond by calling either Host::OnResolveNewSessionPromise()
    // or Host::OnRejectPromise(). If the session is not found, call
    // Host::OnResolveNewSessionPromise() with session_id = NULL.
    virtual void LoadSession(uint32_t promise_id,
        cdm::SessionType session_type, const BufferHolder& session_id) = 0;

    // Updates the session with |response|. The CDM must respond by calling
    // either Host::OnResolvePromise() or Host::OnRejectPromise().
    virtual void UpdateSession(uint32_t promise_id,
        const BufferHolder& session_id,
        const BufferHolder& response) = 0;

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

    virtual void OnStorageId(uint32_t version, const BufferHolder& storage_id) = 0;

    virtual void Destroy() = 0;
  
    virtual void VerifyCdmHost() = 0;

    virtual ~CdmIpcAbstract() {  }

};

class HostIpcAbstract {
public:
    // Called by the CDM with the result after the CDM instance was initialized.
    virtual void OnInitialized(bool success) = 0;

    // Called by the CDM when a key status is available in response to
    // GetStatusForPolicy().
    virtual void OnResolveKeyStatusPromise(uint32_t promise_id,
        cdm::KeyStatus key_status) = 0;

    // Called by the CDM when a session is created or loaded and the value for the
    // MediaKeySession's sessionId attribute is available (|session_id|).
    // This must be called before OnSessionMessage() or
    // OnSessionKeysChange() is called for the same session. |session_id_size|
    // should not include null termination.
    // When called in response to LoadSession(), the |session_id| must be the
    // same as the |session_id| passed in LoadSession(), or NULL if the
    // session could not be loaded.
    virtual void OnResolveNewSessionPromise(uint32_t promise_id, const BufferHolder& session_id) = 0;

    // Called by the CDM when a session is updated or released.
    virtual void OnResolvePromise(uint32_t promise_id) = 0;

    // Called by the CDM when an error occurs as a result of one of the
    // ContentDecryptionModule calls that accept a |promise_id|.
    // |error| must be specified, |error_message| and |system_code|
    // are optional. |error_message_size| should not include null termination.
    virtual void OnRejectPromise(uint32_t promise_id, cdm::Exception exception,
        uint32_t system_code, const BufferHolder& error_message) = 0;

    // Called by the CDM when it has a message for session |session_id|.
    // Size parameters should not include null termination.
    // |legacy_destination_url| is only for supporting the prefixed EME API and
    // is ignored by unprefixed EME. It should only be non-null if |message_type|
    // is kLicenseRenewal.
    virtual void OnSessionMessage(const BufferHolder& session_id,
        cdm::MessageType message_type,
        const BufferHolder& message // ,const BufferHolder& legacy_destination_url
        ) = 0;

    // Called by the CDM when there has been a change in keys or their status for
    // session |session_id|. |has_additional_usable_key| should be set if a
    // key is newly usable (e.g. new key available, previously expired key has
    // been renewed, etc.) and the browser should attempt to resume playback.
    // |key_ids| is the list of key ids for this session along with their
    // current status. |key_ids_count| is the number of entries in |key_ids|.
    // Size parameter for |session_id| should not include null termination.

    //Change Only One Key;
    virtual void OnSessionKeyChange(const BufferHolder& session_id, const BufferHolder& key_id,
        cdm::KeyStatus key_status, uint32_t system_code) = 0;

    // Called by the CDM when there has been a change in the expiration time for
    // session |session_id|. This can happen as the result of an Update() call
    // or some other event. If this happens as a result of a call to Update(),
    // it must be called before resolving the Update() promise. |new_expiry_time|
    // can be 0 to represent "undefined". Size parameter should not include
    // null termination.
    virtual void OnExpirationChange(const BufferHolder& session_id,
        double new_expiry_time) = 0;

    // Called by the CDM when session |session_id| is closed. Size
    // parameter should not include null termination.
    virtual void OnSessionClosed(const BufferHolder& session_id) = 0;

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
    //    cdm::Exception error,
    //    uint32_t system_code,
    //    const BufferHolder& error_message) = 0;

    // The following are optional methods that may not be implemented on all
    // platforms.

    // Sends a platform challenge for the given |service_id|. |challenge| is at
    // most 256 bits of data to be signed. Once the challenge has been completed,
    // the host will call ContentDecryptionModule::OnPlatformChallengeResponse()
    // with the signed challenge response and platform certificate. Size
    // parameters should not include null termination.
    virtual void SendPlatformChallenge(const BufferHolder& service_id,
        const BufferHolder& challenge) = 0;

    // Attempts to enable output protection (e.g. HDCP) on the display link. The
    // |desired_protection_mask| is a bit mask of OutputProtectionMethods. No
    // status callback is issued, the CDM must call QueryOutputProtectionStatus()
    // periodically to ensure the desired protections are applied.
    virtual void EnableOutputProtection(uint32_t desired_protection_mask) = 0;

    // Requests the current output protection status. Once the host has the status
    // it will call ContentDecryptionModule::OnQueryOutputProtectionStatus().
    /*  virtual void QueryOutputProtectionStatus() {
    std::cout << "[" << this << "] ";
    std::cout << "Host::QueryOutputProtectionStatus: " << std::endl;
    host_->QueryOutputProtectionStatus();
    }*/

    virtual void RequestStorageId(uint32_t version) = 0;

    virtual void Destroy() = 0;

private:
    
};


uintptr_t CreateCdmIpcInstance( const std::string& key_system, const std::string& host_name, uintptr_t host_this);


//item(CreateCdmIpc, &CreateCdmIpcInstance)
#define CDM_TABLE(item) \
    item(CreateCdmIpc, &CreateCdmIpcInstance)\
    item(Initialize, &CdmIpcAbstract::Initialize)\
    item(GetStatusForPolicy, &CdmIpcAbstract::GetStatusForPolicy)\
    item(SetServerCertificate, &CdmIpcAbstract::SetServerCertificate)\
    item(CreateSessionAndGenerateRequest, &CdmIpcAbstract::CreateSessionAndGenerateRequest)\
    item(LoadSession, &CdmIpcAbstract::LoadSession)\
    item(UpdateSession, &CdmIpcAbstract::UpdateSession)\
    item(OnStorageId, &CdmIpcAbstract::OnStorageId)\
    item(Destroy, &CdmIpcAbstract::Destroy)
    //item(CloseSession, &CdmIpcAbstract::CloseSession)\
    //item(RemoveSession, &CdmIpcAbstract::RemoveSession)\
    


DECLARE_INVOKE_TABLE(CdmTable, CDM_TABLE);



#define HOST_TABLE(item) \
    item(OnInitialized, &HostIpcAbstract::OnInitialized)\
    item(OnResolveKeyStatusPromise, &HostIpcAbstract::OnResolveKeyStatusPromise)\
    item(OnResolveNewSessionPromise, &HostIpcAbstract::OnResolveNewSessionPromise)\
    item(OnResolvePromise, &HostIpcAbstract::OnResolvePromise)\
    item(OnRejectPromise, &HostIpcAbstract::OnRejectPromise)\
    item(OnSessionMessage, &HostIpcAbstract::OnSessionMessage)\
    item(OnSessionKeyChange, &HostIpcAbstract::OnSessionKeyChange)\
    item(OnExpirationChange, &HostIpcAbstract::OnExpirationChange)\
    item(OnSessionClosed, &HostIpcAbstract::OnSessionClosed)\
    item(SendPlatformChallenge, &HostIpcAbstract::SendPlatformChallenge)\
    item(EnableOutputProtection, &HostIpcAbstract::EnableOutputProtection)\
    item(RequestStorageId, &HostIpcAbstract::RequestStorageId)\
    item(Destroy, &HostIpcAbstract::Destroy)


DECLARE_INVOKE_TABLE(HostTable, HOST_TABLE);
