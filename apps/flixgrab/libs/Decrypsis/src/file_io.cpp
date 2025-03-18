#include "file_io.h"


FileIODummy::FileIODummy(cdm::FileIOClient* client) : file_client_(client)
{
   
    std::cout << "FileIO::Constructor: " << file_client_ << std::endl;
}

FileIODummy::~FileIODummy()
{
   
    std::cout << "FileIO::Destructor: " << file_client_ << std::endl;
}

void FileIODummy::Open(const char* file_name, uint32_t file_name_size)
{
    //std::cout << "[" << this << "] ";
    std::cout << "FileIO::Open " << std::string(file_name, file_name_size) << " : " << file_client_ << std::endl;

    file_name_ = std::string(file_name, file_name_size);
    file_client_->OnOpenComplete(cdm::FileIOClient::Status::kSuccess);

    /*file_.open(filename, std::fstream::in | std::fstream::out | std::ios::binary);

    if (file_.is_open()) {
    file_client_->OnOpenComplete(cdm::FileIOClient::kSuccess);
    }
    else {
    file_client_->OnOpenComplete(cdm::FileIOClient::kError);
    }*/
}

void FileIODummy::Read()
{
    //std::cout << "[" << this << "] ";
    std::cout << "FileIO::Read " << " : " << file_client_ << std::endl;
    file_client_->OnReadComplete(cdm::FileIOClient::Status::kSuccess, content_.data(), content_.size());
    /*file_.seekg(0, file_.end);
    size_t fsize = file_.tellg();
    file_.seekg(0, file_.beg);

    std::vector<uint8_t>  data(0, fsize);
    file_.read((char*)data.data(), fsize);

    file_client_->*/
}

void FileIODummy::Write(const uint8_t* data, uint32_t data_size)
{
    //std::cout << "[" << this << "] ";
    std::cout << "FileIO::Write " << " : " << file_client_ << std::endl;

    content_.assign(data, data + data_size);

    file_client_->OnWriteComplete(cdm::FileIOClient::Status::kSuccess);
}

void FileIODummy::Close()
{
    //std::cout << "[" << this << "] ";
    std::cout << "FileIO::Close: " << file_client_ << std::endl;

    delete this;
}
