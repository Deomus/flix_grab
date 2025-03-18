#pragma once

#include <string>
#include <cstdint>
#include <assert.h>

#include <memory>

#include "buffer_reader.h"
#include "buffer_writer.h"

namespace ipc {
    typedef void *HANDLE;

    enum PipeOp
    {
        kPipeOp_None = 0,
        kPipeOp_Read,
        kPipeOp_Write,
    };

    struct PipeHeader;

    class Pipe
    {

    public:
        Pipe();
        ~Pipe();


        //Open client;
        bool            Open(const std::string& name);
        //Create server;
        bool            Create(const std::string& name, int32_t size);


        //Lock buffer for Read;
        bool            Lock(PipeOp op, uint8_t** buffer, int32_t* buffer_size);
        //Unlock for write;
        void            Unlock();

        //Close clent/server handlers;
        void            Close();

        bool            IsValid() const { return (file_ != NULL && reading_evt_handle_ != NULL && writing_evt_handle_ != NULL); }

    private:


        HANDLE          GetLockEvent(PipeOp op)
        {
            return (op == kPipeOp_Read) ? reading_evt_handle_ : (op == kPipeOp_Write) ? writing_evt_handle_ : NULL;
        }

        HANDLE          GetUnlockEvent(PipeOp op)
        {
            return (op == kPipeOp_Read) ? writing_evt_handle_ : (op == kPipeOp_Write) ? reading_evt_handle_ : NULL;
        }

    private:
        //Mapped File;
        uint8_t*                file_;
        //Usable Buffer;
        uint8_t*                buffer_;

        HANDLE                  mem_handle_;

        HANDLE                  reading_evt_handle_;
        HANDLE                  writing_evt_handle_;


        PipeHeader*             buffer_header_;

        bool                    is_server_;

    };

    class PipeReader : public BufferReader {
    public:
        PipeReader(Pipe& pipe) : pipe_(pipe) {
            uint8_t*                buffer = 0;
            int32_t                 buffer_size = 0;
            if (pipe_.Lock(kPipeOp_Read, &buffer, &buffer_size))
                Init(buffer, buffer_size);
        }

        ~PipeReader() {
            pipe_.Unlock();
        }

    private:
        PipeReader(const PipeReader&);
        void operator=(const PipeReader&);

    private:
        Pipe&           pipe_;
    };

    class PipeWriter : public BufferWriter {
    public:
        PipeWriter(Pipe& pipe) : pipe_(pipe) {
            uint8_t*                buffer = 0;
            int32_t                 buffer_size = 0;
            if (pipe_.Lock(kPipeOp_Write, &buffer, &buffer_size))
                Init(buffer, buffer_size);
        }

        ~PipeWriter() {
            pipe_.Unlock();
        }

    private:
        PipeWriter(const PipeWriter&);
        void operator=(const PipeWriter&);

    private:
        Pipe&           pipe_;
    };
}
