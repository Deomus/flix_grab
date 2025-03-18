#pragma once
#include "content_decryption_module.h"
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

namespace cdm
{
    inline std::string GetSessionTypeName(SessionType type)
    {
        switch (type)
        {
        case kTemporary:
            return "Temporary";
        case kPersistentLicense:
            return "PersistentLicense";

        case kPersistentKeyRelease:
            return "PersistentKeyRelease";
        default:
            break;
        }
        return "Unknown";
    }

    inline std::string GetInitDataTypeName(InitDataType type)
    {
        switch (type)
        {

        case kCenc:
            return "Cenc";
        case kKeyIds:
            return "KeyIds";

        case kWebM:
            return "WebM";
        default:
            break;
        }
        return "Unknown";
    }

    inline std::string GetStatusName(Status status)
    {
        switch (status)
        {

        case kSuccess:
            return "Success";
        case kNeedMoreData:
            return "NeedMoreData";

        case kNoKey:
            return "NoDecryptKey";

        case kSessionError:
            return "SessionError";
        case kDecryptError:
            return "DecryptError";
        case kDecodeError:
            return "DecodeError";
        case kDeferredInitialization:
            return "DeferredInitialization";
        default:
            break;
        }
        return "Unknown";

    }
    inline std::string GetAudioCodecName(AudioDecoderConfig::AudioCodec codec)
    {
        switch (codec)
        {

        case AudioDecoderConfig::kUnknownAudioCodec:
            return "UnknownCodec";
        case AudioDecoderConfig::kCodecVorbis:
            return "Vorbis";

        case AudioDecoderConfig::kCodecAac:
            return "Aac";

        
        default:
            break;
        }
        return "Unknown";
        
    }

    inline std::string GetVideoCodecName(VideoDecoderConfig::VideoCodec codec)
    {
        switch (codec)
        {           
        case VideoDecoderConfig::kUnknownVideoCodec:
            return "UnknownCodec";
        case VideoDecoderConfig::kCodecVp8:
            return "Vp8";

        case VideoDecoderConfig::kCodecH264:
            return "H264";

        case VideoDecoderConfig::kCodecVp9:
            return "Vp9";


        default:
            break;
        }
        return "Unknown";

    }


    inline  std::string GetVideoFormatName(VideoFormat format)
    {
        switch (format)
        {
        case kUnknownVideoFormat:
            return "UnknownFormat";
        case kYv12:
            return "Yv12";

        case kI420:
            return "I420";

        default:
            break;
        }
        return "Unknown";

    }

    inline std::string GetStreamTypeName(StreamType type)
    {
        switch (type)
        {
        case kStreamTypeAudio:
            return "Audio";
        case kStreamTypeVideo:
            return "Video";
                   
        default:
            break;
        }
        return "Unknown";

    }

  
    inline  std::string GetAudioConfigName(AudioDecoderConfig config)
    {
        std::stringstream str;
        str << "codec: " << GetAudioCodecName(config.codec) << " channels: " << config.channel_count << " bits_per_channel: " << config.bits_per_channel << " samples_per_second: " << config.samples_per_second;

        return str.str();
    }

    inline std::string GetVideoConfigName(VideoDecoderConfig config)
    {
        std::stringstream str;
        str << "codec: " << GetVideoCodecName(config.codec) << " format: " << GetVideoFormatName(config.format) << " size: " << config.coded_size.width << "x" << config.coded_size.height;

        return str.str();
    }

    inline std::string GetMessageTypeName(MessageType type)
    {
      
        switch (type)
        {
        case kLicenseRequest:
            return "LicenseRequest";
        case kLicenseRenewal:
            return "LicenseRenewal";

        case kLicenseRelease:
            return "LicenseRelease";

        default:
            break;
        }
        return "Unknown";
    }

    inline std::string GetQueryResultName(QueryResult result)
    {

        switch (result)
        {
        case kQuerySucceeded:
            return "QuerySucceeded";
        case kQueryFailed:
            return "QueryFailed";
                    
        default:
            break;
        }
        return "Unknown";
    }

    inline std::string GetLinkMaskNames(uint32_t mask)
    {
        /*enum OutputLinkTypes {
            kLinkTypeNone = 0,
            kLinkTypeUnknown = 1 << 0,
            kLinkTypeInternal = 1 << 1,
            kLinkTypeVGA = 1 << 2,
            kLinkTypeHDMI = 1 << 3,
            kLinkTypeDVI = 1 << 4,
            kLinkTypeDisplayPort = 1 << 5,
            kLinkTypeNetwork = 1 << 6
        };*/

        std::string types;
        if (mask != kLinkTypeNone)
        {
            if (mask &&  kLinkTypeUnknown)
                types += "+kLinkTypeUnknown";
            if (mask &&  kLinkTypeInternal)
                types += "+kLinkTypeInternal";
            if (mask &&  kLinkTypeVGA)
                types += "+kLinkTypeVGA";
            if (mask &&  kLinkTypeHDMI)
                types += "+kLinkTypeHDMI";
            if (mask &&  kLinkTypeDVI)
                types += "+kLinkTypeDVI";
            if (mask &&  kLinkTypeDisplayPort)
                types += "+kLinkTypeDisplayPort";
            if (mask &&  kLinkTypeNetwork)
                types += "+kLinkTypeNetwork";
        }
        else
            types = "kLinkTypeNone";

       
        return types;
    }

    const char hexmap[] = { '0', '1', '2', '3', '4', '5', '6', '7',
        '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

    inline std::string hexStr(const unsigned char *data, int len)
    {
        std::string s(len * 2, ' ');
        for (int i = 0; i < len; ++i) {
            s[2 * i] = hexmap[(data[i] & 0xF0) >> 4];
            s[2 * i + 1] = hexmap[data[i] & 0x0F];
        }
        return s;
    }

    inline std::string hexStrDelim(const unsigned char *data, int len)
    {
        std::string s(len * 2 + len - 1, ' ');
        for (int i = 0; i < len; ++i) {
            s[3 * i] = hexmap[(data[i] & 0xF0) >> 4];
            s[3 * i + 1] = hexmap[data[i] & 0x0F];
        }
        return s;
    }

    inline std::string hexStrBlocks(const unsigned char *data, int len)
    {
        std::string s;
        for (int i = 0; i < len; i += 16) {
            int j = i;
            for (; j < i + 8 && j < len; j++) {
                s.push_back(hexmap[(data[j] & 0xF0) >> 4]);
                s.push_back(hexmap[data[j] & 0x0F]);
                s.push_back(' ');
            }
            s.pop_back();
            if ( j < len)
                s.push_back('|');

            for (; j < (i + 16) && j < len; j++) {
                s.push_back(hexmap[(data[j] & 0xF0) >> 4]);
                s.push_back(hexmap[data[j] & 0x0F]);
                s.push_back(' ');
            }
            s.push_back('\n');
        }
        s.pop_back();
        return s;
    }

    inline std::string GetKeyStatusName(KeyStatus status)
    {
        /*kUsable = 0,
            kInternalError = 1,
            kExpired = 2,
            kOutputRestricted = 3,
            kOutputDownscaled = 4,
            kStatusPending = 5,
            kReleased = 6*/

        switch (status)
        {
        case kUsable:
            return "Usable";
        case kInternalError:
            return "InternalError";

        case kExpired:
            return "Expired";
        case kOutputRestricted:
            return "OutputRestricted";
        case kOutputDownscaled:
            return "OutputDownscaled";
        case kStatusPending:
            return "StatusPending";
        case kReleased:
            return "Released";
        
        default:
            break;
        }
        return "Unknown";
    }
//
//# pragma pack (1)
//    struct  cenc_init
//    {
//        char            header[8];
//        unsigned int    version;
//        unsigned char   system_id[16];
//        unsigned int    kid_count;
//
//    };
//
//# pragma pack ()
//      
//
//
//    inline void  WriteBuffer(std::ostream& to, const InputBuffer& buffer)
//    {
//        to.write((const char*)&buffer.data_size, sizeof(buffer.data_size));
//        to.write((const char*)buffer.data, buffer.data_size);
//
//        to.write((const char*)&buffer.key_id_size, sizeof(buffer.key_id_size));
//        to.write((const char*)buffer.key_id, buffer.key_id_size);
//
//        to.write((const char*)&buffer.iv_size, sizeof(buffer.iv_size));
//        to.write((const char*)buffer.iv, buffer.iv_size);
//
//        to.write((const char*)&buffer.num_subsamples, sizeof(buffer.num_subsamples));
//
//        for (int i = 0; i < buffer.num_subsamples; ++i)
//        {
//            to.write((const char*)&buffer.subsamples[i].clear_bytes, sizeof(buffer.subsamples[i].clear_bytes));
//            to.write((const char*)&buffer.subsamples[i].cipher_bytes, sizeof(buffer.subsamples[i].cipher_bytes));
//        }
//
//        to.write((const char*)&buffer.timestamp, sizeof(buffer.timestamp));
//    }
//
//
//    inline void  ReadBuffer(std::istream& from, InputBuffer& buffer)
//    {
//        from.read((char*)&buffer.data_size, sizeof(buffer.data_size));
//        char* pData = new char[buffer.data_size];
//        from.read(pData, buffer.data_size);
//        buffer.data = (uint8_t*)pData;
//
//        from.read((char*)&buffer.key_id_size, sizeof(buffer.key_id_size));
//        char* pKey = new char[buffer.key_id_size];
//        from.read(pKey, buffer.key_id_size);
//        buffer.key_id = (uint8_t*)pKey;
//
//        from.read((char*)&buffer.iv_size, sizeof(buffer.iv_size));
//        char* pIv = new char[buffer.iv_size];
//        from.read(pIv, buffer.iv_size);
//        buffer.iv = (uint8_t*)pIv;
//       
//        from.read((char*)&buffer.num_subsamples, sizeof(buffer.num_subsamples));
//        
//
//        SubsampleEntry* pEntries = (SubsampleEntry*)(new char[buffer.num_subsamples * sizeof(SubsampleEntry)]);
//
//        for (int i = 0; i < buffer.num_subsamples; ++i)
//        {
//            from.read((char*)&(pEntries[i].clear_bytes), sizeof(pEntries[i].clear_bytes));
//            from.read((char*)&(pEntries[i].cipher_bytes), sizeof(pEntries[i].cipher_bytes));
//        }
//
//        buffer.subsamples = pEntries;
//
//        from.read((char*)&buffer.timestamp, sizeof(buffer.timestamp));
//    }
//
//    inline void    FreeBuffer(InputBuffer& buffer)
//    {
//        delete[] buffer.data;
//        delete[] buffer.key_id;
//        delete[] buffer.iv;
//
//        delete[] (char*)buffer.subsamples;
//
//    }
//
//    /*
//
//    VideoCodec codec;
//    VideoCodecProfile profile;
//    VideoFormat format;
//
//    // Width and height of video frame immediately post-decode. Not all pixels
//    // in this region are valid.
//    Size coded_size;
//
//    // Optional byte data required to initialize video decoders, such as H.264
//    // AAVC data.
//    uint8_t* extra_data;
//    uint32_t extra_data_size;
//    
//    */
//    inline void  WriteConfig(std::ostream& to, const VideoDecoderConfig& config)
//    {
//        to.write((const char*)&config.codec, sizeof(config.codec));
//        to.write((const char*)&config.profile, sizeof(config.profile));
//        to.write((const char*)&config.format, sizeof(config.format));
//
//        to.write((const char*)&config.coded_size.width, sizeof(config.coded_size.width));
//        to.write((const char*)&config.coded_size.height, sizeof(config.coded_size.height));
//
//        to.write((const char*)&config.extra_data_size, sizeof(config.extra_data_size));
//        to.write((const char*)config.extra_data, config.extra_data_size);
//
//    }
//
//
//    inline void  ReadConfig(std::istream& from, VideoDecoderConfig& config)
//    {
//        from.read((char*)&config.codec, sizeof(config.codec));
//        from.read((char*)&config.profile, sizeof(config.profile));
//        from.read((char*)&config.format, sizeof(config.format));
//
//        from.read((char*)&config.coded_size.width, sizeof(config.coded_size.width));
//        from.read((char*)&config.coded_size.height, sizeof(config.coded_size.height));
//
//
//        from.read((char*)&config.extra_data_size, sizeof(config.extra_data_size));
//        char* pExtra = new char[config.extra_data_size];
//        from.read(pExtra, config.extra_data_size);
//        config.extra_data = (uint8_t*)pExtra;
//        
//
//    }
//
//    inline void    FreeConfig(VideoDecoderConfig& config)
//    {
//        delete[] config.extra_data;
//       
//    }

}
