#pragma once

#include <functional>

#include <cstdint>
#include "decrypt_protocol.h"


typedef std::function<bool(VideoFrameData* frame)>     FrameSource_fn;

enum DecryptStatus
{
    kStatus_CommunicationError = 1000,
};

class AbstractDecryptService {

public:
    virtual int32_t             InitializeVideoDecoder(const VideoConfig& config) = 0;

    virtual int32_t             InitializeAudioDecoder(int codec, int channel_count, int bits_per_channel, int samples_per_second, std::vector<uint8_t> extra_data) = 0;

    virtual int32_t             DecodeVideo(const EncryptedData& encrypted, FrameSource_fn& frame_sink) = 0;

    virtual int32_t             Decrypt(const EncryptedData& encrypted, DecryptedData& decrypted) = 0;

    virtual void                GetKeyIds(Keys& keys) = 0;

    virtual void                Finish() = 0;


protected:
    virtual                     ~AbstractDecryptService() {}
};

class CdmHostForPrivateKeys {
public:
    virtual void OnSessionMessageFromPK(const char* session_id, uint32_t session_id_size, const char* message, uint32_t message_size) = 0;
};

