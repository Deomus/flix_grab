#pragma once
#include <wrl.h>
#include <memory>
#include <vector>

using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;
using namespace ABI::Windows::Foundation;
using namespace ABI::Windows::Foundation::Collections;

#define _LogInfo(lpsz) \
{ \
	wchar_t temp[1024]; \
	swprintf(temp, 1024, L"INFO - %s %s %ld\n", lpsz, __FILEW__, __LINE__); \
	::OutputDebugStringW(temp); \
} \

#define _LogError(lpsz) \
{ \
	wchar_t temp[1024]; \
	swprintf(temp, 1024, L"ERROR - %s %s %ld\n", lpsz, __FILEW__, __LINE__); \
	::OutputDebugStringW(temp); \
} \

#define _EXCEPTION_HANDLER(str)			\
} catch (...) {							\
	_LogError(str);						\
	return E_FAIL;						\
}


    class HStringHelper
    {
    public:
        HStringHelper(HSTRING h) : hstr_(nullptr)
        {
            HRESULT hr = Set(h);
            // Error handling
        }

        HStringHelper(const wchar_t* str)
        {
            unsigned int len;
            len = ::wcslen(str);
            Release();
            ::WindowsCreateString(str, len, &hstr_);
        }

        HStringHelper() : hstr_(nullptr)
        {
        }

        HStringHelper(const HStringHelper& h) : hstr_(nullptr)
        {
            HRESULT hr = Set(h.Get());
            // Error handling
        }

        HStringHelper(HStringHelper&& h) : hstr_(nullptr)
        {
            hstr_ = h.hstr_;
            h.hstr_ = nullptr;
        }

        HStringHelper& operator=(const HStringHelper& h)
        {
            Release();
            hstr_ = h.hstr_;
            return *this;
        }

        HStringHelper& operator=(HStringHelper&& h)
        {
            Release();
            hstr_ = h.hstr_;
            h.hstr_ = nullptr;
            return *this;
        }

        ~HStringHelper()
        {
        }

        // Initialize this string from an HSTRING. A copy is made in this call.
        HRESULT Set(const HSTRING& str) throw()
        {
            Release();
            return ::WindowsDuplicateString(str, &hstr_);
        }

        HSTRING Get() const throw()
        {
            return hstr_;
        }

        const wchar_t* GetRawBuffer(unsigned int* length) const
        {
            return ::WindowsGetStringRawBuffer(hstr_, length);
        }

        void Release() throw()
        {
            ::WindowsDeleteString(hstr_);
            hstr_ = nullptr;
        }

        HRESULT CopyTo(_Outptr_result_maybenull_ _Result_nullonfailure_ HSTRING *str) const throw()
        {
            return ::WindowsDuplicateString(hstr_, str);
        }

    private:
        HSTRING hstr_;
    };

    template <typename T>
    struct Wrap
    {
        typename typedef T type;
        static const T& MakeWrap(const T& t)
        {
            return t;
        }
        static const T& Unwrap(const T& t)
        {
            return t;
        }
    };

    template <>
    struct Wrap<HSTRING>
    {
        typedef HStringHelper type;
        static HStringHelper MakeWrap(const HSTRING& t)
        {
            HStringHelper str;
            str.Set(t);
            return str;
        }
        static HSTRING Unwrap(const HStringHelper& t)
        {
            HSTRING str;
            /*HRESULT hr =*/ t.CopyTo(&str);
            return str;
        }
    };

    //
    // Root.obj : error LNK2019: unresolved external symbol "private: static void __cdecl Microsoft::WRL::Wrappers::HString::operator delete(void *,unsigned int)" (??3HString@Wrappers@WRL@Microsoft@@CAXPAXI@Z) referenced in function "public: void * __thiscall Microsoft::WRL::Wrappers::HString::`scalar deleting destructor'(unsigned int)" (??_GHString@Wrappers@WRL@Microsoft@@QAEPAXI@Z)
    //
#ifdef COMPILATION_ERROR
    template <>
    struct Wrap<HSTRING>
    {
        typedef HString type;
        static HString MakeWrap(const HSTRING& t)
        {
            HString str;
            str.Set(t);
            return str;
        }
        static HSTRING Unwrap(const HString& t)
        {
            HSTRING str;
            HRESULT hr = t.CopyTo(&str);
            return str;
        }
    };
#endif

    template <typename T>
    struct Wrap<T*>
    {
        typename typedef ComPtr<T> type;
        static ComPtr<T> MakeWrap(T* t)
        {
            ComPtr<T> ptr;
            ptr.Attach(t);
            return ptr;
        }
        static T* Unwrap(ComPtr<T> t)
        {
            ComPtr<T> ptr(t);
            return t.Detach();
        }
    };

        template <class T>
        class Iterator : public RuntimeClass<IIterator<T>>
        {
            //InspectableClass(L"Vector.Iterator", BaseTrust)

        private:
            typedef typename std::vector<typename Wrap<T>::type> WrappedVector;
            typedef typename std::shared_ptr<WrappedVector> V;
            typedef typename WrappedVector::iterator IT;

        public:
            Iterator() {}

            ~Iterator() {}

            void LogVectorContent()
            {
                for (IT it = _v->begin(); it != _v->end(); it++)
                {
                    T element = Wrap<T>::Unwrap(*it);
                    void * ptr = (void *)element;
                    wchar_t sz[255];
                    swprintf(sz, 255, L"adress T =0x%08x\n", (int)ptr);
                    _LogInfo(sz);
                }
            }

            HRESULT Init(const V& vec)
            {
                try {
                    _LogInfo(L"Iterator::Init()...");

                    _v = vec;

                    LogVectorContent();

                    _it = _v->begin();
                    if (_it != _v->end())
                    {
                        _element = Wrap<T>::Unwrap(*_it);
                        _bElement = TRUE;
                    }
                    else
                        _bElement = FALSE;
                    return S_OK;
                _EXCEPTION_HANDLER(L"Iterator::Init()...");
            }

        public:
            virtual HRESULT STDMETHODCALLTYPE get_Current(T *current)
            {
                try {
                    _LogInfo(L"Iterator::get_Current()...");
                    if (_it != _v->end())
                    {
                        _bElement = TRUE;
                        _element = Wrap<T>::Unwrap(*_it);
                        *current = _element;
                    }
                    else
                    {
                        _bElement = FALSE;
                    }
                    return S_OK;
                    _EXCEPTION_HANDLER(L"Iterator::get_Current()...");
            }

            virtual HRESULT STDMETHODCALLTYPE get_HasCurrent(boolean *hasCurrent)
            {
                try {
                    _LogInfo(L"Iterator::get_HasCurrent()...");
                    if (_bElement)
                        *hasCurrent = TRUE;
                    else
                        *hasCurrent = FALSE;
                    return S_OK;
                _EXCEPTION_HANDLER(L"Iterator::get_HasCurrent()...");
            }

            virtual HRESULT STDMETHODCALLTYPE MoveNext(boolean *hasCurrent)
            {
                try {
                    _LogInfo(L"Iterator::MoveNext()...");
                    if (_it != _v->end())
                    {
                        ++_it;
                        if (_it == _v->end())
                        {
                            *hasCurrent = FALSE;
                            _bElement = FALSE;
                        }
                        else
                        {
                            *hasCurrent = TRUE;
                            _bElement = TRUE;
                        }
                    }
                    else
                    {
                        *hasCurrent = FALSE;
                        _bElement = FALSE;
                    }
                    return S_OK;
                _EXCEPTION_HANDLER(L"Iterator::MoveNext()...");
            }

        private:
            V _v;
            IT _it;
            T _element;
            boolean _bElement = FALSE;
        };

        class VectorChangedEventArgs : public RuntimeClass<IVectorChangedEventArgs>
        {
            InspectableClass(L"Platform.Collections.VectorChangedEventArgs", BaseTrust)

        public:
            VectorChangedEventArgs(CollectionChange change, unsigned int index) : m_index(index)
            {
                m_change = change;
            }

        public:
            virtual HRESULT STDMETHODCALLTYPE get_CollectionChange(CollectionChange *value) {
                *value = m_change;
                return S_OK;
            }

            virtual HRESULT STDMETHODCALLTYPE get_Index(unsigned int *value) {
                if (m_change == CollectionChange::CollectionChange_Reset)
                {
                    return E_FAIL;
                }

                *value = m_index;
                return S_OK;
            }

        private:
            CollectionChange m_change;
            unsigned int m_index;
        };

        template <typename T>
        class Vector : public RuntimeClass</*IVector<T>,*/
            IIterable<T>>//,
            //IObservableVector<T>>
        {
            //InspectableClass(L"Vector", BaseTrust)

        private:
            typedef typename std::vector<typename Wrap<T>::type> WrappedVector;
            typedef typename WrappedVector::const_iterator CIT;
            typedef typename VectorChangedEventHandler<T> WFC_Handler;

        public:
            Vector()
            {
                _LogInfo(L"Vector::Vector()...");
                _v = std::make_shared<WrappedVector>();
                //m_observed = false;
            }

            ~Vector() {}

        public:
            // read methods
            virtual HRESULT STDMETHODCALLTYPE GetAt(unsigned index, T *item)
            {
                _LogInfo(L"Vector::GetAt()...");
                *item = Wrap<T>::Unwrap((*_v)[index]);
                return S_OK;
            }

            virtual HRESULT STDMETHODCALLTYPE get_Size(unsigned *size)
            {
                _LogInfo(L"Vector::get_Size()...");
                *size = _v->size();
                return S_OK;
            }

            virtual HRESULT STDMETHODCALLTYPE GetView(IVectorView<T> **view)
            {
                _LogInfo(L"Vector::GetView()...");
                return E_NOTIMPL;
            }

            virtual HRESULT STDMETHODCALLTYPE IndexOf(T value, unsigned *index, boolean *found)
            {
                _LogInfo(L"Vector::IndexOf()...");
                unsigned foundedIndex = 0;
                bool founded = false;
                for (CIT it = _v->cbegin(); it != _v->end(); ++it)
                {
                    const T & v = Wrap<T>::Unwrap(*it);
                    if (v == value)
                    {
                        founded = true;
                        break;
                    }
                    foundedIndex++;
                }
                if (founded == true)
                {
                    *found = TRUE;
                    *index = foundedIndex;
                }
                else
                {
                    *found = FALSE;
                }
                return S_OK;
            }

            // write methods
            virtual HRESULT STDMETHODCALLTYPE SetAt(unsigned index, T item)
            {
                _LogInfo(L"Vector::SetAt()...");
                if (index < _v->size())
                {
                    (*_v)[index] = Wrap<T>::MakeWrap(item);
                    //NotifyChanged(index);
                }
                return S_OK;
            }

            virtual HRESULT STDMETHODCALLTYPE InsertAt(unsigned index, T item)
            {
                _LogInfo(L"Vector::InsertAt()...");
                _v->insert(_v->begin() + index, Wrap<T>::MakeWrap(item));
               // NotifyInserted(index);
                return S_OK;
            }

            virtual HRESULT STDMETHODCALLTYPE RemoveAt(unsigned index)
            {
                _LogInfo(L"Vector::RemoveAt()...");
                if (index < _v->size())
                {
                    _v->erase(_v->begin() + index);
                    //NotifyRemoved(index);
                }
                return S_OK;
            }

            virtual HRESULT STDMETHODCALLTYPE Append(T item)
            {
                _LogInfo(L"Vector::Append()...");
                size_t n = _v->size();
                _v->push_back(Wrap<T>::MakeWrap(item));
                //NotifyInserted(static_cast<unsigned int>(n));
                return S_OK;
            }

            virtual HRESULT STDMETHODCALLTYPE RemoveAtEnd()
            {
                _LogInfo(L"Vector::RemoveAtEnd()...");
                if (!_v->empty())
                {
                    _v->pop_back();
                    //NotifyRemoved(static_cast<unsigned int>(_v->size()));
                }
                return S_OK;
            }

            virtual HRESULT STDMETHODCALLTYPE Clear()
            {
                _LogInfo(L"Vector::Clear()...");
                _v->clear();
                //NotifyReset();
                return S_OK;
            }

            virtual HRESULT STDMETHODCALLTYPE First(IIterator<T> **first)
            {
                _LogInfo(L"Vector::First()...");
                ComPtr<Iterator<T>> p = Make<Iterator<T>>();
                p->Init(_v);
                *first = p.Detach();
                return S_OK;
            }

          

        private:
            std::shared_ptr<WrappedVector> _v;
            //bool m_observed;
            //EventSource<VectorChangedEventHandler<T>> m_events;
        };
            

