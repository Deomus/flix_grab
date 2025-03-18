#pragma once

#include <vector>
#include <cstdint>
#include <memory>

#include "content_decryption_module.h"

//class BufferBuilder;
class BufferBuilder;

class BufferPooling : public std::enable_shared_from_this<BufferPooling>
{
    friend class BufferBuilder;

public:
    BufferPooling(uint32_t max_buffers = 5);

public:
    cdm::Buffer*        Allocate(size_t size);
    void                Purge();

protected:
    void                Release(BufferBuilder* buffer);

private:
    uint32_t                                            max_buffers_;
    std::vector<std::unique_ptr<BufferBuilder, void(*)(BufferBuilder*)>>         buffers_;

};

