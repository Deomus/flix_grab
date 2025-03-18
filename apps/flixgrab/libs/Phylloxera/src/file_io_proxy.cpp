#include "file_io_proxy.h"

#include "host_proxy.h"

FileIOProxy::FileIOProxy(HostProxy* host, cdm::FileIO* file, cdm::FileIOClient* client) : host_(host), file_(file), file_client_(client)
{
    id_ = host_->id();

    std::cout << "[" << id_ << "] ";
    std::cout << "FileIO::Constructor" << std::endl;
}

FileIOProxy::~FileIOProxy()
{
    //delete file_;

    std::cout << "[" << id_ << "] ";
    std::cout << "FileIO::Destructor" << std::endl;
}
