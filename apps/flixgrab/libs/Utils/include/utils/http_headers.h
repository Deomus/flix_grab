#pragma once


#include <stdint.h>
#include <string>
#include <sstream>
#include <map>

namespace utils {

    inline std::vector<std::string> split(const std::string &s, char delim = '\n') {
        std::stringstream ss(s);
        std::string item;
        std::vector<std::string> tokens;
        while (getline(ss, item, delim))
            tokens.emplace_back(item);
        return std::move(tokens);
    }


    class HttpHeaders {
    public:
        enum Version {
            Unknown,
            HTTP_1_0,
            HTTP_1_1,
        };
    public:
        HttpHeaders() {}
        HttpHeaders(const char* headers) {
            SetHeaders(headers);
        }

        void            SetHeaders(const char* headers) {
            auto lines = split(headers);
            original_ = headers;

            for (auto& line : lines) {
                if (line.back() == '\r')
                    line.pop_back();
            }

            if (!lines.empty()) {
                //HTTP/1.1 200 OK
                auto specs = split(lines[0], ' ');

                if (specs.size() >= 2) {
                    if (specs[0] == "HTTP/1.0")
                        version_ = HTTP_1_0;
                    else if (specs[0] == "HTTP/1.1")
                        version_ = HTTP_1_1;
                    else
                        version_ = Unknown;

                    status_code_ = std::stoi(specs[1]);

                    //Make response string;
                    if (specs.size() > 2) {
                        response_ = specs[2];
                        for (size_t i = 3; i < specs.size(); ++i) {
                            response_ += ' ';
                            response_ += specs[i];
                        }
                    }
                    
                }
            
                for (size_t i = 1; i < lines.size(); ++i) {
                    auto header = split(lines[i], ':');
                    if (header.size() > 1)
                        headers_[header[0]] = header[1];
                }
            }

            

        }
        
        int64_t         ContentLength() const {
            auto it = headers_.find("Content-Length");
            int64_t length = 0;
            
            if (it != headers_.end()) 
                length = std::stoll(it->second.c_str());
            return length;
        }

        //Length from Content-Range header;
        int64_t         ContentRangeLength() const {
            auto it = headers_.find("Content-Range");
            int64_t length = 0;
            if (it != headers_.end())
                length = std::stoll(it->second.substr(it->second.find_last_of('/')+1));

            return length;
                
        }

        bool            IsAcceptRanges() const {
            auto it = headers_.find("Accept-Ranges");
            return it != headers_.end() ? 
                it->second.find("bytes") != std::string::npos : false;
        }

        bool            HasHeader(const char* header) const {
            return headers_.find(header) != headers_.end();
        }
        const char*     RawHeader(const char* header) const {
            auto it = headers_.find(header);
            if (it != headers_.end())
                return it->second.c_str();
            return nullptr;
        }

        bool            IsValid() const { return !headers_.empty(); }

        int             status_code() const { return status_code_; }
        const char*     response() const { return response_.c_str(); }

        const char*     original() const {return original_.c_str();}

    private:

        std::map<std::string, std::string>      headers_;
        //Headers version;
        Version                                 version_ = Version::HTTP_1_1;
        int                                     status_code_ = 0;
        std::string                             response_;

        std::string                             original_;
    };


}



