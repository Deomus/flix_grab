#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "content_decryption_module.h"


class FileIODummy : public cdm::FileIO
{
public:
    // Opens the file with |file_name| for read and write.
    // FileIOClient::OnOpenComplete() will be called after the opening
    // operation finishes.
    // - When the file is opened by a CDM instance, it will be classified as "in
    //   use". In this case other CDM instances in the same domain may receive
    //   kInUse status when trying to open it.
    // - |file_name| must not contain forward slash ('/') or backslash ('\'), and
    //   must not start with an underscore ('_').
    virtual void Open(const char* file_name, uint32_t file_name_size);

    // Reads the contents of the file. FileIOClient::OnReadComplete() will be
    // called with the read status. Read() should not be called if a previous
    // Read() or Write() call is still pending; otherwise OnReadComplete() will
    // be called with kInUse.
    virtual void Read();

    // Writes |data_size| bytes of |data| into the file.
    // FileIOClient::OnWriteComplete() will be called with the write status.
    // All existing contents in the file will be overwritten. Calling Write() with
    // NULL |data| will clear all contents in the file. Write() should not be
    // called if a previous Write() or Read() call is still pending; otherwise
    // OnWriteComplete() will be called with kInUse.
    virtual void Write(const uint8_t* data, uint32_t data_size);

    // Closes the file if opened, destroys this FileIO object and releases any
    // resources allocated. The CDM must call this method when it finished using
    // this object. A FileIO object must not be used after Close() is called.
    virtual void Close();

public:
    FileIODummy(cdm::FileIOClient* client);
    virtual ~FileIODummy();

private:
  
    std::string                     file_name_;
    std::vector<uint8_t>            content_;
    cdm::FileIOClient*              file_client_;
      
};