//
// Copyright (c) 2014 FreeGrabApp. All rights reserved.
//

#include "precomp.h"
#include "licensing.h"
#include "template.h"
#include "base32.h"
#include "base64.h"
#include "sha1.h"
#include "except.h"
#include "bitstream.h"
#include "uniconv.h"
#include "generator.h"

namespace FreeGrabApp {
    namespace Licensing {
        template<>
        KeyGeneratorT<char>::KeyGeneratorT():
        m_Impl( *new KeyGeneratorImpl())
        {

        }

        template<>
        KeyGeneratorT<wchar_t>::KeyGeneratorT():
        m_Impl( *new KeyGeneratorImpl())
        {

        }

        template<>
        KeyGeneratorT<char>::KeyGeneratorT(const LicenseTemplateT<char> * templ):
        m_Impl( *new KeyGeneratorImpl(&templ->m_Impl) )
        {

        }

        template<>
        KeyGeneratorT<wchar_t>::KeyGeneratorT(const LicenseTemplateT<wchar_t> * templ):
        m_Impl( *new KeyGeneratorImpl(&templ->m_Impl) )
        {

        }

        template<>
        KeyGeneratorT<char>::~KeyGeneratorT()
        {
            delete & m_Impl;
        }

        template<>
        KeyGeneratorT<wchar_t>::~KeyGeneratorT()
        {
            delete & m_Impl;
        }

        template<>
        KeyGeneratorT<char> * KeyGeneratorT<char>::Create()
        {
            return new KeyGeneratorT<char>();
        }

        template<>
        KeyGeneratorT<wchar_t> * KeyGeneratorT<wchar_t>::Create()
        {
            return new KeyGeneratorT<wchar_t>();
        }

        template<>
        void KeyGeneratorT<char>::Destroy(KeyGeneratorT<char> * obj)
        {
            delete obj;
        }

        template<>
        void KeyGeneratorT<wchar_t>::Destroy(KeyGeneratorT<wchar_t> * obj)
        {
            delete obj;
        }

        template<>
        void KeyGeneratorT<char>::SetKeyData(const char * fieldName, const void * buf, int len)
        {
            USES_CONVERSION;
            m_Impl.KeyGeneratorImpl::SetKeyData(A2T(fieldName), buf, len);
        }

        template<>
        void KeyGeneratorT<wchar_t>::SetKeyData(const wchar_t * fieldName, const void * buf, int len)
        {
            USES_CONVERSION;
            m_Impl.KeyGeneratorImpl::SetKeyData(W2T(fieldName), buf, len);
        }

        template<>
        void KeyGeneratorT<char>::SetKeyData(const char * fieldName, int data)
        {
            USES_CONVERSION;
            m_Impl.SetKeyData(A2T(fieldName), data);
        }

        template<>
        void KeyGeneratorT<wchar_t>::SetKeyData(const wchar_t * fieldName, int data)
        {
            USES_CONVERSION;
            m_Impl.SetKeyData(W2T(fieldName), data);
        }

        template<>
        void KeyGeneratorT<char>::SetKeyData(const char * fieldName, const char * data)
        {
            USES_CONVERSION;

            m_Impl.SetKeyData(A2T(fieldName), A2T(data));
        }

        template<>
        void KeyGeneratorT<wchar_t>::SetKeyData(const wchar_t * fieldName, const wchar_t * data)
        {
            USES_CONVERSION;

            m_Impl.SetKeyData(W2T(fieldName), W2T(data));
        }

        template<>
        void KeyGeneratorT<char>::SetKeyData(const char * fieldName, int year, int month, int day)
        {
            USES_CONVERSION;
            m_Impl.SetKeyData(A2T(fieldName), year, month, day);
        }

        template<>
        void KeyGeneratorT<wchar_t>::SetKeyData(const wchar_t * fieldName, int year, int month, int day)
        {
            USES_CONVERSION;
            m_Impl.SetKeyData(W2T(fieldName), year, month, day);
        }

        template<>
        void KeyGeneratorT<char>::SetValidationData(const char * fieldName, const void * buf, int len)
        {
            USES_CONVERSION;
            m_Impl.SetValidationData(A2T(fieldName), buf, len);
        }

        template<>
        void KeyGeneratorT<wchar_t>::SetValidationData(const wchar_t * fieldName, const void * buf, int len)
        {
            USES_CONVERSION;
            m_Impl.SetValidationData(W2T(fieldName), buf, len);
        }

        template<>
        void KeyGeneratorT<char>::SetValidationData(const char * fieldName, int data)
        {
            USES_CONVERSION;
            m_Impl.SetValidationData(A2T(fieldName), data);
        }

        template<>
        void KeyGeneratorT<wchar_t>::SetValidationData(const wchar_t * fieldName, int data)
        {
            USES_CONVERSION;
            m_Impl.SetValidationData(W2T(fieldName), data);
        }

        template<>
        void KeyGeneratorT<char>::SetValidationData(const char * fieldName, const char * data)
        {
            USES_CONVERSION;
            m_Impl.SetValidationData(A2T(fieldName), A2T(data));
        }

        template<>
        void KeyGeneratorT<wchar_t>::SetValidationData(const wchar_t * fieldName, const wchar_t * data)
        {
            USES_CONVERSION;
            m_Impl.SetValidationData(W2T(fieldName), W2T(data));
        }

        template<>
        void KeyGeneratorT<char>::SetKeyTemplate(const LicenseTemplateT<char> & templ)
        {
            m_Impl.SetKeyTemplate(&templ.m_Impl);
        }

        template<>
        void KeyGeneratorT<wchar_t>::SetKeyTemplate(const LicenseTemplateT<wchar_t> & templ)
        {
            m_Impl.SetKeyTemplate(&templ.m_Impl);
        }

        template<>
        void KeyGeneratorT<char>::SetKeyTemplate(const LicenseTemplateT<char> * templ)
        {
            m_Impl.SetKeyTemplate(&templ->m_Impl);
        }

        template<>
        void KeyGeneratorT<wchar_t>::SetKeyTemplate(const LicenseTemplateT<wchar_t> * templ)
        {
            m_Impl.SetKeyTemplate(&templ->m_Impl);
        }

        template<>
        const char * KeyGeneratorT<char>::GenerateKey()
        {
        #ifdef _UNICODE
            USES_CONVERSION;
            static std::string key;
            key = W2A(m_Impl.GenerateKey());
            return key.c_str();
        #else
            return m_Impl.GenerateKey();
        #endif
        }

        template<>
        const wchar_t * KeyGeneratorT<wchar_t>::GenerateKey()
        {
        #ifdef _UNICODE
            return m_Impl.GenerateKey();
        #else
            USES_CONVERSION;
            static std::wstring key;
            key = A2W(m_Impl.GenerateKey());
            return key.c_str();
        #endif
        }
    };
};