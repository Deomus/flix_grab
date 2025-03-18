//databuf.h

#ifndef COMMON__DATABUF_H
#define COMMON__DATABUF_H

#include <stdlib.h>
#include <string>

namespace sys {

static char g_pszEmptyString[1];

class IDataBuff {
public:
    virtual bool resize(size_t size) = 0;

    virtual void free(void) = 0;

    virtual void set(void *pData, size_t cchSize) = 0;

    virtual void set(const std::string& data) = 0;

    virtual void *data(void) = 0;

    virtual void attach(IDataBuff *pIBuf) = 0;

    virtual void attach(void *pBytes, size_t cchSize) = 0;

    virtual void detach(sys::IDataBuff& receiver) = 0;

    virtual void *detach(size_t& cchSize) = 0;

    virtual void getStr(std::string& sBuf) = 0;

    virtual void getStr(std::wstring& str) = 0;

    virtual std::string getString() = 0;

    virtual size_t size(void) = 0;

    virtual bool empty(void) = 0;
};


inline std::string getString(IDataBuff *ibuff) {
    std::string s;
    ibuff->getStr(s);
    return s;
}


class DataBuff : public IDataBuff {
public:
    /*!
     * \fn  DataBuff::DataBuff()
     * \brief   Default constructor.
     */
    DataBuff() :
        data_(nullptr)
        , size_(0) {
    }

    /*!
     * \fn  DataBuff::DataBuff(size_t nSize)
     * \brief   Constructor.
     * \param   nSize   The size.
     */
    DataBuff(size_t nSize) :
        data_(nullptr)
        , size_(0) {
        this->resize(nSize);
    }

    /*!
     * \fn  DataBuff::DataBuff(DataBuff&& other)
     * \brief   Move constructor.
     * \param [in,out]  other   The other.
     */
    DataBuff(DataBuff&& other) :
        size_(other.size_) {
        data_ = other.data_;
        other.data_ = nullptr;
        other.size_ = 0;
    }

    /*!
     * \fn  DataBuff& DataBuff::operator=(DataBuff&& rhs)
     * \brief   Move assignment operator.
     * \param [in,out]  rhs The right hand side.
     * \return  A shallow copy of this object.
     */
    DataBuff& operator=(DataBuff&& rhs) {
        rhs.detach(*this);
        return *this;
    }

    /*!
     * \fn  virtual DataBuff::~DataBuff()
     * \brief   Destructor.
     */
    virtual ~DataBuff() {
        free();
    }

    /*!
     * \fn  virtual bool DataBuff::resize(size_t size)
     * \brief   Resize.
     * \param   size    The size.
     * \return  true if it succeeds, false if it fails.
     */
    virtual bool resize(size_t size) {
        if (this->size() == size) {
            return true;
        }
        else if (size <= 0) {
            size_ = 0;
            free();
            return true;
        }

        if (!data_)
            data_ = (char*)malloc(size);
        else
            data_ = (char*)realloc(data_, size);

        size_ = size;

        return data_ != NULL;
    }

    /*!
     * \fn  virtual void DataBuff::attach(IDataBuff *pIBuf)
     * \brief   Attach data of another DataBuff object.
     * \param [in,out]  pIBuf   DataBuff object, its data will be detached.
     */
    virtual void attach(IDataBuff *pIBuf) {
        if (!pIBuf)
            return;

        data_ = pIBuf->detach(size_);
    }

    /*!
     * \fn  virtual void DataBuff::attach(void *pBytes, size_t cchSize)
     * \brief   Attach.
     * \param [in,out]  pBytes  Pointed to the data (raw bytes) to attach. No copying will be done.
     * \param   cchSize         Number of bytes to attach.
     */
    virtual void attach(void *pBytes, size_t cchSize) {
        if (!pBytes)
            return;

        data_ = pBytes;
        size_ = cchSize;
    }

    /*!
     * \fn  virtual void DataBuff::detach(sys::IDataBuff& receiver)
     * \brief   Detaches the given receiver.
     * \param [in,out]  receiver    The receiver.
     */
    virtual void detach(sys::IDataBuff& receiver) {
        receiver.free();
        (dynamic_cast<sys::DataBuff *>(&receiver))->size_ = size_;
        (dynamic_cast<sys::DataBuff *>(&receiver))->data_ = data_;
        data_ = nullptr;
        size_ = 0;
    }


    /*!
     * \fn  virtual void * DataBuff::detach(size_t& cchSize)
     * \brief   Detaches the internal data, and returns them as pointer. Size of the detached data is returned through cchSize.
     * \param [out]  cchSize Size of detached data.
     * \return  Pointer to the detached data.
     */
    virtual void * detach(size_t& cchSize) {
        cchSize = size_;
        size_ = 0;
        void *pDataBuf = data_;
        data_ = nullptr;
        return pDataBuf;
    }

    /*!
     * \fn  virtual void DataBuff::set(void *pBytes, size_t cchSize)
     * \brief   Set.
     * \param [in,out]  pBytes  The bytes.
     * \param   cchSize         Size of the cch.
     */
    virtual void set(void *pBytes, size_t cchSize) {
        this->resize(cchSize);

        if (!pBytes)
            return;

        for (size_t i = 0; i < cchSize; i++)
             *((char *)data_ + i) = *(((char*)pBytes) + i);
    }

    /*!
     * \fn  virtual void DataBuff::set(const std::string& data)
     * \brief   Set the given s data.
     * \param   data   The data to set.
     */
    virtual void set(const std::string& data) {
        if (data.empty())
            this->set(nullptr, 0);

        this->set((void *)data.c_str(), data.size());
    }


    /*!
     * \fn  virtual void DataBuff::append(void *pBytes, size_t cchSize)
     * \brief   Append.
     * \param [in,out]  pBytes  The bytes.
     * \param   cchSize         Number of bytes.
     */
    virtual void append(void *pBytes, size_t cchSize) {
        size_t oldSize = size_;
        this->resize(oldSize + cchSize);

        if (!pBytes)
            return;

        for (size_t i = 0; i < cchSize; i++)
             *((char *)data_ + oldSize + i) = *(((char*)pBytes) + i);
    }


    /*!
     * \fn  virtual void DataBuff::free(void)
     * \brief   Free this object.
     */
    virtual void free(void) {
        if (data_)
            ::free(data_);

        data_ = nullptr;
        size_ = 0;
    }

    /*!
     * \fn  virtual void DataBuff::*data()
     * \brief   Get the data.
     * \return  null if it fails, else a void*.
     */
    virtual void *data() {
        return data_;
    }

    /*!
     * \fn  virtual void DataBuff::getStr(std::string& out_str)
     * \brief                   Puts internal data into out string.
     * \param [in,out]  out_str String buffer.
     */
    virtual void getStr(std::string& out_str) {
        if (!data_)
            return;
        out_str.assign((char *)data_, size_);
    }

    /*!
     * \fn  virtual void DataBuff::getStr(std::wstring& str)
     * \brief                   Puts internal data into std::wstring.
     * \param [in,out]  out_str String buffer.
     */
    virtual void getStr(std::wstring& out_str) {
        if (!data_)
            return;
        size_t nCharCount = size_ / 2;
        out_str.resize(nCharCount + 1);
        out_str[nCharCount] = 0;
        out_str.assign((wchar_t *)data_, nCharCount);
    }

    /*!
     * \fn  virtual void DataBuff::getString()
     * \brief   Copies internal data into std::string and returns it.
     */
    virtual std::string getString() {
        if (!data_)
            return g_pszEmptyString;
        std::string s((char *)data_, size_);
        return std::move(s);
    }

    /*!
     * \fn  virtual size_t DataBuff::size()
     * \brief   Get the size.
     * \return  Size of the data block.
     */
    virtual size_t size() {
        return size_;
    }

    /*!
     * \fn  virtual bool DataBuff::empty()
     * \brief   Indicates whether the buffer is empty.
     * \return  true if buf is empty, false if not empty.
     */
    virtual bool empty() {
        return size_ == 0;
    }

private:
    void       *data_;
    size_t      size_;
};

}

#endif //COMMON__DATABUF_H
