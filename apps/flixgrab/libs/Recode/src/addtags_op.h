#pragma once
#pragma warning(disable : 4996)

//DLL Module
#pragma comment(lib, "tag.lib")
#pragma comment(lib, "tag_c.lib")

#include <mutex>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

#include <taglib/mpegfile.h>
#include <taglib/attachedpictureframe.h>
#include <taglib/id3v1tag.h>
#include <taglib/id3v2tag.h>
#include <taglib/mp4file.h>
#include <taglib/mp4tag.h>
#include <taglib/mp4coverart.h>
#include <taglib/vorbisfile.h>
#include <taglib/xiphcomment.h>
#include <taglib/flacpicture.h>
#include <taglib/fileref.h>

#include "operation.h"

namespace recode {
    class ImageFile : public TagLib::File {
    public:
        ImageFile(const char* file) : TagLib::File(file) {

        }

        TagLib::ByteVector data() {
            return readBlock(length());
        }

    private:
        virtual TagLib::Tag* tag() const { return 0; }
        virtual TagLib::AudioProperties* audioProperties() const { return 0; }
        virtual bool save() { return false; }
    };

    class AddTagsOp :
        public Operation {

        public:
            AddTagsOp();
            virtual ~AddTagsOp();

            void                                            Add(const std::wstring& out);
            Status                                          Do(OperationListener* listener) override;
            virtual void                                    Cancel() override;

            int                                             StreamCount() { return streams_.size(); }

            void                                            set_artwork_url(const std::wstring& artwork_url);
            void                                            set_metadata_title(const std::wstring& metadata_title);
            void                                            set_metadata_artist(const std::wstring& metadata_artist);
            void                                            set_metadata_album(const std::wstring& metadata_album);
            void                                            set_metadata_album_artist(const std::wstring& metadata_album_artist);
            void                                            set_metadata_track(const int metadata_track);
            void                                            set_metadata_year(const int metadata_year);
            void                                            set_metadata_comment(const std::wstring& metadata_comment);
            void                                            set_metadata_genre(const std::wstring& metadata_genre);
            void                                            set_metadata_image_path(const std::string& metadata_image_path);

        private:
            void                                            runFpcalc(const std::wstring& stream, std::string* fingerprint, std::string* duration);
        
        private:
            std::wstring                                    id3tag_title_;
            std::wstring                                    id3tag_artist_;
            std::wstring                                    id3tag_album_;
            std::wstring                                    id3tag_album_artist_;
            int                                             id3tag_track_ = 0;
            int                                             id3tag_year_ = 0;
            std::wstring                                    id3tag_comment_;
            std::wstring                                    id3tag_genre_;
            std::string                                     id3tag_image_path_;

        private:
            std::vector<std::wstring>                       streams_;
            static std::mutex                               mutex_;

        private:
            HANDLE                                          g_hChildStd_OUT_Rd = NULL;
            HANDLE                                          g_hChildStd_OUT_Wr = NULL;
    };
}