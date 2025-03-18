#include "buffer_pooling.h"
#include <iostream>

class BufferBuilder : public cdm::Buffer
{
    friend class BufferPooling;
public:
    // Destroys the buffer in the same context as it was created.
    virtual void Destroy() {
        std::shared_ptr<BufferPooling>	local_pooling;
        pooling_.swap(local_pooling);
        //MUST be last call in destructor;
        if (local_pooling)
            local_pooling->Release(this);  
    }

    virtual uint32_t Capacity() const { return buffer_.capacity(); }
    virtual uint8_t* Data() { return buffer_.data(); }
    virtual void SetSize(uint32_t size) { buffer_.resize(size); }
    virtual uint32_t Size() const { return buffer_.size(); }

    // protected:

    BufferBuilder()
    {
        std::cout << "Constructor Buffer" << std::endl;
    }
    virtual ~BufferBuilder()
    {
        std::cout << "Destructor Buffer" << std::endl;
    }


    void            Allocate(uint32_t size, const std::shared_ptr<BufferPooling>& pooling) {
        SetSize(size);
        pooling_ = pooling;
    }

    static void destroy(BufferBuilder* ptr) { delete ptr; }

private:
    BufferBuilder(const BufferBuilder&);
    void operator=(const BufferBuilder&);

private:
    std::vector<uint8_t>                buffer_;
    std::shared_ptr<BufferPooling>      pooling_;

};

BufferPooling::BufferPooling(uint32_t max_buffers /*= 2*/) : max_buffers_(max_buffers)
{

}

cdm::Buffer* BufferPooling::Allocate(size_t size)
{
    BufferBuilder* buffer = nullptr;
    if (buffers_.size() > 0) {

        buffer = buffers_.back().release();
        buffers_.pop_back();
    }
    else {
        buffer = new BufferBuilder();
    }
    buffer->Allocate(size, this->shared_from_this());
    return buffer;
}

void BufferPooling::Purge()
{
    buffers_.clear();
}

void BufferPooling::Release(BufferBuilder* buffer)
{
    if (buffers_.size() < max_buffers_)
        buffers_.push_back(std::unique_ptr<BufferBuilder, void(*)(BufferBuilder*)>(buffer, &BufferBuilder::destroy));
    else
        delete buffer;
}
