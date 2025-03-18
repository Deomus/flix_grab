#pragma once
#include <mmconv.h>

#include <vector>
#include <string>
#include <memory>
#include <stdint.h>
#include <thread>

namespace MMConv
{
    class CLameTranscoder 
        : public IMediaTranscoder
    {
    public:
        explicit CLameTranscoder();
        virtual ~CLameTranscoder();

    public:
        TResult                         init(ITransDelegate* pDelegate);
    public:
        virtual TResult                 start(const char* sInFileName, const std::wstring& sOutFileName, int audioOutputSampleRate = 0, int audioOutputBitrate = 0, int audioOutputChannels = 0);
        virtual TResult                 stop(void);
        virtual float                   getProgress();
        virtual EConvErrorCode          getErrorCode();

        virtual void                    set_metadata_title(const std::wstring& metadata_title) override;
        virtual void                    set_metadata_artist(const std::wstring& metadata_artist) override;
        virtual void                    set_metadata_album(const std::wstring& metadata_album) override;
        virtual void                    set_metadata_album_artist(const std::wstring& metadata_album_artist) override;
        virtual void                    set_metadata_year(const std::wstring& metadata_year) override;
        virtual void                    set_metadata_comment(const std::wstring& metadata_comment) override;
        virtual void                    set_metadata_genre(const std::wstring& metadata_genre) override;
        virtual void                    set_metadata_track(const std::wstring& metadata_track) override;
        virtual void                    set_metadata_image_path(const std::string& metadata_image_path) override;

        virtual void                    shutdown(void);
    public:
        virtual void                    onProgress();
        virtual void                    onError();
        virtual void                    onStart();
        virtual void                    onStop();

    protected:
        
        void                            setStreamOptions(const std::string& sStSpec, void* spOutStream, std::stringstream& ssCmd);
        void                            setOutputOptions(void* spOutFile, std::stringstream& ssCmd);
        void                            setInputOptions(void* spInFile, std::stringstream& ssCmd);


    private:
        void                            workerThread();
        std::vector<unsigned char>      getUtf16WithBom(const std::wstring& str);

    private:
        std::string                     sInFileName; // "http://player.ooyala.com/player/ipad/J4NXB4djqiGBj6JAGO_YxRX6KRPdM2Vk_3500.m3u8"
        std::wstring                    sOutFileName;

        MMConv::ITransDelegate*         m_spDelegate = nullptr;

        float                           m_progress = -1;
        EConvErrorCode                  m_nLastError = CONV_ERROR_CODE_SUCCESS;
        std::thread                     m_worker;
        bool                            m_bStop = false;

        int m_audioOutputSampleRate = 0; // Hz, 0 - keep original
        int m_audioOutputBitrate = 0;    // bps, -1=STANDARD, -2=EXTREME, -3=INSANE, -4=R3MIX, -10=keep original, 0=lame auto bitrate
        int m_audioOutputChannels = 0;   // 0 - keep original

        std::wstring id3tag_title_;
        std::wstring id3tag_artist_;
        std::wstring id3tag_album_;
        std::wstring id3tag_album_artist_;
        std::wstring id3tag_year_;
        std::wstring id3tag_comment_;
        std::wstring id3tag_genre_;
        std::wstring id3tag_track_;
        std::string  id3tag_image_path_;

    public:
        static TResult createLameTranscoder(MMConv::ITransDelegate* pDelegate, std::shared_ptr<MMConv::IMediaTranscoder>& ppTranscoder);

    };


} //namespace MMConv
