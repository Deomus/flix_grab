#pragma once

#include <string>
#include <vector>


#include "abstract_service.h"
#include "interproc_client.h"


typedef std::vector<uint8_t>        key_id_t;

class DecryptService : public AbstractDecryptService
{
public:
    DecryptService(const std::string& name);

    virtual ~DecryptService();

public:
    virtual int32_t             InitializeVideoDecoder(const VideoConfig& config);

    virtual int32_t             DecodeVideo(const EncryptedData& encrypted, FrameSource_fn& frame_sink);
    virtual int32_t             Decrypt(const EncryptedData& encrypted, DecryptedData& decrypted);
    virtual void                GetKeyIds(Keys& keys);
     
    virtual void                Finish();

    bool                        Reset();

public:
    const std::string&      name() const { return service_name_; }

private:
    bool                        ReadFrame(const BufferReader& reader, InvokeStatus& status, FrameSource_fn& frame_sink);

private:
    std::string                         service_name_;
    InterprocClient                     client_;
};

