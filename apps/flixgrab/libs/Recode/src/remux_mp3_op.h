#pragma once
#include <string>
#include <vector>
#include <map>
#include <mutex>

#include "status.h"

#include "operation.h"
#include "mmconv.h"


namespace recode {

    class RemuxMP3Op
        : public Operation , public MMConv::ITransDelegate
    {

    public:
        RemuxMP3Op();
        virtual ~RemuxMP3Op();

        Status              Do(OperationListener* listener) override;
        void                Cancel() override;

        void                AddStream(const std::string& url, const std::pair<std::wstring, int>& out);
        int                 StreamCount() { return streams_.size(); }

        void                set_metadata_title(const std::wstring& metadata_title);
        void                set_metadata_artist(const std::wstring& metadata_artist);
        void                set_metadata_album(const std::wstring& metadata_album);
        void                set_metadata_album_artist(const std::wstring& metadata_album_artist);
        void                set_metadata_year(const std::wstring& metadata_year);
        void                set_metadata_comment(const std::wstring& metadata_comment);
        void                set_metadata_genre(const std::wstring& metadata_genre);
        void                set_metadata_track(const std::wstring& metadata_track);
        void                set_metadata_image_path(const std::string& metadata_image_path);

    private:
        int lamePresetValue(const int preset);

    private:
        std::shared_ptr<MMConv::IMediaTranscoder>       transcoder_;
        std::string                                     url_;
        std::wstring                                    output_file_;
        int                                             preset_;
        std::vector<std::pair<std::string, std::pair<std::wstring, int>>> streams_;

        std::mutex                                      mutex_;
        std::condition_variable                         cv_;        // wait for encoder
        OperationListener *                             listener_;  // callback
    
    private:
        std::wstring                                    id3tag_title_;
        std::wstring                                    id3tag_artist_;
        std::wstring                                    id3tag_album_;
        std::wstring                                    id3tag_album_artist_;
        std::wstring                                    id3tag_year_;
        std::wstring                                    id3tag_comment_;
        std::wstring                                    id3tag_genre_;
        std::wstring                                    id3tag_track_;
        std::string                                     id3tag_image_path_;

    //ITransDelegate impl:
    public:
        virtual void                    onStart(MMConv::IMediaTranscoder*) override {}
        virtual void                    onStop(MMConv::IMediaTranscoder* ) override
        {
            std::cout << " ALL DONE !" << std::endl;
            cv_.notify_one();
        }
        virtual void                    onProgress(MMConv::IMediaTranscoder* p) override {
            std::cout << " Progress: " << (uint16_t)(p->getProgress() * 100) << "%" << std::endl;
            if (listener_)
                listener_->OnProgress(p->getProgress());
        }
        virtual void                    onError(MMConv::IMediaTranscoder*) override {
            std::cout << " ERROR !" << std::endl;
            cv_.notify_one();
        }

    };
}