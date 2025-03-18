#pragma once

#include <common_config.h>

#include <mutex>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <map>

#include "operation.h"

#define TTML_XML_NOT_LANG               L"not xml language"


namespace recode {
    enum EConvErrorCode  {
        CONV_ERROR_CODE_UNKNOWN = 0,
        CONV_ERROR_CODE_OPEN_INPUT_FILE,
        CONV_ERROR_CODE_OPEN_OUTPUT_FILE,
        CONV_ERROR_CODE_FORMAT_NO_SUPPORTED,
        CONV_ERROR_CODE_NO_DATA_FILES,
        CONV_ERROR_CODE_LANGUAGE_NOT_DEFINED
    };

    class ConvertToStrOp :
        public Operation {

        public:
            ConvertToStrOp(const std::string& tessdata_folder);
            virtual ~ConvertToStrOp();

            void                                                Add(const std::string& in_file, const std::wstring& out_file);
            Status                                              Do(OperationListener* listener) override;
            virtual void                                        Cancel() override;
            int                                                 StreamCount() { return streams_.size(); }
            void                                                SaveOriginalTrack(bool save_original_track = false) { save_original_track_ = save_original_track; }
            bool                                                SupportedFormat(std::wstring fileName);

        private:
            int                                                 findLast(std::wstring &str, std::wstring find_str);
            std::wstring                                        getFormatFile(std::wstring file_name);
            void                                                convertPathFile(std::wstring &path_file);
            std::pair<int, std::wstring>                        getTagXML(std::wstring &line, bool first = true);
            void                                                deleteAllSpace(std::wstring &str);
            void                                                handleSRT(std::wifstream &in, std::wofstream &out);
            void                                                handleTTML(std::wifstream &in, std::wofstream &out);
            void                                                handleVTT(std::wifstream &in, std::wofstream &out);
            std::wstring                                        getEncoding(std::wstring &line);
            long long                                           getTTP_tickRate(std::wstring &line);
            std::wstring                                        getXML_Language(std::wstring &line);
            bool                                                checkLineTTMLonSubtitles(std::wstring &line);
            std::wstring                                        getTTML2SRT(std::wstring &line);
            std::wstring                                        getTimeTTML2SRT(std::wstring &str);
        
        //TTMLv2 zip images
        private:
            std::vector<std::wstring>                           getLanguageForSourceFile(std::wstring& in);
            bool                                                checkLineXML_TTMLonSubtitles(std::wstring& line);//проверка строки, что это субтитры
            std::wstring                                        getTIMEfromXML_TTML(std::wstring& line);//взять время

        private:
            std::vector<std::pair<std::wstring, std::wstring>>  streams_;
            static std::mutex                                   mutex_;

        private:
            bool                                                ttmlEncoding_not_find = true;
            std::wstring                                        ttmlEncoding = L"";
            bool                                                ttp_tickRate_not_find = true;
            long long                                           ttp_tickRate = 1;
            bool                                                xml_language_not_find = true;
            std::wstring                                        xml_language = TTML_XML_NOT_LANG;
            std::string                                         tessdata_folder_;
            bool                                                save_original_track_ = false;
    };
}