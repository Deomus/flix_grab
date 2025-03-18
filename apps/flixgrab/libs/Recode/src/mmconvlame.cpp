#include "mmconvlame.h"

#include <algorithm>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <assert.h>

#include <mmconv.h>

#include <fstream>
#include <lame/lame.h>
#include "lameaudiodecoder.h"

//#include "logmacros_def.h"

#pragma comment(lib, "mp3lame.lib")


#define LOG_DBG(m) std::cout << m
#define LOG_INF(m) std::cout << m
#define LOG_ERR(m)  std::cerr << m
#define TIER0_DBG_A(p, m) std::cout << p << ";" << m
#define TIER0_ERR_A(p, m) std::cerr << p << ";" << m

#define COUNT_OF_MILLISECONDS_TO_SEND_PROGRESS 300

namespace MMConv
{
    CLameTranscoder::CLameTranscoder()
    {}

    CLameTranscoder::~CLameTranscoder()
    {
        LOG_DBG("Destroying lame transcoder" );

        //for (auto in : m_vInFile) {
        //    LOG_DBG("Input File=" << in->getFilePath());
        //}
        //stop();
        if (m_worker.joinable()) {
            if (std::this_thread::get_id() != m_worker.get_id())
                m_worker.join();
            else
                m_worker.detach();
        }
    }

    TResult CLameTranscoder::init(ITransDelegate* pDelegate)
    {
        TResult nRet = kResult_Success;
        //do
        //{
            //nRet = createDictionary(&m_spDict);
            //if (nRet != kResult_Success)
            //    break;

            m_spDelegate = pDelegate;
        //} 
        //while (false);
        return nRet;
    }

    //TResult CLameTranscoder::getInput(const char* pszFilePath, IInputFile** ppFile)
    //{
    //    assert(ppFile);

    //    TResult nRet = kResult_Success;

    //    auto spInputFile = TSmartPtr<CInputFile>::create(new CInputFile());
    //    if ((nRet = spInputFile->init(m_vInFile.size(), pszFilePath)) == kResult_Success)
    //    {
    //        m_vInFile.push_back(spInputFile);
    //        *ppFile = spInputFile.share();
    //    }
    //    return nRet;
    //}

    //TResult CLameTranscoder::createOutput(const char* pszFilePath, IOutputFile** ppFile)
    //{
    //    assert(ppFile);

    //    TResult nRet = kResult_Success;

    //    auto spOutputFile = TSmartPtr<COutputFile>::create(new COutputFile());
    //    if ((nRet = spOutputFile->init(m_vOutFile.size(), pszFilePath)) == kResult_Success)
    //    {
    //        m_vOutFile.push_back(spOutputFile);
    //        *ppFile = spOutputFile.share();
    //    }
    //    return nRet;
    //}

    //TResult  CLameTranscoder::createGraph(const char* pszFilter, ITransGraph** ppGraph)
    //{
    //    assert(ppGraph);

    //    TResult nRet = kResult_Success;

    //    auto spGraph = TSmartPtr<CTransGraph>::create(new CTransGraph());
    //    if ((nRet = spGraph->init(pszFilter)) == kResult_Success)
    //    {
    //        m_spGraph = spGraph;
    //        *ppGraph = m_spGraph.share();
    //    }
    //    return nRet;
    //}

    //TResult  CLameTranscoder::getDict(IDictionary ** ppDict)
    //{
    //    assert(ppDict);
    //    if (m_spDict)
    //    {
    //        *ppDict = m_spDict.share();
    //        return kResult_Success;
    //    }
    //    return kResult_Fail;
    //}

    TResult CLameTranscoder::start(const char* sInFileName, const std::wstring& sOutFileName,
        int audioOutputSampleRate, int audioOutputBitrate, int audioOutputChannels)
    {
        TResult nRet = kResult_Success;

        this->sInFileName = sInFileName;
        this->sOutFileName = sOutFileName;
        m_audioOutputSampleRate = audioOutputSampleRate;
        m_audioOutputBitrate = audioOutputBitrate;
        m_audioOutputChannels = audioOutputChannels;

        m_worker = std::thread(&CLameTranscoder::workerThread, this);
        LOG_INF("Start lame transcoding ...");

        return nRet;
    }

    TResult CLameTranscoder::stop(void)
    {
        TResult nRet = kResult_Success;
        LOG_INF("Stop lame transcoding.");
        
        m_bStop = true;
       /* if (m_worker.joinable()) {
            if (std::this_thread::get_id() != m_worker.get_id())
                m_worker.join();
            else
                m_worker.detach();
        }*/
        return nRet;
    }

    float CLameTranscoder::getProgress()
    {
        return m_progress;
    }

    EConvErrorCode  CLameTranscoder::getErrorCode()
    {
        return m_nLastError;
    }

    void CLameTranscoder::shutdown(void)
    {
        if (!m_bStop) {
            stop();
        }
        if (m_worker.joinable()) {
            if (std::this_thread::get_id() != m_worker.get_id())
                m_worker.join();
            else
                m_worker.detach();
        }
        m_spDelegate = NULL;
    }

    void CLameTranscoder::onProgress()
    {
        if (m_spDelegate) m_spDelegate->onProgress(this);
    }

    void CLameTranscoder::onError()
    {
        if (m_spDelegate) m_spDelegate->onError(this);
    }

    void CLameTranscoder::onStart()
    {
        if (m_spDelegate) m_spDelegate->onStart(this);
    }

    void CLameTranscoder::onStop()
    {
        if (m_spDelegate) m_spDelegate->onStop(this);
    }

    void CLameTranscoder::setStreamOptions(const std::string& sStSpec, void* spStream, std::stringstream& ssCmd)
    {
    }

    void CLameTranscoder::setOutputOptions(void* spOutFile, std::stringstream& ssCmd)
    {
    }

    void CLameTranscoder::setInputOptions(void* spInFile, std::stringstream& ssCmd)
    {
    }

    std::vector<unsigned char> CLameTranscoder::getUtf16WithBom(const std::wstring& str) {
        if (str.empty())
            return std::vector<unsigned char>();

        const auto p = reinterpret_cast<unsigned char const*>(&str[0]);
        std::size_t size = str.size() * sizeof(str.front());
        std::vector<unsigned char> bytes(p, p + size);

        std::vector<unsigned char> vUtf8 = { 0xFF,0xFE }; // bom
        vUtf8.insert(vUtf8.end(), bytes.begin(), bytes.end());
        vUtf8.insert(vUtf8.end(), 0);
        vUtf8.insert(vUtf8.end(), 0);

        return vUtf8;
    }

    void CLameTranscoder::workerThread()
    {
        LOG_INF("--> workerThread");
        //This Can Be Deleted Before Lame Ends;
        //TODO: TSmartPtr<CLameTranscoder>         this_holder(this);
        
        this->onStart();
        CAudioDecoder audioDecoder;
        
        int audioTrackNumber = -1; 
        int audioOutputSampleRate = this->m_audioOutputSampleRate; //0;
        int audioOutputBitrate = this->m_audioOutputBitrate; // 0;
        int audioOutputChannels = this->m_audioOutputChannels; // 0

        try
        {
            unsigned long long dwSamples = 0;
            unsigned long long dwMP3Buffer = 0;
            unsigned long long dwWrite = 0;

            lame_global_flags* pLame = NULL;

            std::ofstream fileMp3(sOutFileName.c_str(), std::ios::out | std::ios::binary);
            m_bStop = false;
            bool bFinished = false;
            do
            {
                if (!audioDecoder.open(sInFileName.c_str()))
                {
                    LOG_ERR("workerThread | m_decoder open failed");
                    m_nLastError = CONV_ERROR_CODE_OPEN_FAILED;
                    this->onError();
                    break;
                }

                if (!fileMp3.is_open())
                {
                    LOG_ERR("workerThread | fileMp3 open failed");
                    this->m_nLastError = CONV_ERROR_CODE_OPEN_FAILED;
                    this->onError();
                    break;
                }

                TAudioParams    inParams;
                TAudioParams    outParams;
                if (!audioDecoder.getAudioParams(inParams, audioTrackNumber))
                {
                    LOG_ERR("workerThread | getAudioParams failed");
                    this->m_nLastError = CONV_ERROR_CODE_OPEN_FAILED;
                    this->onError();
                    break;
                }
                outParams = inParams;

                if (audioOutputSampleRate > 0)
                    outParams.sampleRate = audioOutputSampleRate;

                if (outParams.channels > 2)
                    outParams.channels = 2;

                pLame = lame_init();
                if (pLame == NULL)
                {
                    LOG_ERR("workerThread | pLame not init");
                    this->m_nLastError = CONV_ERROR_CODE_INVALID_INPUT_ARGUMENT;
                    this->onError();
                    return;
                }

                // --------------- Set id3 tags

                const char* ID_TITLE        = "TIT2";
                const char* ID_ARTIST       = "TPE1";
                const char* ID_ALBUM        = "TALB";
                const char* ID_ALBUM_ARTIST = "TPE2";
                const char* ID_YEAR         = "TYER";
                const char* ID_COMMENT      = "COMM";
                const char* ID_GENRE        = "TCON";
                const char* ID_TRACK        = "TRCK";
                const char* ID_DATE         = "TDRL"; //not used
                const char* ID_DURATION     = "TSSE"; //not used
                

                id3tag_init(pLame);
                id3tag_add_v2(pLame);
                id3tag_v2_only(pLame);
                lame_set_write_id3tag_automatic(pLame, 0);

                //id3tag_set_title(pLame, id3tag_title_.c_str());
                if (0 != id3tag_set_textinfo_utf16(pLame, ID_TITLE, reinterpret_cast<const unsigned short*>(getUtf16WithBom(id3tag_title_).data())))
                    LOG_ERR(" id3tag title is invalid");

                //id3tag_set_artist(pLame, id3tag_artist_.c_str());
                if (0 != id3tag_set_textinfo_utf16(pLame, ID_ARTIST, reinterpret_cast<const unsigned short*>(getUtf16WithBom(id3tag_artist_).data())))
                    LOG_ERR(" id3tag artist is invalid");

                //id3tag_set_album(pLame, id3tag_album_.c_str());
                if (0 != id3tag_set_textinfo_utf16(pLame, ID_ALBUM, reinterpret_cast<const unsigned short*>(getUtf16WithBom(id3tag_album_).data())))
                    LOG_ERR(" id3tag album is invalid");

                if (0 != id3tag_set_textinfo_utf16(pLame, ID_ALBUM_ARTIST, reinterpret_cast<const unsigned short*>(getUtf16WithBom(id3tag_album_artist_).data())))
                    LOG_ERR(" id3tag album_artist is invalid");

                //id3tag_set_year(pLame, id3tag_year_.c_str());
                if (0 != id3tag_set_textinfo_utf16(pLame, ID_YEAR, reinterpret_cast<const unsigned short*>(getUtf16WithBom(id3tag_year_).data())))
                    LOG_ERR(" id3tag year is invalid");

                //id3tag_set_comment(pLame, id3tag_comment_.c_str());
                if (0 != id3tag_set_comment_utf16(pLame, 0, 0, reinterpret_cast<const unsigned short*>(getUtf16WithBom(id3tag_comment_).data())))
                    LOG_ERR(" id3tag comment is invalid");

                //if (0 != id3tag_set_genre(pLame, id3tag_genre_.c_str()))  LOG_ERR(" id3tag genre is invalid");
                if (0 != id3tag_set_textinfo_utf16(pLame, ID_GENRE, reinterpret_cast<const unsigned short*>(getUtf16WithBom(id3tag_genre_).data())))
                    LOG_ERR(" id3tag genre is invalid");

                //if (0 != id3tag_set_track(pLame, id3tag_track_.c_str()))  LOG_ERR(" id3tag track number is invalid");
                if (0 != id3tag_set_textinfo_utf16(pLame, ID_TRACK, reinterpret_cast<const unsigned short*>(getUtf16WithBom(id3tag_track_).data())))
                    LOG_ERR(" id3tag track is invalid");

                //if (0 != id3tag_set_albumart(pLame, reinterpret_cast<char*>(id3tag_image_.data()), id3tag_image_.size())) // maximum size of albumart image (128KB)
                //    LOG_ERR(" id3tag albumart image is invalid");

                // maximum size of albumart image (128KB)
                std::ifstream f_img(id3tag_image_path_, std::fstream::binary);
                if (f_img.good()) {
                    //std::vector<uint8_t> v_buf = fitImgToSize(f_img, 128 * 1024);
                    std::vector<uint8_t> v_buf((std::istreambuf_iterator<char>(f_img)), (std::istreambuf_iterator<char>()));
                    if (v_buf.size() > 128 * 1024 || 0 != id3tag_set_albumart(pLame, reinterpret_cast<char*>(v_buf.data()), v_buf.size()))
                        LOG_ERR(" id3tag albumart image is invalid");
                }



                // --------------- Set arguments to LAME encoder -------------------------
                // Set input sample frequency
                lame_set_in_samplerate(pLame, outParams.sampleRate);
                lame_set_bWriteVbrTag(pLame, 1);
                //lame_set_VBR( pLame, vbr_default ); 

                // Set frequency resampling rate, if specified
                //if ( m_pBase->audioOutputSampleRate > 0 )
                // lame_set_out_samplerate( pLame, m_pBase->audioOutputSampleRate );
                // Set CBR encode;
                if (audioOutputBitrate > 0) {
                    lame_set_preset(pLame, audioOutputBitrate / 1000);
                    lame_set_VBR(pLame, vbr_off);
                }

                lame_set_num_channels(pLame, outParams.channels);

                switch (audioOutputChannels) {
                    case 1:
                        lame_set_mode(pLame, MONO);
                        break;
                    case 2:
                        lame_set_mode(pLame, JOINT_STEREO);
                        break;
                }

                /*
                R3MIX = 1000,
                STANDARD = 1001,
                EXTREME = 1002,
                INSANE = 1003,*/
                //UGLY HACK:
                switch (audioOutputBitrate) {
                    case 0:
                    case -11:   // kPreset_MP3FromVideoStream - use bitrate from input
                        if (outParams.bitrate > 0) {
                            lame_set_preset(pLame, outParams.bitrate / 1000);
                            lame_set_VBR(pLame, vbr_off);
                        }
                        break;
                    case -1:
                        lame_set_preset(pLame, STANDARD);
                        break;
                    case -2:
                        lame_set_preset(pLame, EXTREME);
                        break;
                    case -3:
                        lame_set_preset(pLame, INSANE);
                        break;
                    case -4:
                        lame_set_preset(pLame, R3MIX);
                        break;
                    case -10:
                        lame_set_preset(pLame, 32); //kPreset_MP3LowQuality
                        lame_set_VBR(pLame, vbr_off);
                        break;
                }

                if (0 != lame_init_params(pLame)) {
                    LOG_ERR("workerThread | lame_init_params failed");
                    this->m_nLastError = CONV_ERROR_CODE_INVALID_INPUT_ARGUMENT;
                    this->onError();
                    break;
                }

                //LAME encoding call will accept any number of samples.  
                if (0 == lame_get_version(pLame))
                    // For MPEG-II, only 576 samples per frame per channel
                    dwSamples = 576;// * lame_get_num_channels( pLame );
                else
                    // For MPEG-I, 1152 samples per frame per channel
                    dwSamples = 1152;// * lame_get_num_channels( pLame );

                // Set MP3 buffer size, conservative estimate
                dwMP3Buffer = (unsigned long long)(1.25 * (dwSamples/* / lame_get_num_channels( pLame )*/)+7200);

                CAudioFrame     frame, localFrame, mp3Frame;

                size_t nBytes = (size_t)dwSamples * 2 /*16 bit*/ * lame_get_num_channels(pLame);
                frame.resize(nBytes);
                mp3Frame.resize((size_t)dwMP3Buffer);

                if (!audioDecoder.beginDecode(outParams, audioTrackNumber))
                {
                    LOG_ERR("workerThread | beginDecode failed");
                    this->m_nLastError = CONV_ERROR_CODE_DECODE_FAILED;
                    this->onError();
                    break;
                }
#if defined(WIN32) || defined(WIN64)
                double dEndPos = DBL_MAX;
#else
                double dEndPos = std::numeric_limits<double>::max();
#endif

                bool bDecode = !this->m_bStop;

                double      timestamp = 0;
                size_t      offset = 0;
                double      timeFactor = dwSamples * TIME_RESCALE / (double)(outParams.sampleRate * outParams.duration);
                double progress = 0;
                unsigned long milliseconds_since_epoch_old = 0;

                while (bDecode)
                {
                    //Fill frame by bytes;
                    if (localFrame.getLength() <= offset)
                    {
                        //Load frame;
                        EDecodeResult eResult = audioDecoder.decodeFrame(timestamp, &localFrame);

                        switch (eResult)
                        {
                        case kDecodeResult_Eof:
                            bDecode = false;
                            break;
                        case kDecodeResult_Error:
                        {
                            bDecode = false;
                            LOG_ERR("workerThread | decodeFrame failed");
                            this->m_nLastError = CONV_ERROR_CODE_DECODE_FRAME;
                            this->onError();
                            break;
                        }
                        case kDecodeResult_Success:
                            bDecode = true;
                            break;
                        }

                        offset = 0;
                    }

                    if (dEndPos < timestamp)
                        break;

                    if (nBytes > frame.getLength())
                    {
                        size_t nBytesCopy = nBytes - frame.getLength();
                        offset += frame.copyFrom(localFrame, offset, nBytesCopy);;
                        continue;
                    }

                    progress += timeFactor;
                    this->m_progress = (float)progress;

                    bDecode = bDecode & !this->m_bStop;
                    unsigned long milliseconds_since_epoch_now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                    if (milliseconds_since_epoch_now - milliseconds_since_epoch_old > COUNT_OF_MILLISECONDS_TO_SEND_PROGRESS 
                        && bDecode) {
                        milliseconds_since_epoch_old = milliseconds_since_epoch_now;
                        this->onProgress();
                    }
                    

                    // Encode samples
                    short int* pSamples = (short int*)frame.getBuffer();

                    if (1 == lame_get_num_channels(pLame))
                        dwWrite = lame_encode_buffer(pLame, pSamples, pSamples, (int)dwSamples, mp3Frame.getBuffer(), 0);
                    else
                        dwWrite = lame_encode_buffer_interleaved(pLame, pSamples, (int)dwSamples, mp3Frame.getBuffer(), 0);

                    if (dwWrite < 0)
                    {
                        LOG_ERR("save | dwWrite < 0");
                        this->m_nLastError = CONV_ERROR_CODE_INTERNAL;
                        this->onError();
                        break;
                    }

                    frame.setLength(0);
                    mp3Frame.setLength((size_t)dwWrite);

                    fileMp3.write((const char*)mp3Frame.getBuffer(), mp3Frame.getLength());
                }

                audioDecoder.endDecode();

                dwWrite = lame_encode_flush_nogap(pLame, mp3Frame.getBuffer(), mp3Frame.getSize());
                if (dwWrite > 0)
                {
                    fileMp3.write((const char*)mp3Frame.getBuffer(), dwWrite);
                }
                else if (dwWrite < 0)
                {
                    TIER0_ERR_A("Mp3Lame", "MP3 Buffer Too Small");
                }

                fileMp3.flush();

                if (lame_get_bWriteVbrTag(pLame))
                {
                    //Write header;
                    fileMp3.seekp(0);

                    // write id3v2 header
                    auto id3v2_size = lame_get_id3v2_tag(pLame, 0, 0);
                    if (id3v2_size > 0) {
                        unsigned char* id3v2tag = (unsigned char*)malloc(id3v2_size);
                        if (id3v2tag != 0) {
                            size_t  n_bytes = lame_get_id3v2_tag(pLame, id3v2tag, id3v2_size);
                            fileMp3.write((const char*)id3v2tag, n_bytes);
                            free(id3v2tag);
                        }
                    }

                    size_t n = lame_get_lametag_frame(pLame, 0, 0); /* ask for bufer size */

                    if (n > 0 && !fileMp3.fail())
                    {
                        mp3Frame.resize(n);
                        /* Put it all to disk again */
                        n = lame_get_lametag_frame(pLame, mp3Frame.getBuffer(), n);
                        if (n > 0)
                        {
                            fileMp3.write((const char*)mp3Frame.getBuffer(), n);
                        }
                    }
                }
                bFinished = true;
            } while (false);

            // clean up of allocated memory
            if (pLame)
                lame_close(pLame);

            fileMp3.close();
            audioDecoder.close();

            if (bFinished)
            {
                //this->m_progress = 100;
                this->m_progress = 1.f;
                this->onStop();
            }
        }
        catch (...)
        {
            LOG_ERR("workerThread | exception");
            this->m_nLastError = CONV_ERROR_CODE_INTERNAL;
            this->onError();
        }
        LOG_INF("<-- workerThread");
    }
    
    TResult CLameTranscoder::createLameTranscoder(MMConv::ITransDelegate* pDelegate, std::shared_ptr<MMConv::IMediaTranscoder>& ppTranscoder)
    {
        //assert(ppTranscoder);
        TResult nRet = kResult_Success;
        
        auto spTranscoder = std::make_shared<MMConv::CLameTranscoder>();
        
        if ((nRet = spTranscoder->init(pDelegate)) == kResult_Success)
            ppTranscoder = spTranscoder;
        return nRet;
    }


    void CLameTranscoder::set_metadata_title(const std::wstring& metadata_title) {
        id3tag_title_ = metadata_title;
    }
    void CLameTranscoder::set_metadata_artist(const std::wstring& metadata_artist) {
        id3tag_artist_ = metadata_artist;
    }
    void CLameTranscoder::set_metadata_album(const std::wstring& metadata_album) {
        id3tag_album_ = metadata_album;
    }
    void CLameTranscoder::set_metadata_album_artist(const std::wstring& metadata_album_artist) {
        id3tag_album_artist_ = metadata_album_artist;
    }
    void CLameTranscoder::set_metadata_year(const std::wstring& metadata_year) {
        id3tag_year_ = metadata_year;
    }
    void CLameTranscoder::set_metadata_comment(const std::wstring& metadata_comment) {
        id3tag_comment_ = metadata_comment;
    }
    void CLameTranscoder::set_metadata_genre(const std::wstring& metadata_genre) {
        id3tag_genre_ = metadata_genre;
    }
    void CLameTranscoder::set_metadata_track(const std::wstring& metadata_track) {
        id3tag_track_ = metadata_track;
    }
    void CLameTranscoder::set_metadata_image_path(const std::string& metadata_image_path) {
        id3tag_image_path_ = metadata_image_path;
    }
} // namespace MMConv
