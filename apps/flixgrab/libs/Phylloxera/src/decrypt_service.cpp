#include "decrypt_service.h"
#include <functional>
#include <assert.h>
#include <iostream>

DecryptService::DecryptService(const std::string& name) : service_name_(name)
{
    std::cout << "DecryptService::Constructor " << service_name_ << std::endl;
}

DecryptService::~DecryptService()
{
    std::cout << "DecryptService::Destructor " << service_name_ << std::endl;
}

int32_t DecryptService::InitializeVideoDecoder(const VideoConfig& config)
{
    InvokeStatus    status;

    bool result = client_.Invoke(kInvoke_InitVideoDecoder,
        std::bind(&VideoConfig::Serializer<BufferWriter>, const_cast<VideoConfig*>(&config), std::placeholders::_1),
        std::bind(&InvokeStatus::Serializer<BufferReader>, &status, std::placeholders::_1));

    if (result)
        return status.value;

    return kStatus_CommunicationError;
}

int32_t DecryptService::DecodeVideo(const EncryptedData& encrypted, FrameSource_fn& frame_sink)
{
    InvokeStatus    status;

    //Invoke Decrypt;
    bool result = client_.Invoke(kInvoke_DecryptVideo,
        std::bind(&EncryptedData::Serializer<BufferWriter>, const_cast<EncryptedData*>(&encrypted), std::placeholders::_1),
        std::bind(&DecryptService::ReadFrame, this, std::placeholders::_1, std::ref(status), std::ref(frame_sink))
        /*std::bind(&VideoFrameData::Serializer<interproc::BufferReader, interproc::block_reader_fn>, &frame, std::placeholders::_1,
        frame_reader)*/);

    if (result)
        return status.value;

    return kStatus_CommunicationError;
}

int32_t DecryptService::Decrypt(const EncryptedData& encrypted, DecryptedData& decrypted) {
   
    //Invoke Decrypt;
    bool result = client_.Invoke(kInvoke_Decrypt,
        std::bind(&EncryptedData::Serializer<BufferWriter>, const_cast<EncryptedData*>(&encrypted), std::placeholders::_1),
        std::bind(&DecryptedData::Serializer<BufferReader>, &decrypted, std::placeholders::_1));

    if (result)
        return decrypted.status;

    return kStatus_CommunicationError;
}

void DecryptService::GetKeyIds(Keys& keys)
{
    //Invoke Decrypt;
    bool result = client_.Invoke(kInvoke_GetKeyIds,
        [](BufferWriter&)->bool {return true; }, //Stub Function
        std::bind(&Keys::Serializer<BufferReader>, &keys, std::placeholders::_1)
        );

    if (!result) {
        keys.keys.clear();
        std::cout << "DecryptService::GetKeyIds Error Invoke " << service_name_ << std::endl;
    }
}


void DecryptService::Finish()
{
    //Invoke Finish;
    bool result = client_.Invoke(kInvoke_Finish,
        [](BufferWriter&)->bool {return true; }, //Stub Function
        [](BufferReader&)->bool {return true; }
    );

    if (!result) {
        std::cout << "Can't Invoke Finish" << std::endl;
    }
}

bool DecryptService::Reset()
{
    client_.Stop();
    return client_.Start(service_name_);
}

bool DecryptService::ReadFrame(const BufferReader& reader, InvokeStatus& status, FrameSource_fn& frame_sink)
{
    bool result = true;

    VideoFrameData  frame_header;
    block_reader_fn frame_reader = std::bind(frame_sink, &frame_header, std::placeholders::_1, std::placeholders::_2);

    result &= status.Serializer(reader);

    if (result && (status.value == cdm::kSuccess)) {
        result &= frame_header.Serializer(reader);
        result &= reader.Block(frame_reader);
    }
    return result;
}
