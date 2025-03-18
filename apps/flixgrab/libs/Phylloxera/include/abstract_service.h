#pragma once

#include <functional>

#include <cstdint>
#include "decrypt_protocol.h"


typedef std::function<bool(VideoFrameData* frame_header, const uint8_t* frame_data, size_t frame_size)>     FrameSource_fn;

enum DecryptStatus
{
    kStatus_CommunicationError = 1000,
};

class AbstractDecryptService
{
public:
    virtual int32_t             InitializeVideoDecoder(const VideoConfig& config) = 0;

    virtual int32_t             DecodeVideo(const EncryptedData& encrypted, FrameSource_fn& frame_sink) = 0;

    virtual int32_t             Decrypt(const EncryptedData& encrypted, DecryptedData& decrypted) = 0;

    virtual void                GetKeyIds(Keys& keys) = 0;

    virtual void                Finish() = 0;


protected:

    virtual ~AbstractDecryptService() {}


};
