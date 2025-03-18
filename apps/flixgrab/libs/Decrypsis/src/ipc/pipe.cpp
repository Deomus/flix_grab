#include "pipe.h"
#include <iostream>
#include <windows.h>

using namespace ipc;

struct ipc::PipeHeader
{
    uint32_t        size;
    PipeOp          current_op;
    bool            is_valid;
};

enum { kHeader_Size = sizeof(ipc::PipeHeader) };

Pipe::Pipe() 
    : file_(NULL)
    , mem_handle_(NULL)
    , reading_evt_handle_(NULL)
    , writing_evt_handle_(NULL)
    , buffer_header_(NULL)
    , is_server_(false)
{

}

Pipe::~Pipe()
{
    Close();
}

bool Pipe::Open(const std::string& name)
{
    std::string mem_name = "Local\\" + name + std::string("_mem");
    std::string read_name = "Local\\" + name + std::string("_rd");
    std::string write_name = "Local\\" + name + std::string("_wr");


    mem_handle_ = OpenFileMappingA(
        FILE_MAP_ALL_ACCESS,   // read/write access
        FALSE,                 // do not inherit the name
        mem_name.c_str());               // name of mapping object

    if (mem_handle_ == NULL) {
        std::cout << " Could not open file mapping object: " << GetLastError() << std::endl;
    }
    else {

        //Initialize Buffer;
        file_ = (uint8_t*)MapViewOfFile(mem_handle_, // handle to map object
            FILE_MAP_ALL_ACCESS,  // read/write permission
            0,
            0,
            kHeader_Size);

        if (file_)
        {
            //Read Bufffer Size;
            buffer_header_ = (PipeHeader*)file_;
            int32_t buffer_size = buffer_header_->size;

            UnmapViewOfFile(file_);

            //Remap File;

            file_ = (uint8_t*)MapViewOfFile(mem_handle_, // handle to map object
                FILE_MAP_ALL_ACCESS,  // read/write permission
                0,
                0,
                buffer_size + kHeader_Size);
        }

        if (file_)
        {
            buffer_header_ = (PipeHeader*)file_;
            buffer_ = file_ + kHeader_Size;

            //Init Events;
            writing_evt_handle_ = OpenEventA(EVENT_ALL_ACCESS, FALSE, write_name.c_str());

            if (writing_evt_handle_ == NULL) {
                std::cout << " Could not open event object: " << write_name << " error: " << GetLastError() << std::endl;
            }

            reading_evt_handle_ = OpenEventA(EVENT_ALL_ACCESS, FALSE, read_name.c_str());

            if (reading_evt_handle_ == NULL) {
                std::cout << " Could not open event object: " << read_name << " error: " << GetLastError() << std::endl;
            }

        }

    }

    return (file_ != NULL);
}

bool Pipe::Create(const std::string& name, int32_t size)
{
    if (file_) Close();

    std::string mem_name = "Local\\" + name + std::string("_mem");
    std::string read_name = "Local\\" + name + std::string("_rd");
    std::string write_name = "Local\\" + name + std::string("_wr");

    mem_handle_ = CreateFileMappingA(
        INVALID_HANDLE_VALUE,    // use paging file
        NULL,                    // default security
        PAGE_READWRITE,          // read/write access
        0,                       // maximum object size (high-order DWORD)
        size + kHeader_Size,                // maximum object size (low-order DWORD)
        mem_name.c_str());                 // name of mapping object

    if (mem_handle_ == NULL)
    {
        std::cout << " Could not create file mapping object: " << mem_name << " error: " << GetLastError() << std::endl;
    }
    else {

        //Initialize Buffer;

        if (file_ != NULL)
            ::UnmapViewOfFile(file_);
        file_ = (uint8_t*)MapViewOfFile(mem_handle_, // handle to map object
            FILE_MAP_ALL_ACCESS,  // read/write permission
            0,
            0,
            size + kHeader_Size);

        if (file_ == NULL)
            return false;

        //Store Size In Header;
        buffer_header_ = (PipeHeader*)file_;
        buffer_header_->size = size;
        buffer_header_->current_op = kPipeOp_None;
        buffer_header_->is_valid = true;

        buffer_ = file_ + kHeader_Size;

        writing_evt_handle_ = CreateEventA(NULL, FALSE, TRUE, write_name.c_str());
              

        if (writing_evt_handle_ == NULL) {
            std::cout << " Could not create event object: " << write_name << " error: " << GetLastError() << std::endl;
        }
        else if (GetLastError() == ERROR_ALREADY_EXISTS) {
            std::cout << " Event object: " << write_name << " already has" << std::endl;
            SetEvent(writing_evt_handle_);
        }

        reading_evt_handle_ = CreateEventA(NULL, FALSE, FALSE, read_name.c_str());
        
        if (reading_evt_handle_ == NULL) {
            std::cout << " Could not create event object: " << read_name << " error: " << GetLastError() << std::endl;
        }
        else if (GetLastError() == ERROR_ALREADY_EXISTS) {
            std::cout << " Event object: " << read_name << " already has" << std::endl;
            ResetEvent(reading_evt_handle_);
        }
        is_server_ = true;
    }

    return (file_ != NULL);
}

bool Pipe::Lock(PipeOp op, uint8_t** buffer, int32_t* buffer_size)
{
    assert(buffer_header_);

    if (!buffer_header_->is_valid) return false;

    /*if (buffer_header_->current_op != kPipeOp_None)
    {
        std::cout << "Current Operation not finished: " << buffer_header_->current_op << std::endl;
        return false;
    }*/

    if (WAIT_OBJECT_0 != ::WaitForSingleObject(GetLockEvent(op), INFINITE)) {
        std::cout << "WaitForSingleObject Error " << GetLastError() << std::endl;
        return false;
    }

    assert(buffer_header_->current_op == kPipeOp_None);

    buffer_header_->current_op = op;
    *buffer = buffer_;
    *buffer_size = buffer_header_->size;
    return true;
}

void Pipe::Unlock()
{
    assert(buffer_header_);
    PipeOp op = buffer_header_->current_op;
    buffer_header_->current_op = kPipeOp_None;

    SetEvent(GetUnlockEvent(op));
}

void Pipe::Close()
{
    if (is_server_ && file_)
        buffer_header_->is_valid = false;

    if (reading_evt_handle_) {
        CloseHandle(reading_evt_handle_);
        reading_evt_handle_ = nullptr;
    }

    if (writing_evt_handle_) {
        CloseHandle(writing_evt_handle_);
        writing_evt_handle_ = nullptr;
    }

    if (file_) {
        UnmapViewOfFile(file_);
        file_ = nullptr;
    }

    if (mem_handle_) {
        CloseHandle(mem_handle_);
        mem_handle_ = nullptr;
    }

   
}
