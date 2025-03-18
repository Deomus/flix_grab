
#pragma once 

#include <stdint.h>

#include <string>
#include <vector>
#include <functional>

namespace ipc {
    typedef std::function<bool(uint8_t* block_data, size_t& block_size)>     block_writer_fn;

    // A simple buffer writer implementation which appends various data types to
    /// buffer.
    class BufferWriter {
    public:
        /// Create a BufferReader from a raw buffer.
        BufferWriter(uint8_t* buf, size_t size)
            : buf_(buf), size_(size), pos_(0) {}
        ~BufferWriter() {}


        /*  /// These convenience functions append the integers (in network byte order,
        /// i.e. big endian) of various size and signedness to the end of the buffer.
        /// @{
        void AppendInt(uint8_t v);
        void AppendInt(uint16_t v);
        void AppendInt(uint32_t v);
        void AppendInt(uint64_t v);
        void AppendInt(int16_t v);
        void AppendInt(int32_t v);
        void AppendInt(int64_t v);
        /// @}*/


        bool AppendArray(const uint8_t* buf, size_t size)
        {
            if (!HasBytes(size)) return false;
            memcpy(buf_ + pos_, buf, size);
            pos_ += size;
            return true;
        }

        bool AppendArray(const int8_t* buf, size_t size)
        {
            if (!HasBytes(size)) return false;
            memcpy(buf_ + pos_, buf, size);
            pos_ += size;
            return true;
        }



        // Internal implementation of multi-byte write.
        template <typename T>
        bool Append(T v)
        {
            return AppendArray(reinterpret_cast<uint8_t*>(&v), sizeof(T));
        }

        template <typename T>
        bool Pod(const T& t)
        {
            return AppendArray((uint8_t*)&t, sizeof(t));
        }

        template <typename T>
        bool Vector(const std::vector<T>& v)
        {
            if (Pod(v.size()) &&
                AppendArray((uint8_t*)v.data(), v.size()*sizeof(T)))

                return true;

            return false;
        }

        bool String(const std::string& v)
        {
            if (Pod(v.size()) &&
                AppendArray((int8_t*)v.data(), v.size()))

                return true;

            return false;

        }

        bool WString(const std::wstring& v)
        {
            if (Pod(sizeof(wchar_t)*v.size()) &&
                AppendArray((int8_t*)v.data(), sizeof(wchar_t)*v.size()))

                return true;

            return false;

        }

        bool    Data(const uint8_t* v, const size_t& count)
        {

            if (Pod(count) &&
                AppendArray(v, count))

                return true;

            return false;

        }


        bool        Block(block_writer_fn writer)
        {
            size_t& size = make_ref<size_t>();
            size = size_ - pos_;

            if (Pod(size) && writer(current_data(), size)) {
                pos_ += size;
                return true;
            }
            return false;
        }

        void        Reset() { pos_ = 0; }

        /// @return true if there are more than @a count bytes in the stream, false
        ///         otherwise.
        bool HasBytes(size_t count) { return pos() + count <= size(); }

        const uint8_t* data() const { return buf_; }

        uint8_t* data() { return buf_; }



        size_t size() const { return size_; }
        void set_size(size_t size) { size_ = size; }
        size_t pos() const { return pos_; }

    protected:
        BufferWriter() : buf_(NULL), size_(0), pos_(0) {}

        void    Init(uint8_t* buf, size_t size) {
            buf_ = buf;
            size_ = size;
        }


    private:

        const uint8_t* current_data() const { return buf_ + pos_; }

        uint8_t* current_data() { return buf_ + pos_; }

        template <typename T>
        T&      make_ref() { return *(T*)(buf_ + pos_); }

    private:

        uint8_t* buf_;
        size_t size_;
        size_t pos_;

    };


}

    
