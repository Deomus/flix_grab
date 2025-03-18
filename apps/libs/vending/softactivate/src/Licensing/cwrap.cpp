//
// Copyright (c) 2014 FreeGrabApp. All rights reserved.
//

#include "precomp.h"

#include "base32.h"
#include "base64.h"
#include "sha1.h"
#include "except.h"
#include "bitstream.h"
#include "template.h"
#include "generator.h"
#include "validator.h"
#include "certificate.h"
#include "uniconv.h"

void * KeyGenerator_Create()
{
    return new(std::nothrow) KeyGeneratorImpl();
}

void KeyGenerator_Destroy(void * generator)
{
    delete static_cast<KeyGeneratorImpl *>(generator);
}

int KeyGenerator_SetKeyTemplate(void * generator, const void * tmpl)
{
    try {
        static_cast<KeyGeneratorImpl *>(generator)->SetKeyTemplate(static_cast<LicenseTemplateImpl *>(const_cast<void *>(tmpl)));
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int KeyGenerator_SetKeyDataA(void * generator, const char * fieldName, const void * data, int len)
{
    USES_CONVERSION;

    try {
        static_cast<KeyGeneratorImpl *>(generator)->SetKeyData(A2T(fieldName), data, len);
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int KeyGenerator_SetKeyDataW(void * generator, const wchar_t * fieldName, const void * data, int len)
{
    USES_CONVERSION;

    try {
        static_cast<KeyGeneratorImpl *>(generator)->SetKeyData(W2T(fieldName), data, len);
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int KeyGenerator_SetIntKeyDataA(void * generator, const char * fieldName, int data)
{
    USES_CONVERSION;

    try {
        static_cast<KeyGeneratorImpl *>(generator)->SetKeyData(A2T(fieldName), data);
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int KeyGenerator_SetIntKeyDataW(void * generator, const wchar_t * fieldName, int data)
{
    USES_CONVERSION;

    try {
        static_cast<KeyGeneratorImpl *>(generator)->SetKeyData(W2T(fieldName), data);
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int KeyGenerator_SetDateKeyDataA(void * generator, const char * fieldName, int year, int month, int day)
{
    USES_CONVERSION;

    try {
        static_cast<KeyGeneratorImpl *>(generator)->SetKeyData(A2T(fieldName), year, month, day);
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int KeyGenerator_SetDateKeyDataW(void * generator, const wchar_t * fieldName, int year, int month, int day)
{
    USES_CONVERSION;

    try {
        static_cast<KeyGeneratorImpl *>(generator)->SetKeyData(W2T(fieldName), year, month, day);
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int KeyGenerator_SetStringKeyDataA(void * generator, const char * fieldName, const char * data)
{
    USES_CONVERSION;

    try {
        static_cast<KeyGeneratorImpl *>(generator)->SetKeyData(A2T(fieldName), A2T(data));
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int KeyGenerator_SetStringKeyDataW(void * generator, const wchar_t * fieldName, const wchar_t * data)
{
    USES_CONVERSION;

    try {
        static_cast<KeyGeneratorImpl *>(generator)->SetKeyData(W2T(fieldName), W2T(data));
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int KeyGenerator_SetValidationDataA(void * generator, const char * fieldName, const void * buf, int len)
{
    USES_CONVERSION;

    try {
        static_cast<KeyGeneratorImpl *>(generator)->SetValidationData(A2T(fieldName), buf, len);
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int KeyGenerator_SetValidationDataW(void * generator, const wchar_t * fieldName, const void * buf, int len)
{
    USES_CONVERSION;

    try {
        static_cast<KeyGeneratorImpl *>(generator)->SetValidationData(W2T(fieldName), buf, len);
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int KeyGenerator_SetIntValidationDataA(void * generator, const char * fieldName, int data)
{
    USES_CONVERSION;

    try {
        static_cast<KeyGeneratorImpl *>(generator)->SetValidationData(A2T(fieldName), data);
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int KeyGenerator_SetIntValidationDataW(void * generator, const wchar_t * fieldName, int data)
{
    USES_CONVERSION;

    try {
        static_cast<KeyGeneratorImpl *>(generator)->SetValidationData(W2T(fieldName), data);
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int KeyGenerator_SetStringValidationDataA(void * generator, const char * fieldName, const char * data)
{
    USES_CONVERSION;
    USES_CONVERSION_2;

    try {
        static_cast<KeyGeneratorImpl *>(generator)->SetValidationData(A2T(fieldName), A2T_2(data));
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int KeyGenerator_SetStringValidationDataW(void * generator, const wchar_t * fieldName, const wchar_t * data)
{
    USES_CONVERSION;

    try {
        static_cast<KeyGeneratorImpl *>(generator)->SetValidationData(W2T(fieldName), W2T_2(data));
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int KeyGenerator_GenerateKeyA(void * generator, const char **key)
{
    try {
#ifdef _UNICODE
        USES_CONVERSION;
        static string keyA;
        
        keyA = W2A(static_cast<KeyGeneratorImpl *>(generator)->GenerateKey());

        *key = keyA.c_str();
#else
        *key = static_cast<KeyGeneratorImpl *>(generator)->GenerateKey();
#endif
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int KeyGenerator_GenerateKeyW(void * generator, const wchar_t **key)
{
    try {
#ifdef _UNICODE
        *key = static_cast<KeyGeneratorImpl *>(generator)->GenerateKey();
#else
        USES_CONVERSION;
        static wstring keyW;
        
        keyW = A2W(static_cast<KeyGeneratorImpl *>(generator)->GenerateKey());

        *key = keyW.c_str();
#endif
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

void * KeyValidator_Create()
{
    return new(std::nothrow) KeyValidatorImpl();
}

void KeyValidator_Destroy(void * validator)
{
    delete static_cast<KeyValidatorImpl *>(validator);
}

int KeyValidator_SetKeyTemplate(void * validator, const void * tmpl)
{
    try {
        static_cast<KeyValidatorImpl *>(validator)->SetKeyTemplate(static_cast<LicenseTemplateImpl *>(const_cast<void *>(tmpl)));
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int KeyValidator_SetKeyA(void * validator, const char * key)
{
    USES_CONVERSION;
    try {
        static_cast<KeyValidatorImpl *>(validator)->SetKey(A2T(key));
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int KeyValidator_SetKeyW(void * validator, const wchar_t * key)
{
    USES_CONVERSION;

    try {
        static_cast<KeyValidatorImpl *>(validator)->SetKey(W2T(key));
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int KeyValidator_SetValidationDataA(void * validator, const char * fieldName, const void * buf, int len)
{
    USES_CONVERSION;
    try {
        static_cast<KeyValidatorImpl *>(validator)->SetValidationData(A2T(fieldName), buf, len);
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int KeyValidator_SetValidationDataW(void * validator, const wchar_t * fieldName, const void * buf, int len)
{
    USES_CONVERSION;

    try {
        static_cast<KeyValidatorImpl *>(validator)->SetValidationData(W2T(fieldName), buf, len);
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int KeyValidator_SetIntValidationDataA(void * validator, const char * fieldName, int data)
{
    USES_CONVERSION;

    try {
        static_cast<KeyValidatorImpl *>(validator)->SetValidationData(A2T(fieldName), data); 
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int KeyValidator_SetIntValidationDataW(void * validator, const wchar_t * fieldName, int data)
{
    USES_CONVERSION;

    try {
        static_cast<KeyValidatorImpl *>(validator)->SetValidationData(W2T(fieldName), data); 
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int KeyValidator_SetStringValidationDataA(void * validator, const char * fieldName, const char * data)
{
    USES_CONVERSION;

    try {
        static_cast<KeyValidatorImpl *>(validator)->SetValidationData(A2T(fieldName), A2T(data));
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int KeyValidator_SetStringValidationDataW(void * validator, const wchar_t * fieldName, const wchar_t * data)
{
    USES_CONVERSION;

    try {
        static_cast<KeyValidatorImpl *>(validator)->SetValidationData(W2T(fieldName), W2T(data));
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int KeyValidator_IsKeyValid(void * validator)
{
    int status = STATUS_SUCCESS;

    try {
        if (!static_cast<KeyValidatorImpl *>(validator)->IsKeyValid())
            status = STATUS_INVALID_LICENSE_KEY;
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return status;
}

int KeyValidator_QueryKeyDataA(void * validator, const char * dataField, void * buf, int * len)
{
    USES_CONVERSION;

    try {
        static_cast<KeyValidatorImpl *>(validator)->QueryKeyData(A2T(dataField), buf, len);
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int KeyValidator_QueryKeyDataW(void * validator, const wchar_t * dataField, void * buf, int * len)
{
    USES_CONVERSION;

    try {
        static_cast<KeyValidatorImpl *>(validator)->QueryKeyData(W2T(dataField), buf, len);
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int KeyValidator_QueryIntKeyDataA(void * validator, const char * dataField, int * data)
{
    USES_CONVERSION;

    try {
        *data = static_cast<KeyValidatorImpl *>(validator)->QueryIntKeyData(A2T(dataField));
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int KeyValidator_QueryIntKeyDataW(void * validator, const wchar_t * dataField, int * data)
{
    USES_CONVERSION;

    try {
        *data = static_cast<KeyValidatorImpl *>(validator)->QueryIntKeyData(W2T(dataField));
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int KeyValidator_QueryDateKeyDataA(void * validator, const char * dataField, int * year, int * month, int * day)
{
    USES_CONVERSION;

    try {
        static_cast<KeyValidatorImpl *>(validator)->QueryDateKeyData(A2T(dataField), year, month, day);
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int KeyValidator_QueryDateKeyDataW(void * validator, const wchar_t * dataField, int * year, int * month, int * day)
{
    USES_CONVERSION;

    try {
        static_cast<KeyValidatorImpl *>(validator)->QueryDateKeyData(W2T(dataField), year, month, day);
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int KeyValidator_QueryValidationDataA(void * validator, const char * dataField, void * buf, int * len)
{
    USES_CONVERSION;

    try {
        static_cast<KeyValidatorImpl *>(validator)->QueryValidationData(A2T(dataField), buf, len);
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int KeyValidator_QueryValidationDataW(void * validator, const wchar_t * dataField, void * buf, int * len)
{
    USES_CONVERSION;

    try {
        static_cast<KeyValidatorImpl *>(validator)->QueryValidationData(W2T(dataField), buf, len);
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

void * LicenseTemplate_Create()
{
    return new(std::nothrow) LicenseTemplateImpl();
}

void LicenseTemplate_Destroy(void * tmpl)
{
    delete static_cast<LicenseTemplateImpl *>(tmpl);
}

int LicenseTemplate_SetVersion(void * tmpl, int version)
{
    try {
        static_cast<LicenseTemplateImpl *>(tmpl)->SetVersion(version);
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int LicenseTemplate_GetVersion(void * tmpl, int * version)
{
    try {
        static_cast<LicenseTemplateImpl *>(tmpl)->GetVersion();
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int LicenseTemplate_SetNumberOfGroups(void * tmpl, int numGroups)
{
    try {
        static_cast<LicenseTemplateImpl *>(tmpl)->SetNumberOfGroups(numGroups);
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int LicenseTemplate_GetNumberOfGroups(void * tmpl, int * numGroups)
{
    try {
        *numGroups = static_cast<LicenseTemplateImpl *>(tmpl)->GetNumberOfGroups();
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int LicenseTemplate_SetCharactersPerGroup(void * tmpl, int charsPerGroup)
{
    try {
        static_cast<LicenseTemplateImpl *>(tmpl)->SetCharactersPerGroup(charsPerGroup);
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int LicenseTemplate_GetCharactersPerGroup(void * tmpl, int * charsPerGroup)
{
    try {
        *charsPerGroup = static_cast<LicenseTemplateImpl *>(tmpl)->GetCharactersPerGroup();
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int LicenseTemplate_SetGroupSeparatorA(void * tmpl, const char * groupSep)
{
    USES_CONVERSION;

    try {
        static_cast<LicenseTemplateImpl *>(tmpl)->SetGroupSeparator(A2T(groupSep));
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int LicenseTemplate_SetGroupSeparatorW(void * tmpl, const wchar_t * groupSep)
{
    USES_CONVERSION;

    try {
        static_cast<LicenseTemplateImpl *>(tmpl)->SetGroupSeparator(W2T(groupSep));
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int LicenseTemplate_GetGroupSeparatorA(void * tmpl, const char **groupSep)
{
    try {
#ifdef _UNICODE
        *groupSep = NULL;
#else
        *groupSep = static_cast<LicenseTemplateImpl *>(tmpl)->GetGroupSeparator();
#endif
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int LicenseTemplate_GetGroupSeparatorW(void * tmpl, const wchar_t **groupSep)
{
    try {
#ifdef _UNICODE
        *groupSep = static_cast<LicenseTemplateImpl *>(tmpl)->GetGroupSeparator();
#else
        *groupSep = NULL;
#endif
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int LicenseTemplate_SetEncoding(void * tmpl, int encoding)
{
    try {
        static_cast<LicenseTemplateImpl *>(tmpl)->SetEncoding(encoding);
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int LicenseTemplate_GetEncoding(void * tmpl, int * encoding)
{
    try {
        *encoding = static_cast<LicenseTemplateImpl *>(tmpl)->GetEncoding();
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int LicenseTemplate_SetHeaderA(void * tmpl, const char * header)
{
    USES_CONVERSION;

    try {
        static_cast<LicenseTemplateImpl *>(tmpl)->SetHeader(A2T(header));
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int LicenseTemplate_SetHeaderW(void * tmpl, const wchar_t * header)
{
    USES_CONVERSION;

    try {
        static_cast<LicenseTemplateImpl *>(tmpl)->SetHeader(W2T(header));
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int LicenseTemplate_GetHeaderA(void * tmpl, const char ** header)
{
    try {
#ifdef _UNICODE
        *header = NULL;
#else
        *header = static_cast<LicenseTemplateImpl *>(tmpl)->GetHeader();
#endif
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int LicenseTemplate_GetHeaderW(void * tmpl, const wchar_t ** header)
{
    try {
#ifdef _UNICODE
        *header = static_cast<LicenseTemplateImpl *>(tmpl)->GetHeader();
#else
        *header = NULL;
#endif
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int LicenseTemplate_SetFooterA(void * tmpl, const char * footer)
{
    USES_CONVERSION;

    try {
        static_cast<LicenseTemplateImpl *>(tmpl)->SetFooter(A2T(footer));
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int LicenseTemplate_SetFooterW(void * tmpl, const wchar_t * footer)
{
    USES_CONVERSION;

    try {
        static_cast<LicenseTemplateImpl *>(tmpl)->SetFooter(W2T(footer));
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int LicenseTemplate_GetFooterA(void * tmpl, const char **footer)
{
    try {
#ifdef _UNICODE
        *footer = NULL;
#else
        *footer = static_cast<LicenseTemplateImpl *>(tmpl)->GetFooter();
#endif
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int LicenseTemplate_GetFooterW(void * tmpl, const wchar_t **footer)
{
    try {
#ifdef _UNICODE
        *footer = static_cast<LicenseTemplateImpl *>(tmpl)->GetFooter();
#else
        *footer = NULL;
#endif
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int LicenseTemplate_SetDataSize(void * tmpl, int sizeInBits)
{
    try {
        static_cast<LicenseTemplateImpl *>(tmpl)->SetDataSize(sizeInBits);
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int LicenseTemplate_GetDataSize(void * tmpl, int * sizeInBits)
{
    try {
        *sizeInBits = static_cast<LicenseTemplateImpl *>(tmpl)->GetDataSize();
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int LicenseTemplate_AddDataFieldA(void * tmpl, const char * fieldName, int fieldType, int fieldBitSize, int offset)
{
    USES_CONVERSION;
    try {
        static_cast<LicenseTemplateImpl *>(tmpl)->AddDataField(A2T(fieldName), fieldType, fieldBitSize, offset);
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int LicenseTemplate_AddDataFieldW(void * tmpl, const wchar_t * fieldName, int fieldType, int fieldBitSize, int fieldOffset)
{
    USES_CONVERSION;
    try {
        static_cast<LicenseTemplateImpl *>(tmpl)->AddDataField(W2T(fieldName), fieldType, fieldBitSize, fieldOffset);
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}


int LicenseTemplate_GetDataFieldA(void *tmpl, const char *fieldName, int * fieldType, int *fieldSize, int *fieldOffset)
{
    USES_CONVERSION;

    if (static_cast<LicenseTemplateImpl *>(tmpl)->GetDataField(A2T(fieldName), fieldType, fieldSize, fieldOffset))
        return STATUS_SUCCESS;

    return STATUS_GENERIC_ERROR;
}

int LicenseTemplate_GetDataFieldW(void *tmpl, const wchar_t *fieldName, int * fieldType, int *fieldSize, int *fieldOffset)
{
    USES_CONVERSION;

    if (static_cast<LicenseTemplateImpl *>(tmpl)->GetDataField(W2T(fieldName), fieldType, fieldSize, fieldOffset))
        return STATUS_SUCCESS;

    return STATUS_GENERIC_ERROR;
}

int LicenseTemplate_EnumDataFieldsA(void * tmpl, void **enumHandle, const char **fieldName, int * fieldType, int * fieldSize, int * fieldOffset)
{
    int status = STATUS_SUCCESS;

    try {
#ifdef _UNICODE
        status = STATUS_GENERIC_ERROR;
#else
        if (!static_cast<LicenseTemplateImpl *>(tmpl)->EnumDataFields(enumHandle, fieldName, fieldType, fieldSize, fieldOffset))
            status = STATUS_GENERIC_ERROR;
#endif
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return status;
}

int LicenseTemplate_EnumDataFieldsW(void * tmpl, void **enumHandle, const wchar_t **fieldName, int * fieldType, int * fieldSize, int * fieldOffset)
{
    int status = STATUS_SUCCESS;

    try {
#ifdef _UNICODE
        if (!static_cast<LicenseTemplateImpl *>(tmpl)->EnumDataFields(enumHandle, fieldName, fieldType, fieldSize, fieldOffset))
            status = STATUS_GENERIC_ERROR;
#else
        status = STATUS_GENERIC_ERROR;
#endif
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return status;
}

int LicenseTemplate_SetValidationDataSize(void * tmpl, int sizeInBits)
{
    try {
        static_cast<LicenseTemplateImpl *>(tmpl)->SetValidationDataSize(sizeInBits);
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int LicenseTemplate_GetValidationDataSize(void * tmpl, int * sizeInBits)
{
    try {
        *sizeInBits = static_cast<LicenseTemplateImpl *>(tmpl)->GetValidationDataSize();
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int LicenseTemplate_AddValidationFieldA(void * tmpl, const char * fieldName, int fieldType, int fieldBitSize, int offset)
{
    USES_CONVERSION;
    try {
        static_cast<LicenseTemplateImpl *>(tmpl)->AddValidationField(A2T(fieldName), fieldType, fieldBitSize, offset);
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int LicenseTemplate_AddValidationFieldW(void * tmpl, const wchar_t * fieldName, int fieldType, int fieldBitSize, int offset)
{
    USES_CONVERSION;
    try {
        static_cast<LicenseTemplateImpl *>(tmpl)->AddValidationField(W2T(fieldName), fieldType, fieldBitSize, offset);
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int LicenseTemplate_GetValidationFieldA(void *tmpl, const char *fieldName, int * fieldType, int *fieldSize, int *fieldOffset)
{
    USES_CONVERSION;

    if (static_cast<LicenseTemplateImpl *>(tmpl)->GetValidationField(A2T(fieldName), fieldType, fieldSize, fieldOffset))
        return STATUS_SUCCESS;

    return STATUS_GENERIC_ERROR;
}

int LicenseTemplate_GetValidationFieldW(void *tmpl, const wchar_t *fieldName, int * fieldType, int *fieldSize, int *fieldOffset)
{
    USES_CONVERSION;

    if (static_cast<LicenseTemplateImpl *>(tmpl)->GetValidationField(W2T(fieldName), fieldType, fieldSize, fieldOffset))
        return STATUS_SUCCESS;

    return STATUS_GENERIC_ERROR;
}

int LicenseTemplate_EnumValidationFieldsA(void * tmpl, void **enumHandle, const char **fieldName, int * fieldType, int * fieldSize, int * fieldOffset)
{
    int status = STATUS_SUCCESS;

    try {
#ifdef _UNICODE
        status = STATUS_GENERIC_ERROR;
#else
        if (!static_cast<LicenseTemplateImpl *>(tmpl)->EnumValidationFields(enumHandle, fieldName, fieldType, fieldSize, fieldOffset))
            status = STATUS_GENERIC_ERROR;
#endif
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return status;
}

int LicenseTemplate_EnumValidationFieldsW(void * tmpl, void **enumHandle, const wchar_t **fieldName, int * fieldType, int * fieldSize, int * fieldOffset)
{
    int status = STATUS_SUCCESS;

    try {
#ifdef _UNICODE
        if (!static_cast<LicenseTemplateImpl *>(tmpl)->EnumValidationFields(enumHandle, fieldName, fieldType, fieldSize, fieldOffset))
            status = STATUS_GENERIC_ERROR;
#else
        status = STATUS_GENERIC_ERROR;
#endif
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return status;
}

int LicenseTemplate_SetSignatureSize(void * tmpl, int signatureSize)
{
    try {
        static_cast<LicenseTemplateImpl *>(tmpl)->SetSignatureSize(signatureSize);
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int LicenseTemplate_GetSignatureSize(void * tmpl, int * signatureSize)
{
    try {
        *signatureSize = static_cast<LicenseTemplateImpl *>(tmpl)->GetSignatureSize();
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int LicenseTemplate_LoadXml(void * tmpl, const char * xmlTemplate)
{
    try {
        static_cast<LicenseTemplateImpl *>(tmpl)->LoadXml(xmlTemplate);
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int LicenseTemplate_SaveXml(void * tmpl, int savePrivateKey, const char **xmlTemplate)
{
    try {
        *xmlTemplate = static_cast<LicenseTemplateImpl *>(tmpl)->SaveXml((savePrivateKey) ? true : false);
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int LicenseTemplate_SetSigningServiceUrlA(void * tmpl, const char * url)
{
    USES_CONVERSION;
    
    try {
        static_cast<LicenseTemplateImpl *>(tmpl)->SetSigningServiceUrl(A2T(url));
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int LicenseTemplate_SetSigningServiceUrlW(void * tmpl, const wchar_t * url)
{
    USES_CONVERSION;

    try {
        static_cast<LicenseTemplateImpl *>(tmpl)->SetSigningServiceUrl(W2T(url));
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int LicenseTemplate_GetSigningServiceUrlA(void * tmpl, const char **url)
{
    try {
#ifdef _UNICODE
        *url = NULL;
#else
        *url = static_cast<LicenseTemplateImpl *>(tmpl)->GetSigningServiceUrl();
#endif
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int LicenseTemplate_GetSigningServiceUrlW(void * tmpl, const wchar_t **url)
{
    try {
#ifdef _UNICODE
        *url = static_cast<LicenseTemplateImpl *>(tmpl)->GetSigningServiceUrl();
#else
        *url = NULL;
#endif
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int LicenseTemplate_SetSigningServiceTemplateId(void * tmpl, int templateId)
{
    try {
        static_cast<LicenseTemplateImpl *>(tmpl)->SetSigningServiceTemplateId(templateId);
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int LicenseTemplate_GetSigningServiceTemplateId(void * tmpl, int * templateId)
{
    try {
        *templateId = static_cast<LicenseTemplateImpl *>(tmpl)->GetSigningServiceTemplateId();
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int KeyHelper_GetCurrentHardwareIdW(const wchar_t **hwid)
{
    try {
        *hwid = KeyHelperT<wchar_t>::GetCurrentHardwareId();
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int KeyHelper_GetCurrentHardwareIdA(const char **hwid)
{
    try {
        *hwid = KeyHelperT<char>::GetCurrentHardwareId();
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}



int KeyHelper_MatchCurrentHardwareIdW(const wchar_t *hwid)
{
    try {
        if (!KeyHelperT<wchar_t>::MatchCurrentHardwareId(hwid))
            return STATUS_INVALID_LICENSE_KEY;
    }
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int KeyHelper_MatchCurrentHardwareIdA(const char *hwid)
{
    try {
        if (!KeyHelperT<char>::MatchCurrentHardwareId(hwid))
            return STATUS_INVALID_LICENSE_KEY;
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int SDKRegistration_SetLicenseKeyA(const char * key)
{
    try {
        SDKRegistrationT<char>::SetLicenseKey(key);
    }
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int SDKRegistration_SetLicenseKeyW(const wchar_t * key)
{
    try {
        SDKRegistrationT<wchar_t>::SetLicenseKey(key);
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int LicenseTemplate_GetPublicKeyCertificateA(void * tmpl, const char **base64Certificate)
{
    try {
#ifdef _UNICODE
        USES_CONVERSION;
        static string base64KeyA;

        base64KeyA = W2A(static_cast<LicenseTemplateImpl *>(tmpl)->GetPublicKeyCertificate());

        *base64Certificate = base64KeyA.c_str();
#else
        *base64Certificate = static_cast<LicenseTemplateImpl *>(tmpl)->GetPublicKeyCertificate();
#endif
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int LicenseTemplate_GetPublicKeyCertificateW(void * tmpl, const wchar_t **base64Certificate)
{
    try {
#ifdef _UNICODE
        *base64Certificate = static_cast<LicenseTemplateImpl *>(tmpl)->GetPublicKeyCertificate();
#else
        *base64Certificate = NULL;
#endif
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int LicenseTemplate_SetPublicKeyCertificateA(void * tmpl, const char * base64Certificate)
{
    USES_CONVERSION;

    try {
        static_cast<LicenseTemplateImpl *>(tmpl)->SetPublicKeyCertificate(A2T(base64Certificate));
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int LicenseTemplate_SetPublicKeyCertificateW(void * tmpl, const wchar_t * base64Certificate)
{
    USES_CONVERSION;

    try {
        static_cast<LicenseTemplateImpl *>(tmpl)->SetPublicKeyCertificate(W2T(base64Certificate));
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int LicenseTemplate_GetPrivateKeyA(void * tmpl, const char **base64Key)
{
    try {
#ifdef _UNICODE
        USES_CONVERSION;
        static string base64KeyA;
        
        base64KeyA = W2A(static_cast<LicenseTemplateImpl *>(tmpl)->GetPrivateKey());

        *base64Key = base64KeyA.c_str();
#else
        *base64Key = static_cast<LicenseTemplateImpl *>(tmpl)->GetPrivateKey();
#endif
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int LicenseTemplate_GetPrivateKeyW(void * tmpl, const wchar_t **base64Key)
{
    try {
#ifdef _UNICODE
        *base64Key = static_cast<LicenseTemplateImpl *>(tmpl)->GetPrivateKey();
#else
        USES_CONVERSION;
        static wstring base64KeyW;
        
        base64KeyW = A2W(static_cast<LicenseTemplateImpl *>(tmpl)->GetPrivateKey());

        *base64Key = base64KeyW.c_str();
#endif
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int LicenseTemplate_SetPrivateKeyA(void * tmpl, const char * base64Key)
{
    USES_CONVERSION;

    try {
        static_cast<LicenseTemplateImpl *>(tmpl)->SetPrivateKey(A2T(base64Key));
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int LicenseTemplate_SetPrivateKeyW(void * tmpl, const wchar_t * base64Key)
{
    USES_CONVERSION;

    try {
        static_cast<LicenseTemplateImpl *>(tmpl)->SetPrivateKey(W2T(base64Key));
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int LicenseTemplate_GenerateSigningKeyPair(void * tmpl)
{
    try {
        static_cast<LicenseTemplateImpl *>(tmpl)->GenerateSigningKeyPair();
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int LicenseTemplate_SetPropertyA(void * tmpl, const char * path, const char * name, const char * value)
{
    USES_CONVERSION;
    USES_CONVERSION_2;
    
    try {
        static_cast<LicenseTemplateImpl *>(tmpl)->SetProperty(A2T(path), A2T_2(name), A2T_2(value));
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int LicenseTemplate_SetPropertyW(void * tmpl, const wchar_t * path, const wchar_t * name, const wchar_t * value)
{
    USES_CONVERSION;

    try {
        static_cast<LicenseTemplateImpl *>(tmpl)->SetProperty(W2T(path), W2T_2(name), W2T_2(value));
    }
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int LicenseTemplate_GetPropertyA(void * tmpl, const char * path, const char * name, const char **value)
{
    try {
#ifdef _UNICODE
        *value = NULL;
#else
        *value = static_cast<LicenseTemplateImpl *>(tmpl)->GetProperty(path, name);
#endif
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int LicenseTemplate_GetPropertyW(void *tmpl, const wchar_t * path, const wchar_t * name, const wchar_t **value)
{
    try {
#ifdef _UNICODE
        *value = static_cast<LicenseTemplateImpl *>(tmpl)->GetProperty(path, name);
#else
        *value = NULL;
#endif
    } 
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();
        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}

int Certificate_SignA(const char * csr, const char * privateKey, const char * signingPrivateKey, const char * signingCertificate, const char * certificateAuthorityUrl, int expYear, int expMonth, int expDay, const char **signedCert)
{
    USES_CONVERSION;
    USES_CONVERSION_2;
    USES_CONVERSION_3;
    USES_CONVERSION_4;
    USES_CONVERSION_5;

    try {
        static std::string staticSignedCert;
        
        staticSignedCert = T2A(Certificate::Sign(
            A2T(csr),
            A2T_2(privateKey),
            A2T_3(signingPrivateKey),
            A2T_4(signingCertificate),
            A2T_5(certificateAuthorityUrl),
            expYear, expMonth, expDay));

        *signedCert = staticSignedCert.c_str();
    }
    catch (Exception * ex)
    {
        int status = ex->GetCode();
        ex->Destroy();

        return status;
    }
    catch (...)
    {
        return STATUS_GENERIC_ERROR;
    }

    return STATUS_SUCCESS;
}
