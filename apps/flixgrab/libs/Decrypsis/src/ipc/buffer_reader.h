
#pragma once 

#include <stdint.h>

#include <string>
#include <vector>
#include <functional>

namespace ipc {
    typedef std::function<bool(const uint8_t* block_data, uint32_t block_size)>     block_reader_fn;
    /// A simple buffer reader implementation, which reads data of various types
    /// from a fixed byte array.
    class BufferReader {
    public:
        /// Create a BufferReader from a raw buffer.
        BufferReader(const uint8_t* buf, size_t size)
            : buf_(buf), size_(size), pos_(0) {}
        ~BufferReader() {}

        /// @return true if there are more than @a count bytes in the stream, false
        ///         otherwise.
        bool HasBytes(size_t count) const { return pos() + count <= size(); }


        /// Read N-byte integer of the corresponding signedness and store it in the
        /// 8-byte return type.
        /// @param num_bytes should not be larger than 8 bytes.
        /// @return false if there are not enough bytes in the buffer, true otherwise.
        /// @{
        bool ReadNBytesInto8(uint64_t* v, size_t num_bytes)
        {
            return ReadNBytes(v, num_bytes);
        }
        bool ReadNBytesInto8s(int64_t* v, size_t num_bytes)
        {
            return ReadNBytes(v, num_bytes);
        }
        /// @}

        template<typename T>
        bool ReadToVector(std::vector<T>* vec, size_t count) const
        {
            if (!HasBytes(count))
                return false;
            T* buf_data = (T*)(buf_ + pos_);
            vec->assign(buf_data, buf_data + count);
            pos_ += count;
            return true;
        }

        bool ReadToData(uint8_t* data, size_t count) const
        {
            if (!HasBytes(count))
                return false;

            memcpy(data, buf_ + pos_, count);
            pos_ += count;
            return true;
        }

        bool ReadToString(std::string* str, size_t size) const
        {
            if (!HasBytes(size))
                return false;
            if (size > 0) {
                str->assign(buf_ + pos_, buf_ + pos_ + size);
                pos_ += size;
            }

            return true;
        }

        bool ReadToWString(std::wstring* str, size_t size) const
        {
            if (!HasBytes(size))
                return false;
            if (size > 0) {
                str->assign(buf_ + pos_, buf_ + pos_ + size);
                pos_ += size;
            }

            return true;
        }

        /// Advance the stream by this many bytes.
        /// @return false if there are not enough bytes in the buffer, true otherwise.
        bool SkipBytes(size_t num_bytes)
        {
            if (!HasBytes(num_bytes))
                return false;
            pos_ += num_bytes;
            return true;
        }

        
        const uint8_t* data() const { return buf_; }
        size_t size() const { return size_; }
        void set_size(size_t size) { size_ = size; }
        size_t pos() const { return pos_; }

        // Internal implementation of multi-byte reads.
        template <typename T>
        bool Read(T* t)
        {
            return ReadNBytes(t, sizeof(*t));
        }

        template <typename T>
        bool Pod(T& t) const
        {
            return ReadNBytes(&t, sizeof(t));
        }

        template <typename T>
        bool Vector(std::vector<T>& v) const
        {
            size_t size = 0;
            if (Pod(size) &&
                ReadToVector(&v, size))
                return true;
            return false;
        }

        bool String(std::string& v) const
        {
            size_t size = 0;
            if (Pod(size) &&
                ReadToString(&v, size))
                return true;
            return false;
        }

        bool WString(std::wstring& v) const
        {
            size_t size = 0;
            if (Pod(size) &&
                ReadToWString(&v, size))
                return true;
            return false;
        }

        bool GetBlock(const uint8_t*& ptr, size_t& num_bytes)
        {
            if (Pod(num_bytes)) {
                ptr = current_data();
                return SkipBytes(num_bytes);
            }

            return false;
        }


        bool        Block(block_reader_fn reader) const
        {
            size_t size = 0;
            if (Pod(size) && reader(current_data(), size)) {
                pos_ += size;
                return true;
            }
            return false;
        }

        void        Reset() const { pos_ = 0; }

    protected:
        BufferReader() : buf_(NULL), size_(0), pos_(0) {}

        void    Init(const uint8_t* buf, size_t size) {
            buf_ = buf;
            size_ = size;
        }


    private:

        inline const uint8_t* current_data() const { return buf_ + pos_; }


        //template <typename T>
        //bool ReadNBytes(T* t, size_t num_bytes)
        //{
        //    if (!HasBytes(num_bytes))
        //        return false;

        //    // Sign extension is required only if
        //    //     |num_bytes| is less than size of T, and
        //    //     T is a signed type.
        //    const bool sign_extension_required =
        //        num_bytes < sizeof(*t) && static_cast<T>(-1) < 0;
        //    // Perform sign extension by casting the byte value to int8_t, which will be
        //    // sign extended automatically when it is implicitly converted to T.
        //    T tmp = sign_extension_required ? static_cast<int8_t>(buf_[pos_++])
        //        : buf_[pos_++];
        //    for (size_t i = 1; i < num_bytes; ++i) {
        //        tmp <<= 8;
        //        tmp |= buf_[pos_++];
        //    }
        //    *t = tmp;
        //    return true;
        //}

        template <typename T>
        bool ReadNBytes(T* t, size_t num_bytes) const
        {
            if (!HasBytes(num_bytes))
                return false;

            memcpy(t, buf_ + pos_, num_bytes);
            pos_ += num_bytes;
            return true;

        }

        const uint8_t* buf_;
        size_t size_;
        mutable size_t pos_;


    };


}

    