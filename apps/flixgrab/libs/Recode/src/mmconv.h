#pragma once

#include <stdint.h>
#include <string>
#include <vector>

#include "mmconvdefs.h"

namespace MMConv
{

enum EH264Profile
{
    kH264Profile_Baseline = 0,
    kH264Profile_Main,
    kH264Profile_High,
};

static const char* MmcParam_UseGpuAccelerator = "UseGpuAccelerator";        //bool

static const char* MmcParam_Width = "Width";                                //std::vector<int>
static const char* MmcParam_Height = "Height";                              //std::vector<int>
static const char* MmcParam_Bitrate = "Bitrate";                            //int
static const char* MmcParam_FrameFormat = "FrameFmt";                       //std::vector<EFrameFormat>
static const char* MmcParam_FrameRate = "FrameRate";                        //TRational
static const char* MmcParam_SampleAspectRatio = "SAR";                      //TRational
static const char* MmcParam_DisplayAspectRatio = "DAR";                     //TRational
static const char* MmcParam_QScale             = "QScale";                  //int


static const char* MmcParam_SampleRate = "SampleRate";                      //std::vector<int>
static const char* MmcParam_ChannelLayout = "ChannelLayout";                //std::vector<EChannelLayout>

static const char* MmcParam_Codec = "Codec";                                //ECodec
static const char* MmcParam_CodecName = "CodecName";                        //const char*
static const char* MmcParam_AdditionalCodecParam = "AdditionalCodecParam";                        //const char*

//Output param for container and so on;
static const char* MmcParam_AdditionalOutputParam = "AdditionalOutputParam";                        //const char*

//NOTE: FFMPEG Specific Filter (Usually Uses With Codec Copying);
//TODO: May be Enum Needed;
static const char* MmcParam_BitstreamFilter = "BitstreamFilter";            //const char*


//Encoder Params
static const char* MmcParam_H264_Profile = "Profile";                        //EH264Profile


//////////////////////////////////////////////////////////
//for In/OutputFile
static const char* MmcParam_MediaFormat  = "MediaFormat";                        //MMCore::EMediaFormat eFmt
static const char* MmcParam_Duration     = "Duration";                           //Int64
static const char* MmcParam_StartTime    = "StartTime";                          //Int64
static const char* MmcParam_SeekTime     = "SeekTime";                           //Int64

static const char* MmcParam_NumberFrames = "NumberFrames";                       //Int64 number video frames
static const char* MmcParam_Loop = "Loop";                                       //Int32 loop param for GIF format


//Tags:
static const char* MmcTag_Title         = "TagTitle";                                  //const char*
static const char* MmcTag_ArtworkUri    = "TagArtwork";                                //const char* URI to file jpg, png
static const char* MmcTag_Comments      = "TagComments";                                //const char*


////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef float TVolumeAdjust;
enum ERotate
{
    kRotate_None = 0,
    kRotate_Clockwise90,
    kRotate_CounterClockwise90,
    kRotate_Clockwise180,
    kRotate_Clockwise270,
    kRotate_VerticalFlip,
    kRotate_HorizontalFlip,
    kRotate_CounterClockwise90Flip,
    kRotate_Clockwise90Flip,
    kRotate_End,
};

enum EWatermarkType {
    kWatermark_None = 0,
    //Adds Video Intro And Outro If video reference is exists or audio if exists;
    kWatermark_IntroAndOutro,
    kWatermark_VideoOverlay1min,
    kWatermark_AudioOverlay1min,
};

typedef int32_t             TResult;

enum EResult
{
    kResult_Success = 0,
    kResult_Fail,               //Procedure failed for unknown reason;
    kResult_WrongParam,         //Wrong input param detected;
    kResult_OutOfRange,         //Using for enumerate;
    kResult_NoMemory,
    kResult_IoError,
    kResult_Unsupported,
    kResult_Timeout,
    kResult_NotImplemented,
    kResult_Skipped,
    kResult_Unconfigured,
    kResult_WrongAddress,
    kResult_WrongAccess,
    kResult_WrongState,
    kResult_BufferOverflow,
    kResult_Interrupted,

    kResult_ForceDWORD = 0x7FFFFFFF,

};

#if defined(WIN32) || defined(WIN64)
#ifdef MMCONV_EXPORTS
#define MMCONV_API __declspec(dllexport)
#else
#define MMCONV_API __declspec(dllimport)
#endif
#else
#define MMCONV_API
#endif

//#if defined(WIN32) || defined(WIN64)

class IMediaTranscoder;
class ITransDelegate
{
public:
    virtual void                    onStart(IMediaTranscoder*) = 0;
    virtual void                    onStop(IMediaTranscoder*) = 0;
    virtual void                    onProgress(IMediaTranscoder*) = 0;
    virtual void                    onError(IMediaTranscoder*) = 0;
};

class IMediaTranscoder
{
public:
    virtual TResult                 start(const char* sInFileName, const std::wstring& sOutFileName, int audioOutputSampleRate = 0, int audioOutputBitrate = 0, int audioOutputChannels = 0) = 0;
    virtual TResult                 stop(void) = 0;
    virtual float                   getProgress() = 0;
    virtual EConvErrorCode          getErrorCode() = 0;

    virtual void                    set_metadata_title(const std::wstring& metadata_title) = 0;
    virtual void                    set_metadata_artist(const std::wstring& metadata_artist) = 0;
    virtual void                    set_metadata_album(const std::wstring& metadata_album) = 0;
    virtual void                    set_metadata_album_artist(const std::wstring& metadata_album_artist) = 0;
    virtual void                    set_metadata_year(const std::wstring& metadata_year) = 0;
    virtual void                    set_metadata_comment(const std::wstring& metadata_comment) = 0;
    virtual void                    set_metadata_genre(const std::wstring& metadata_genre) = 0;
    virtual void                    set_metadata_track(const std::wstring& metadata_track) = 0;
    virtual void                    set_metadata_image_path(const std::string& metadata_image_path) = 0;

    virtual void                    shutdown(void) = 0;
};

//extern "C"
//{
//#endif
//    MMCONV_API  TResult     createFFTranscoder(ITransDelegate* pDelegate, IMediaTranscoder** ppTranscoder);
     //TResult     createLameTranscoder(ITransDelegate* pDelegate, IMediaTranscoder** ppTranscoder);
    //MMCONV_API  TResult     createLameFilterTranscoder(ITransDelegate* pDelegate, IMediaTranscoder** ppTranscoder);
    //MMCONV_API  TResult     createGPUFFTranscoder(ITransDelegate* pDelegate, IMediaTranscoder** ppTranscoder);

    //MMCONV_API  TResult     createSimpleTranscoder(const char *szInPath, const char *szOutPath, const TFilter *pFilter, MMPreset::IPreset* pPreset, ISimpleTransDelegate* pDelegate, ISimpleTranscoder** ppTranscoder, bool bOnlyFFtrans = false);
    //MMCONV_API  TResult     createExtractFrameTranscoder(const char *szInPath, const char *szOutPath, MMCore::TTimestamp nTime, const TFilter *pFilter, ISimpleTransDelegate* pDelegate, ISimpleTranscoder** ppTranscoder);
    //MMCONV_API  TResult     createAVJoinTranscoder(const char *szAudioInPath, const char *szVideoInPath, const char *szOutPath, MMCore::EMediaFormat eOutFmt, ISimpleTransDelegate* pDelegate, ISimpleTranscoder** ppTranscoder);
    //MMCONV_API  TResult     createHlsDownloader(const char *szIn, const char *szOut, ISimpleTransDelegate* pDelegate, ISimpleTranscoder** ppTranscoder, int urlType);
    //MMCONV_API  TResult     createMergeTranscoder(const char* szIn[], int nInCount, const char *szOut, MMPreset::IPreset* pPreset, ISimpleTransDelegate* pDelegate, ISimpleTranscoder** ppTranscoder);
    //MMCONV_API  TResult     getOriginalPreset(const char *szInPath, const char *szPreset, MMPreset::IPreset** ppPreset);
    ////
    ////FFMpeg based Tagger 
    //MMCONV_API  TResult     createTagger(const char *szInPath, const char *szOutPath, Tier0::IDictionary* tags, ISimpleTransDelegate* pDelegate, ISimpleTranscoder** ppTranscoder);
    ////Precode Files to use concat demuxer;
    //MMCONV_API  TResult     createConcatPrecoder(const char* in, const char *out, MMPreset::IPreset* preset, ISimpleTransDelegate* delegate, ISimpleTranscoder** transcoder);
    ////Concat files from collection;
    //MMCONV_API  TResult     createConcat(const char *out, ISimpleTransDelegate* delegate, ISimpleTranscoder** transcoder, IMediaCollection** collection);

//#if defined(WIN32) || defined(WIN64)
//    }
//#endif

} // namespace MMConv
