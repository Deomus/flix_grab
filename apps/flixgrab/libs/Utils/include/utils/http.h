#pragma once

#include <stdint.h>
#include <string>
#include <vector>
#include <memory>

#include "status.h"
#include "http_headers.h"

namespace utils {
   
  
    class Http {
        friend class HttpPrivate;
    public:
      /*  enum Status {
            Success = 0,
            InternalError,
            InvalidUrl,
            ConnectionError,
            BufferOverflow,
            Timeout,
            NoMemory
        };*/

    public:
       
        Http();
        ~Http();

    public:

        Status      Open(const char* url);
        const char* url() const;
        //Send HEAD request
        Status      Headers(HttpHeaders* headers);
        //Send GET request
        Status      Get(int64_t from = 0, int64_t to = -1, HttpHeaders* headers = nullptr);
        //After GET request
        Status      Read(void* buffer, uint64_t* length);
        Status      DataAvailable(uint32_t* length);

        void        Close();

    private:
        std::unique_ptr<HttpPrivate>     private_;

    private:
        //DISABLE_COPY
        Http(const Http&);
        void operator=(const Http&);
    };

   
}


