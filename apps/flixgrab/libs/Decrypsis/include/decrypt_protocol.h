#pragma once

#include <cstdint>
#include <vector>
#include <assert.h>
#include <iostream>

#include "content_decryption_module.h"


#define DEFAULT_FRAME_SERVER_NAME               "rpc_decryptor"

enum InvokeType
{
    kInvoke_StartStream = 0,
    kInvoke_InitVideoDecoder,
    kInvoke_InitAudioDecoder,

    kInvoke_DecryptVideo,
    kInvoke_DecryptAudio,
    kInvoke_Decrypt,
    kInvoke_GetKeyIds,
    kInvoke_Finish,


};

struct KeyInfo {

    std::vector<uint8_t>                    key_id;
    cdm::KeyStatus                          status;
    uint32_t                                system_code;

    KeyInfo(const cdm::KeyInformation& info) {
        key_id.assign(info.key_id, info.key_id + info.key_id_size);
        status = info.status;
        system_code = info.system_code;
    }

    KeyInfo() : status(cdm::kInternalError), system_code(0) {}

    template<typename B>
    bool        Serializer(B& buffer)
    {
        bool result = true;

        result &= buffer.Pod(status);
        result &= buffer.Pod(system_code);
        result &= buffer.Vector(key_id);
        
        return result;
    }
};

struct Keys
{
    std::vector<KeyInfo>                    keys;

    template<typename B>
    bool        Serializer(B& buffer)
    {
        bool result = true;

        size_t size = keys.size();
        result &= buffer.Pod(size);

        keys.resize(size);

        for (uint32_t i = 0; i < size; ++i) {
            result &= keys[i].Serializer(buffer);
        }

        return result;
    }

};


//struct KeyIds
//{
//    std::vector<std::vector<uint8_t>>       key_ids;
//
//    template<typename B>
//    bool        Serializer(B& buffer)
//    {
//        bool result = true;
//
//        size_t size = key_ids.size();
//        result &= buffer.Pod(size);
//        
//        key_ids.resize(size);
//
//        for (uint32_t i = 0; i < size; ++i) {
//            result &= buffer.Vector(key_ids[i]);
//        }
//        
//        return result;
//    }
//
//};

struct VideoConfig
{
    cdm::VideoCodec             codec;
    cdm::VideoCodecProfile      profile;
    cdm::VideoFormat                                format;

    // Width and height of video frame immediately post-decode. Not all pixels
    // in this region are valid.
    uint32_t                                        width;
    uint32_t                                        height;

    float                                           frame_rate;


    template<typename B>
    bool        Serializer(B& buffer)
    {
        bool result = true;
        result &= buffer.Pod(codec);
        result &= buffer.Pod(profile);
        result &= buffer.Pod(format);
        result &= buffer.Pod(width);
        result &= buffer.Pod(height);
        result &= buffer.Pod(frame_rate);

        return result;
    }
        
};


struct EncryptedData
{
    // Presentation timestamp in microseconds.
    int64_t                                         timestamp;  

    std::vector<uint8_t>                            buffer;

    // Key ID to identify the decryption key.
    std::vector<uint8_t>                            key_id;

    // Initialization vector.
    std::vector<uint8_t>                            iv;

    std::vector<cdm::SubsampleEntry>                subsamples;

    template<typename B>
    bool        Serializer(B& b)
    {
        bool result = true;
        result &= b.Pod(timestamp);
        result &= b.Vector(buffer);
        result &= b.Vector(key_id);
        result &= b.Vector(iv);
               
        result &= b.Vector(subsamples);

        if (!result)
        {
            std::cout << "Error Writing Encrypted buffer of size: " << buffer.size() << " subsamples_count: " << subsamples.size() << std::endl;
        }

        return result;
    }
            
};

struct DecryptedData {

    cdm::Status                                     status;
    int64_t                                         timestamp;
    std::vector<uint8_t>                            buffer;

    template<typename B>
    bool        Serializer(B& b)
    {
        bool result = true;
        result &= b.Pod(status);
        result &= b.Pod(timestamp);
        result &= b.Vector(buffer);
       
        if (!result)
        {
            std::cout << "Error Writing Decrypted buffer of size: " << buffer.size() << std::endl;
        }

        return result;
    }
};

struct InvokeStatus 
{
    //cdm::Status         value;
    uint32_t         value;

    InvokeStatus() : value(cdm::kSuccess) {}

    template<typename B>
    bool        Serializer(B& buffer)
    {
        bool result = true;
        result &= buffer.Pod(value);
       
        return result;
    }
};

struct VideoFrameData
{
    InvokeStatus                                    status;
    // Presentation timestamp in microseconds.
    int64_t                                         timestamp;

    cdm::VideoFormat                                format;

    // Width and height of video frame immediately post-decode. Not all pixels
    // in this region are valid.
    decltype(cdm::Size::width)                     width;
    decltype(cdm::Size::height)                    height;

    uint32_t                                        plane_offset[cdm::VideoPlane::kMaxPlanes];
    uint32_t                                        plane_stride[cdm::VideoPlane::kMaxPlanes];

    uint8_t*                                        data;
    size_t                                          data_size;
    
  
    //template<typename B, typename F>
    //bool        Serializer(B& buffer, F frame_serializer )
    //{
    //    bool result = true;
    //    result &= status.Serializer(buffer);

    //    if (status.value == cdm::kSuccess) {
    //        result &= buffer.Pod(timestamp);

    //        result &= buffer.Pod(format);
    //        result &= buffer.Pod(width);
    //        result &= buffer.Pod(height);
    //        //Read/Write Frame Block
    //        result &= buffer.Block(frame_serializer);
    //                   
    //    }
    //    return result;
    //    
    //}

    //template<typename B>
    //bool        Serializer(B& buffer)
    //{
    //    bool result = true;
    //  
    //    result &= buffer.Pod(timestamp);

    //    result &= buffer.Pod(format);
    //    result &= buffer.Pod(width);
    //    result &= buffer.Pod(height);

    //    return result;

    //}

};
