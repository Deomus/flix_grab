//
// Copyright (c) 2014 FreeGrabApp. All rights reserved.
//

#include "precomp.h"
#include "licensing.h"
#include "template.h"
#include "bitstream.h"
#include "base32.h"
#include "base64.h"
#include "sha1.h"
#include "except.h"
#include "uniconv.h"
#include "validator.h"

namespace FreeGrabApp {
    namespace Licensing {
        template<>
        KeyValidatorT<char>::KeyValidatorT():
        m_Impl( *new KeyValidatorImpl() )
        {

        }

        template<>
        KeyValidatorT<wchar_t>::KeyValidatorT():
        m_Impl( *new KeyValidatorImpl() )
        {

        }

        template<>
        KeyValidatorT<char>::KeyValidatorT(const LicenseTemplateT<char> * keyTemplate):
        m_Impl( *new KeyValidatorImpl(&keyTemplate->m_Impl) )
        {

        }

        template<>
        KeyValidatorT<wchar_t>::KeyValidatorT(const LicenseTemplateT<wchar_t> * keyTemplate):
        m_Impl( *new KeyValidatorImpl(&keyTemplate->m_Impl) )
        {

        }

        template<>
        KeyValidatorT<char>::~KeyValidatorT()
        {
            delete & m_Impl;
        }

        template<>
        KeyValidatorT<wchar_t>::~KeyValidatorT()
        {
            delete & m_Impl;
        }

        template<>
        KeyValidatorT<char> * KeyValidatorT<char>::Create()
        {
            return new KeyValidatorT<char>();
        }

        template<>
        KeyValidatorT<wchar_t> * KeyValidatorT<wchar_t>::Create()
        {
            return new KeyValidatorT<wchar_t>();
        }

        template<>
        void KeyValidatorT<char>::Destroy(KeyValidatorT<char> * obj)
        {
            delete obj;
        }

        template<>
        void KeyValidatorT<wchar_t>::Destroy(KeyValidatorT<wchar_t> * obj)
        {
            delete obj;
        }

        template<>
        void KeyValidatorT<char>::SetKeyTemplate(const LicenseTemplateT<char> * templ)
        {
            m_Impl.SetKeyTemplate(&templ->m_Impl);
        }

        template<>
        void KeyValidatorT<wchar_t>::SetKeyTemplate(const LicenseTemplateT<wchar_t> * templ)
        {
            m_Impl.SetKeyTemplate(&templ->m_Impl);
        }

        template<>
        void KeyValidatorT<char>::SetValidationData(const char * fieldName, const void * buf, int len)
        {
            USES_CONVERSION;
            m_Impl.SetValidationData(A2T(fieldName), buf, len);
        }

        template<>
        void KeyValidatorT<wchar_t>::SetValidationData(const wchar_t * fieldName, const void * buf, int len)
        {
        #ifndef _UNICODE
            USES_CONVERSION;
        #endif
            m_Impl.SetValidationData(W2T(fieldName), buf, len);
        }

        template<>
        void KeyValidatorT<char>::SetValidationData(const char * fieldName, const char * data)
        {
            USES_CONVERSION;
            m_Impl.SetValidationData(A2T(fieldName), A2T(data));
        }

        template<>
        void KeyValidatorT<wchar_t>::SetValidationData(const wchar_t * fieldName, const wchar_t * data)
        {
            USES_CONVERSION;
            m_Impl.SetValidationData(W2T(fieldName), W2T(data));
        }

        template<>
        void KeyValidatorT<char>::SetValidationData(const char * fieldName, int data)
        {
            USES_CONVERSION;
            m_Impl.SetValidationData(A2T(fieldName), data);
        }

        template<>
        void KeyValidatorT<wchar_t>::SetValidationData(const wchar_t * fieldName, int data)
        {
            USES_CONVERSION;
            m_Impl.SetValidationData(W2T(fieldName), data);
        }

        template<>
        void KeyValidatorT<char>::SetKey(const char * key)
        {
            USES_CONVERSION;
            m_Impl.SetKey(A2T(key));
        }

        template<>
        void KeyValidatorT<wchar_t>::SetKey(const wchar_t * key)
        {
            USES_CONVERSION;
            m_Impl.SetKey(W2T(key));
        }

        template<>
        bool KeyValidatorT<char>::IsKeyValid()
        {
            return m_Impl.IsKeyValid();
        }

        template<>
        bool KeyValidatorT<wchar_t>::IsKeyValid()
        {
            return m_Impl.IsKeyValid();
        }

        template<>
        void KeyValidatorT<char>::QueryKeyData(const char * dataField, void * buf, int * len)
        {
            USES_CONVERSION;
            m_Impl.QueryKeyData(A2T(dataField), buf, len);
        }

        template<>
        void KeyValidatorT<wchar_t>::QueryKeyData(const wchar_t * dataField, void * buf, int * len)
        {
            USES_CONVERSION;
            m_Impl.QueryKeyData(W2T(dataField), buf, len);
        }

        template<>
        int KeyValidatorT<char>::QueryIntKeyData(const char * dataField)
        {
            USES_CONVERSION;
            return m_Impl.QueryIntKeyData(A2T(dataField));
        }

        template<>
        int KeyValidatorT<wchar_t>::QueryIntKeyData(const wchar_t * dataField)
        {
            USES_CONVERSION;
            return m_Impl.QueryIntKeyData(W2T(dataField));
        }

        template<>
        void KeyValidatorT<char>::QueryDateKeyData(const char * dataField, int * year, int * month, int * day)
        {
            USES_CONVERSION;
            m_Impl.QueryDateKeyData(A2T(dataField), year, month, day);
        }

        template<>
        void KeyValidatorT<wchar_t>::QueryDateKeyData(const wchar_t * dataField, int * year, int * month, int * day)
        {
            USES_CONVERSION;
            m_Impl.QueryDateKeyData(W2T(dataField), year, month, day);
        }

        template<>
        void KeyValidatorT<char>::QueryValidationData(const char * dataField, void * buf, int * len)
        {
            USES_CONVERSION;
            m_Impl.QueryValidationData((dataField) ? A2T(dataField) : NULL, buf, len);
        }

        template<>
        void KeyValidatorT<wchar_t>::QueryValidationData(const wchar_t * dataField, void * buf, int * len)
        {
            USES_CONVERSION;
            m_Impl.QueryValidationData((dataField) ? W2T(dataField) : NULL, buf, len);
        }
    };
};
