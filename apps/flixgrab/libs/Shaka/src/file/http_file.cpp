
#include "http_file.h"

#include <stdio.h>

#include "../url.h"
#include <base/strings/string_utils.h>


#include <utils/http.h>
#include <algorithm>
#include <base\logging.h>



namespace shaka {
    namespace media {

        HttpFile::HttpFile(const char* file_name, const char* mode)
            : File(file_name)
            , file_mode_(mode)
            , size_(0) {
            http_ = std::make_unique<utils::Http>();
        }
        HttpFile::~HttpFile() {
            Close();
        }

        bool HttpFile::Open() {

            utils::HttpHeaders headers;
            utils::Status status;
            if (status.Update(http_->Open(this->file_name().c_str())) &&
                status.Update(http_->Headers(&headers))) {
                size_ = headers.ContentLength();
                bool accept_ranges = headers.HasHeader("Accept-Ranges") ?
                    strstr(headers.RawHeader("Accept-Ranges"), "bytes") : false;

                if (headers.HasHeader("Content-Range"))
                    size_ = headers.ContentRangeLength();
                else if (!accept_ranges) {
                    //Ranges skiped by server;
                    size_ = 0;
                }
            }
            return status;
        }


        bool HttpFile::Close() {
            // Close any open handles.
           
            http_->Close();

            return true;
        }

#define CHUNK_SIZE              (16*1024)
        int64_t HttpFile::Read(void* buffer, uint64_t length) {
            DCHECK(buffer != NULL);
    
            utils::Status status;
            utils::HttpHeaders headers;

            uint64_t last_position = std::min<uint64_t>(position_ + length, size_);
                        
            if (last_position <= (uint64_t)position_)
                return 0;
            
            status = http_->Get(position_, last_position, &headers);

            uint64_t read = 0;
            size_t chunk_size = CHUNK_SIZE;
            uint8_t* buffer_bytes = static_cast<uint8_t*>(buffer);

            while (status && chunk_size == CHUNK_SIZE) {
                status = http_->Read(buffer_bytes, &chunk_size);
                read += chunk_size;
                buffer_bytes += chunk_size;
            }

            if (!status)
                return -1;

            position_ += read;
            return read;
           
        }

        int64_t HttpFile::Write(const void* buffer, uint64_t length) {
           
            return 0;
        }

        int64_t HttpFile::Size() {
            
            return size_;
        }

        bool HttpFile::Flush() {
           
            return true;
        }

        bool HttpFile::Seek(uint64_t position) {

            if ((int64_t)position < size_)
                position_ = position;
            else
                return false;

            return true;
        }

        bool HttpFile::Tell(uint64_t* position) {
            *position = position_;
            return true;
        }

      


    }  // namespace media
}  // namespace shaka
