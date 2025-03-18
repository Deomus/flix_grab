#pragma once 

//#include <windows.h>

#include <string>
#include <vector>
#include <cstdint>


#include "content_decryption_module.h"

//#include <limits.h>
//#include <iostream>
#include "struct_strings.h"

#include "buffer_pooling.h"




class CdmGrab;

class HostProxy : public cdm::ContentDecryptionModule::Host
{
public:
    virtual cdm::Buffer* Allocate(uint32_t capacity);

    // Requests the host to call ContentDecryptionModule::TimerFired() |delay_ms|
    // from now with |context|.
    virtual void SetTimer(int64_t delay_ms, void* context);

    // Returns the current wall time in seconds.
    virtual cdm::Time GetCurrentWallTime();

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
        cdm::Error error,
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
        uint32_t message_size,
        const char* legacy_destination_url,
        uint32_t legacy_destination_url_length);

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
    virtual void OnLegacySessionError(
        const char* session_id, uint32_t session_id_length,
        cdm::Error error,
        uint32_t system_code,
        const char* error_message, uint32_t error_message_length);

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

public:
    HostProxy();
    virtual ~HostProxy();

    cdm::Time   GetNowTime() const;

    std::string GetSession() const { return session_; }
private:
    std::shared_ptr<BufferPooling>                  buffer_pooling_;
    std::string                             session_;
};