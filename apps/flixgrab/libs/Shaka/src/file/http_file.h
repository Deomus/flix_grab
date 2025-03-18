
#ifndef HTTP_FILE_H_
#define HTTP_FILE_H_

#include <stdint.h>

#include <string>
#include <memory>

#include "file.h"

namespace utils {
    class Http;
};

namespace shaka {
    namespace media {

       

        /// Implement LocalFile which deals with local storage.
        class HttpFile : public File {
        public:
            /// @param file_name C string containing the name of the file to be accessed.
            /// @param mode C string containing a file access mode, refer to fopen for
            ///        the available modes.
            HttpFile(const char* file_name, const char* mode);

            /// @name File implementation overrides.
            /// @{
            bool Close() override;
            int64_t Read(void* buffer, uint64_t length) override;
            int64_t Write(const void* buffer, uint64_t length) override;
            int64_t Size() override;
            bool Flush() override;
            bool Seek(uint64_t position) override;
            bool Tell(uint64_t* position) override;
            /// @}

            /// Delete a local file.
            /// @param file_name is the path of the file to be deleted.
            /// @return true if successful, or false otherwise.
            static bool Delete(const char* file_name);

     //   protected:
            ~HttpFile() override;

            bool    Open() override;
                    
            


        private:
            std::unique_ptr<utils::Http>        http_;
            
            std::string     file_mode_;

            int64_t         position_;
            int64_t         size_;
            

            DISALLOW_COPY_AND_ASSIGN(HttpFile);
        };

    }  // namespace media
}  // namespace shaka

#endif  // HTTP_FILE_H_
