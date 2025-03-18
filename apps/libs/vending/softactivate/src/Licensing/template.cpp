//
// Copyright (c) 2014 FreeGrabApp. All rights reserved.
//

#include "precomp.h"

#include "licensing.h"
#include "template.h"
#include "base64.h"
#include "except.h"
#include "uniconv.h"
#include "tinyxml2.h"

const unsigned char LicenseTemplateImpl::m_demoPublicKeys[10][23] = {
    {8, 0x67, 0xC9, 0x0C, 0xB9, 0xF9, 0x7D, 0x1C, 0x9C},
    {11, 0x76, 0x8C, 0x73, 0xC8, 0xC3, 0x6E, 0xEE, 0x7E, 0x3A, 0xC2, 0x13},
    {12, 0xC2, 0x8F, 0x03, 0x24, 0x4B, 0xAF, 0xC7, 0x94, 0x20, 0x3D, 0x46, 0x03},
    {13, 0xC8, 0xC3, 0x75, 0x6A, 0x9E, 0x74, 0x5F, 0xC9, 0x18, 0xCF, 0xA0, 0xD4, 0xA9},
    {14, 0x75, 0xD5, 0x7F, 0xDC, 0x08, 0x48, 0x93, 0x55, 0xDF, 0x63, 0xF9, 0xC4, 0x2C, 0x48},
    {14, 0xE1, 0x5A, 0xA2, 0xE0, 0xB7, 0x33, 0xFF, 0x88, 0x20, 0xFF, 0x1F, 0x91, 0xC9, 0x77},
    {17, 0xCC, 0x50, 0x01, 0x33, 0x11, 0x20, 0xFF, 0xA8, 0xED, 0x4F, 0xF2, 0x0A, 0xD7, 0xCB, 0xD0, 0xC3, 0x14},
    {18, 0x5A, 0xCC, 0xFB, 0xF5, 0x00, 0x62, 0xB5, 0x6B, 0xEE, 0x28, 0xE3, 0x92, 0x56, 0x2C, 0xAD, 0x76, 0xB8, 0xAE},
    {20, 0x4B, 0x36, 0x14, 0x2C, 0x29, 0xF0, 0x0C, 0xCD, 0x2F, 0xC2, 0xE0, 0x24, 0x4A, 0x6E, 0xCA, 0x83, 0x7B, 0xA7, 0xB1, 0x05},
    {22, 0x6B, 0x3C, 0x1D, 0xD7, 0x6F, 0xAC, 0x59, 0x37, 0xAA, 0xCC, 0x52, 0xF9, 0xBB, 0x7B, 0x25, 0x34, 0x93, 0x40, 0x7D, 0xE0, 0x90, 0x07}
};

const unsigned char LicenseTemplateImpl::m_demoPrivateKeys[10][23] = {
    {7, 0x5B, 0x3C, 0x97, 0xDC, 0x58, 0x4C, 0x37},
    {9, 0xC3, 0xD3, 0xB3, 0xE7, 0x13, 0x0E, 0xC2, 0x46, 0x2F},
    {10, 0x25, 0x72, 0x41, 0x0A, 0x22, 0x8E, 0x88, 0x25, 0x1C, 0x01},
    {12, 0x23, 0x5F, 0xA9, 0x40, 0x61, 0x5A, 0xB6, 0xC9, 0x07, 0x86, 0x1E, 0x01},
    {12, 0x24, 0x2F, 0x29, 0xA7, 0xE1, 0xAF, 0xC1, 0x42, 0x41, 0x82, 0xAF, 0x67},
    {13, 0xA8, 0x89, 0x6A, 0x64, 0x85, 0x9D, 0x00, 0xBC, 0x06, 0x92, 0xF0, 0xF9, 0x08},
    {15, 0x00, 0x8F, 0x7E, 0x9F, 0x61, 0x38, 0x8B, 0xB3, 0x9D, 0x2D, 0x68, 0x17, 0x7E, 0x7D, 0x19},
    {17, 0xBE, 0x53, 0x27, 0xF2, 0xB4, 0xD6, 0xA0, 0xA8, 0xF9, 0x10, 0x67, 0x94, 0x61, 0xD9, 0x8B, 0x4B, 0x03},
    {18, 0x0E, 0x15, 0xCB, 0xFC, 0x8D, 0x6C, 0x93, 0xA3, 0x1F, 0x70, 0xB8, 0xFB, 0x25, 0xB4, 0xB8, 0xE1, 0xED, 0x04},
    {20, 0x66, 0x8C, 0xE8, 0x03, 0x44, 0x4E, 0x5C, 0x81, 0x15, 0xBF, 0x01, 0x07, 0x6A, 0xA3, 0x59, 0xF6, 0xE0, 0x97, 0x76, 0x45}
};

const char_t * LicenseTemplateImpl::m_certificateAuthorityUrl = _T("http://vending.freegrabapp.com/LicensingService/api/Certificates/Issuer");

#define TEMPLATE  T_STR("LicenseKeyTemplate")
#define FORMAT    T_STR("LicenseKey")
#define DATA      T_STR("Data")
#define FIELD     T_STR("Field")
#define SIGNATURE T_STR("Signature")
#define SIGNATURE_PUBLIC_KEY  T_STR("SignaturePublicKey")
#define SIGNATURE_PRIVATE_KEY T_STR("SignaturePrivateKey")
#define SIGNING_SERVICE_URL   T_STR("SigningServiceUrl")
#define SIGNING_SERVICE_TEMPLATE_ID T_STR("SigningServiceTemplateId")
#define DATA_FIELDS           T_STR("DataFields")
#define VALIDATION_FIELDS     T_STR("ValidationFields")

void LicenseTemplateImpl::LoadXml(const char * xmlTemplate)
{
    USES_CONVERSION;
    tinyxml2::XMLDocument xml;

    if (xml.Parse(xmlTemplate) != tinyxml2::XML_NO_ERROR)
        throw new KeyExceptionImpl(STATUS_OUT_OF_MEMORY);

    std::string rootElementName;
    const char * attr;
    tinyxml2::XMLElement * element, * rootElement;
    
    if ((element = rootElement = xml.FirstChildElement()) == NULL) throw new KeyExceptionImpl(STATUS_OUT_OF_MEMORY);
    
    rootElementName = rootElement->Name();

    int templateVersion;

    if ((attr = element->Attribute("version")) == NULL)
        templateVersion = 1;
    else
        templateVersion = atoi(attr);

    if (templateVersion < 1 && templateVersion > 3)
        throw new KeyExceptionImpl(STATUS_GENERIC_ERROR, _T("unsupported template version"));

    if ((element = element->FirstChildElement("LicenseKey")) == NULL) throw new KeyExceptionImpl(STATUS_OUT_OF_MEMORY);

    m_keyEncoding = ENCODING_BASE32X;
    if ((attr = element->Attribute("encoding")) != NULL)
        if (_stricmp(attr, "base64") == 0)
            m_keyEncoding = ENCODING_BASE64X;

    if ((attr = element->Attribute("characterGroups")) == NULL)
        throw new KeyExceptionImpl(STATUS_OUT_OF_MEMORY);

    m_numGroups = atoi(attr);

    if ((attr = element->Attribute("charactersPerGroup")) == NULL)
        throw new KeyExceptionImpl(STATUS_OUT_OF_MEMORY);

    m_charsPerGroup = atoi(attr);

    m_groupSeparator = _T("-");

    if ((attr = element->Attribute("groupSeparator")) != NULL)
        m_groupSeparator = UTF82T(attr);

    m_header = _T("");

    if ((attr = element->Attribute("header")) != NULL)
        m_header = UTF82T(attr);

    m_footer = _T("");

    if ((attr = element->Attribute("footer")) != NULL)
        m_footer = UTF82T(attr);
    
    int type, offset, size;

    if ((element = xml.FirstChildElement()) == NULL) throw new KeyExceptionImpl(STATUS_GENERIC_ERROR);
    if ((element = element->FirstChildElement("LicenseKey")) == NULL) throw new KeyExceptionImpl(STATUS_GENERIC_ERROR);
    if ((element = element->FirstChildElement("Data")) == NULL) throw new KeyExceptionImpl(STATUS_GENERIC_ERROR);
    if ((element = element->FirstChildElement("DataFields")) == NULL) throw new KeyExceptionImpl(STATUS_GENERIC_ERROR);

    for (element = element->FirstChildElement("Field"); element != NULL; element = element->NextSiblingElement())
    {
        attr = element->Attribute("type");

        if ( _stricmp( attr, "raw" ) == 0 )
            type = FIELD_TYPE_RAW;
        else
        if ( _stricmp( attr, "integer" ) == 0 )
            type = FIELD_TYPE_INTEGER;
        else
        if ( _stricmp( attr, "string" ) == 0 )
            type = FIELD_TYPE_STRING;
        else
        if ( _stricmp( attr, "date13") == 0 )
            type = FIELD_TYPE_DATE13;
        else
        if ( _stricmp( attr, "date14" ) == 0 )
            type = FIELD_TYPE_DATE14;
        else
        if ( _stricmp( attr, "date16" ) == 0 )
            type = FIELD_TYPE_DATE16;
        else
            throw new KeyExceptionImpl(STATUS_GENERIC_ERROR);

        offset = 0;

        if ((attr = element->Attribute("offset")) != NULL )
        {
            offset = atoi(attr);
        }

        if ((attr = element->Attribute("size")) == NULL )
            throw new KeyExceptionImpl(STATUS_GENERIC_ERROR);

        size = atoi(attr);

        if ((attr = element->Attribute("name")) == NULL)
            throw new KeyExceptionImpl(STATUS_GENERIC_ERROR);

        m_keyDataMap->AddField(UTF82T(attr), (BitStruct::FIELD_TYPE)type, size, offset );
    }

    if ((element = xml.FirstChildElement()) == NULL) throw new KeyExceptionImpl(STATUS_GENERIC_ERROR);
    if ((element = element->FirstChildElement("LicenseKey")) == NULL) throw new KeyExceptionImpl(STATUS_GENERIC_ERROR);
    if ((element = element->FirstChildElement("Data")) == NULL) throw new KeyExceptionImpl(STATUS_GENERIC_ERROR);
    if ((element = element->FirstChildElement("ValidationFields")) == NULL) throw new KeyExceptionImpl(STATUS_GENERIC_ERROR);

    m_validationDataSize = 0;

    for (element = element->FirstChildElement("Field"); element != NULL; element = element->NextSiblingElement())
    {
        attr = element->Attribute("type");

        if ( _stricmp( attr, "raw" ) == 0 )
            type = FIELD_TYPE_RAW;
        else
        if ( _stricmp( attr, "integer" ) == 0 )
            type = FIELD_TYPE_INTEGER;
        else
        if ( _stricmp( attr, "string" ) == 0 )
            type = FIELD_TYPE_STRING;
        else
        if ( _stricmp ( attr, "date13" ) == 0 )
            type = FIELD_TYPE_DATE13;
        else
        if ( _stricmp( attr, "date14" ) == 0 )
            type = FIELD_TYPE_DATE14;
        else
        if ( _stricmp( attr, "date16" ) == 0 )
            type = FIELD_TYPE_DATE16;
        else
            throw new KeyExceptionImpl(STATUS_GENERIC_ERROR);

        offset = 0;

        if ((attr = element->Attribute("offset")) != NULL )
        {
            offset = atoi(attr);
        }

        if ((attr = element->Attribute("size")) == NULL )
            throw new KeyExceptionImpl(STATUS_GENERIC_ERROR);

        size = atoi(attr);

        if ((attr = element->Attribute("name")) == NULL)
            throw new KeyExceptionImpl(STATUS_GENERIC_ERROR);

        m_validationDataMap->AddField(UTF82T(attr), (BitStruct::FIELD_TYPE)type, size, offset );

        if (m_validationDataSize < offset + size)
                m_validationDataSize = offset + size;
    }

    if ((element = xml.FirstChildElement()) == NULL) throw new KeyExceptionImpl(STATUS_GENERIC_ERROR);
    if ((element = element->FirstChildElement("LicenseKey")) == NULL) throw new KeyExceptionImpl(STATUS_GENERIC_ERROR);
    if ((element = element->FirstChildElement("Signature")) == NULL) throw new KeyExceptionImpl(STATUS_GENERIC_ERROR);

    if ((attr = element->Attribute("size")) == NULL)
        throw new KeyExceptionImpl(STATUS_GENERIC_ERROR, _T("signature size not provided in template"));

    int signatureSize = atoi(attr);
    if (signatureSize <= 0)
        throw new KeyExceptionImpl(STATUS_GENERIC_ERROR, _T("invalid signature size"));

    SetSignatureSize(signatureSize);

    SetDataSize((m_numGroups * m_charsPerGroup * m_keyEncoding) - signatureSize);

    tinyxml2::XMLElement * element2;
    
    if ((element2 = element->FirstChildElement("SignaturePrivateKey")) != NULL)
    {
        if (templateVersion == 1)
        {
            m_signingKey = new Key(UTF82T(element2->GetText()), m_signatureKeyType);
            
            int len = 1024;
            void * buf = _alloca(1024);

            if (!m_signingKey->Store(buf, &len))
                throw new KeyExceptionImpl(STATUS_GENERIC_ERROR, _T("invalid private key"));

            ValidatePrivateKey(buf, len);
        } else 
        if (templateVersion <= 3)
            SetPrivateKey(UTF82T(element2->GetText()));
        else
            throw new KeyExceptionImpl(STATUS_GENERIC_ERROR, _T("unsupported template version"));
    }

    if ((element2 = element->FirstChildElement("SignaturePublicKey")) != NULL)
    {
        if (templateVersion == 1)
        {
            m_verificationKey = new Key(UTF82T(element2->GetText()), m_signatureKeyType);
            m_certificate = Certificate::Generate(signatureSize, m_signingKey, m_verificationKey, NULL, NULL, m_certificateAuthorityUrl, 0);
            ValidateCertificate();
        } 
        else
        if (templateVersion <= 3)
            SetPublicKeyCertificate(UTF82T(element2->GetText()));
        else
            throw new KeyExceptionImpl(STATUS_GENERIC_ERROR, _T("unsupported template version"));
    }

    if ((element2 = element->FirstChildElement("SigningServiceUrl")) != NULL)
        SetSigningServiceUrl(UTF82T(element2->GetText()));

    if ((element2 = element->FirstChildElement("SigningServiceTemplateId")) != NULL)
        SetSigningServiceTemplateId(atoi(element2->GetText()));

    if (templateVersion < 3)
        return;

    if ((element = rootElement->FirstChildElement("Properties")) != NULL)
    {
        m_properties.LoadXml(element);
    }
}

#ifndef MAX_XML_BUFFER_SIZE
#define MAX_XML_BUFFER_SIZE 4096
#endif

const char * LicenseTemplateImpl::SaveXml(bool savePrivateKey)
{
    USES_CONVERSION;
    BASE64 base64;
    char_t buffer[ MAX_XML_BUFFER_SIZE ];

    m_xmlTemplate.resize(0);

    t_snprintf(buffer, MAX_XML_BUFFER_SIZE, T_STR("<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n"));m_xmlTemplate.append(T2UTF8(buffer));
    t_snprintf(buffer, MAX_XML_BUFFER_SIZE, T_STR("<LicenseTemplate version=\"%d\">\n"), m_version);m_xmlTemplate.append(T2UTF8(buffer));
    t_snprintf(buffer, MAX_XML_BUFFER_SIZE, T_STR("\t<LicenseKey\n")
                            T_STR("\t\tencoding=\"%s\"\n")
                            T_STR("\t\tcharacterGroups=\"%d\"\n")
                            T_STR("\t\tcharactersPerGroup=\"%d\"\n")
                            T_STR("\t\tgroupSeparator=\"%s\"\n")
                            T_STR("\t\theader=\"%s\"\n")
                            T_STR("\t\tfooter=\"%s\">\n"), (m_keyEncoding == ENCODING_BASE64X) ? T_STR("BASE64") : T_STR("BASE32X"), m_numGroups, m_charsPerGroup, m_groupSeparator.c_str(), m_header.c_str(), m_footer.c_str());m_xmlTemplate.append(T2UTF8(buffer));
    t_snprintf(buffer, MAX_XML_BUFFER_SIZE, T_STR("\t\t<Data>\n"));m_xmlTemplate.append(T2UTF8(buffer));
    t_snprintf(buffer, MAX_XML_BUFFER_SIZE, T_STR("\t\t\t<DataFields size=\"%d\">\n"), m_dataSize);m_xmlTemplate.append(T2UTF8(buffer));

    void * enumHandle;
    const char_t * name;
    BitStruct::FIELD_TYPE type;
    int size, offset;
    string_t fieldType;

    enumHandle = NULL;

    while (m_keyDataMap->EnumFields(&enumHandle, &name, &type, &size, &offset))
    {
        switch ((int)type)
        {
            case FIELD_TYPE_INTEGER:
                fieldType = _T("Integer");
                break;

            case FIELD_TYPE_STRING:
                fieldType = _T("String");
                break;

            case FIELD_TYPE_RAW:
                fieldType = _T("Raw");
                break;

            case FIELD_TYPE_DATE13:
                fieldType = _T("Date13");
                break;

            case FIELD_TYPE_DATE14:
                fieldType = _T("Date14");
                break;

            case FIELD_TYPE_DATE16:
                fieldType = _T("Date16");
                break;

            default:
                fieldType = _T("Raw");
        }

        t_snprintf(buffer, MAX_XML_BUFFER_SIZE, T_STR("\t\t\t\t<Field name=\"%s\" type=\"%s\" size=\"%d\" offset=\"%d\" />\n"), name, fieldType.c_str(), size, offset); 
        m_xmlTemplate.append(T2UTF8(buffer));
    }

    t_snprintf(buffer, MAX_XML_BUFFER_SIZE, T_STR("\t\t\t</DataFields>\n"));m_xmlTemplate.append(T2UTF8(buffer));
    t_snprintf(buffer, MAX_XML_BUFFER_SIZE, T_STR("\t\t\t<ValidationFields>\n"));m_xmlTemplate.append(T2UTF8(buffer));

    enumHandle = NULL;

    while (m_validationDataMap->EnumFields(&enumHandle, &name, &type, &size, &offset))
    {
        switch ((int)type)
        {
            case FIELD_TYPE_INTEGER:
                fieldType = _T("Integer");
                break;

            case FIELD_TYPE_STRING:
                fieldType = _T("String");
                break;

            case FIELD_TYPE_RAW:
                fieldType = _T("Raw");
                break;

            case FIELD_TYPE_DATE13:
                fieldType = _T("Date13");
                break;

            case FIELD_TYPE_DATE14:
                fieldType = _T("Date14");
                break;

            case FIELD_TYPE_DATE16:
                fieldType = _T("Date16");
                break;

            default:
                fieldType = _T("Raw");
        }

        t_snprintf(buffer, MAX_XML_BUFFER_SIZE, T_STR("\t\t\t\t<Field name=\"%s\" type=\"%s\" size=\"%d\" offset=\"%d\" />\n"), name, fieldType.c_str(), size, offset); 
        m_xmlTemplate.append(T2UTF8(buffer));
    }

    t_snprintf(buffer, MAX_XML_BUFFER_SIZE, T_STR("\t\t\t</ValidationFields>\n"));m_xmlTemplate.append(T2UTF8(buffer));
    t_snprintf(buffer, MAX_XML_BUFFER_SIZE, T_STR("\t\t</Data>\n"));m_xmlTemplate.append(T2UTF8(buffer));
    t_snprintf(buffer, MAX_XML_BUFFER_SIZE, T_STR("\t\t<Signature size=\"%d\">\n"), m_signatureSize);m_xmlTemplate.append(T2UTF8(buffer));

    if (m_certificate)
    {
        t_snprintf(buffer, MAX_XML_BUFFER_SIZE, T_STR("\t\t\t<SignaturePublicKey>%s</SignaturePublicKey>\n"), m_certificate->ToString() );m_xmlTemplate.append(T2UTF8(buffer));

        if (savePrivateKey && m_signingKey)
        {
            t_snprintf(buffer, MAX_XML_BUFFER_SIZE, T_STR("\t\t\t<SignaturePrivateKey>%s</SignaturePrivateKey>\n"), m_signingKey->ToString() );m_xmlTemplate.append(T2UTF8(buffer));
        }
    }

    if (m_signingServiceUrl.length() > 0)
    {
        t_snprintf(buffer, MAX_XML_BUFFER_SIZE, T_STR("\t\t\t<SigningServiceUrl>%s</SigningServiceUrl>\n"), m_signingServiceUrl.c_str() );m_xmlTemplate.append(T2UTF8(buffer));
    }

    if (m_signingServiceTemplateId != 0)
    {
        t_snprintf(buffer, MAX_XML_BUFFER_SIZE, T_STR("\t\t\t<SigningServiceTemplateId>%d</SigningServiceTemplateId>\n"), m_signingServiceTemplateId );m_xmlTemplate.append(T2UTF8(buffer));
    }

    t_snprintf(buffer, MAX_XML_BUFFER_SIZE, T_STR("\t\t</Signature>\n"));m_xmlTemplate.append(T2UTF8(buffer));
    t_snprintf(buffer, MAX_XML_BUFFER_SIZE, T_STR("\t</LicenseKey>\n"));m_xmlTemplate.append(T2UTF8(buffer));
    
    t_snprintf(buffer, MAX_XML_BUFFER_SIZE, T_STR("\t<Properties>\n"));m_xmlTemplate.append(T2UTF8(buffer));
        m_properties.SaveXml(m_xmlTemplate);
    t_snprintf(buffer, MAX_XML_BUFFER_SIZE, T_STR("\t</Properties>\n"));m_xmlTemplate.append(T2UTF8(buffer));
    
    t_snprintf(buffer, MAX_XML_BUFFER_SIZE, T_STR("</LicenseTemplate>"));m_xmlTemplate.append(T2UTF8(buffer));

    return m_xmlTemplate.c_str();
}

namespace FreeGrabApp {
    namespace Licensing {
        template<>
        LicenseTemplateT<char>::LicenseTemplateT():
        m_Impl(*(new LicenseTemplateImpl()))
        {

        }

        template<>
        LicenseTemplateT<wchar_t>::LicenseTemplateT():
        m_Impl(*(new LicenseTemplateImpl()))
        {

        }

        template<>
        LicenseTemplateT<char>::~LicenseTemplateT()
        {
            delete & m_Impl;
        }

        template<>
        LicenseTemplateT<wchar_t>::~LicenseTemplateT()
        {
            delete & m_Impl;
        }

        template<>
        LicenseTemplateT<char> * LicenseTemplateT<char>::Create()
        {
            return new LicenseTemplateT<char>();
        }

        template<>
        LicenseTemplateT<wchar_t> * LicenseTemplateT<wchar_t>::Create()
        {
            return new LicenseTemplateT<wchar_t>();
        }

        template<>
        void LicenseTemplateT<char>::Destroy(LicenseTemplateT<char> * obj)
        {
            delete obj;
        }

        template<>
        void LicenseTemplateT<wchar_t>::Destroy(LicenseTemplateT<wchar_t> * obj)
        {
            delete obj;
        }

        template<>
        void LicenseTemplateT<char>::SetNumberOfGroups(int numGroups)
        {
            m_Impl.SetNumberOfGroups(numGroups);
        }

        template<>
        void LicenseTemplateT<wchar_t>::SetNumberOfGroups(int numGroups)
        {
            m_Impl.SetNumberOfGroups(numGroups);
        }

        template<>
        unsigned LicenseTemplateT<char>::GetNumberOfGroups()
        {
            return m_Impl.GetNumberOfGroups();
        }

        template<>
        unsigned LicenseTemplateT<wchar_t>::GetNumberOfGroups()
        {
            return m_Impl.GetNumberOfGroups();
        }

        template<>
        void LicenseTemplateT<char>::SetCharactersPerGroup(int charsPerGroup)
        {
            m_Impl.SetCharactersPerGroup(charsPerGroup);
        }

        template<>
        void LicenseTemplateT<wchar_t>::SetCharactersPerGroup(int charsPerGroup)
        {
            m_Impl.SetCharactersPerGroup(charsPerGroup);
        }

        template<>
        unsigned LicenseTemplateT<char>::GetCharactersPerGroup()
        {
            return m_Impl.GetCharactersPerGroup();
        }

        template<>
        unsigned LicenseTemplateT<wchar_t>::GetCharactersPerGroup()
        {
            return m_Impl.GetCharactersPerGroup();
        }

        template<>
        void LicenseTemplateT<char>::SetGroupSeparator(const char * groupSep)
        {
            USES_CONVERSION;
            m_Impl.SetGroupSeparator(A2T(groupSep));
        }

        template<>
        void LicenseTemplateT<wchar_t>::SetGroupSeparator(const wchar_t * groupSep)
        {
            USES_CONVERSION;
            m_Impl.SetGroupSeparator(W2T(groupSep));
        }

        template<>
        const char * LicenseTemplateT<char>::GetGroupSeparator()
        {
        #ifdef _UNICODE
            USES_CONVERSION;
            static std::string groupSep;
            groupSep = W2A(m_Impl.GetGroupSeparator());
            return groupSep.c_str();
        #else
            return m_Impl.GetGroupSeparator();
        #endif
        }

        template<>
        const wchar_t * LicenseTemplateT<wchar_t>::GetGroupSeparator()
        {
        #ifdef _UNICODE
            return m_Impl.GetGroupSeparator();
        #else
            USES_CONVERSION;
            static std::wstring groupSep;
            groupSep = A2W(m_Impl.GetGroupSeparator());
            return groupSep.c_str();
        #endif
        }

        template<>
        void LicenseTemplateT<char>::SetEncoding(int encoding)
        {
            m_Impl.SetEncoding(encoding);
        }

        template<>
        void LicenseTemplateT<wchar_t>::SetEncoding(int encoding)
        {
            m_Impl.SetEncoding(encoding);
        }

        template<>
        int LicenseTemplateT<char>::GetEncoding()
        {
            return m_Impl.GetEncoding();
        }

        template<>
        int LicenseTemplateT<wchar_t>::GetEncoding()
        {
            return m_Impl.GetEncoding();
        }

        template<>
        void LicenseTemplateT<char>::SetHeader(const char * header)
        {
            USES_CONVERSION;
            m_Impl.SetHeader(A2T(header));
        }

        template<>
        void LicenseTemplateT<wchar_t>::SetHeader(const wchar_t * header)
        {
            USES_CONVERSION;
            m_Impl.SetHeader(W2T(header));
        }

        template<>
        const char * LicenseTemplateT<char>::GetHeader()
        {
        #ifdef _UNICODE
            USES_CONVERSION;
            static std::string header;
            header = W2A(m_Impl.GetHeader());
            return header.c_str();
        #else
            return m_Impl.GetHeader();
        #endif
        }

        template<>
        const wchar_t * LicenseTemplateT<wchar_t>::GetHeader()
        {
        #ifdef _UNICODE
            return m_Impl.GetHeader();
        #else
            USES_CONVERSION;
            static std::wstring header;
            header = A2W(m_Impl.GetHeader());
            return header.c_str();
        #endif
        }

        template<>
        void LicenseTemplateT<char>::SetFooter(const char * footer)
        {
            USES_CONVERSION;
            m_Impl.SetFooter(A2T(footer));
        }

        template<>
        void LicenseTemplateT<wchar_t>::SetFooter(const wchar_t * footer)
        {
            USES_CONVERSION;
            m_Impl.SetFooter(W2T(footer));
        }

        template<>
        const char * LicenseTemplateT<char>::GetFooter()
        {
        #ifdef _UNICODE
            USES_CONVERSION;
            static std::string footer;
            footer = W2A(m_Impl.GetFooter());
            return footer.c_str();
        #else
            return m_Impl.GetFooter();
        #endif
        }

        template<>
        const wchar_t * LicenseTemplateT<wchar_t>::GetFooter()
        {
        #ifdef _UNICODE
            return m_Impl.GetFooter();
        #else
            USES_CONVERSION;
            static std::wstring footer;
            footer = A2W(m_Impl.GetFooter());
            return footer.c_str();
        #endif
        }

        template<>
        void LicenseTemplateT<char>::SetDataSize(int sizeInBits)
        {
            m_Impl.SetDataSize(sizeInBits);
        }

        template<>
        void LicenseTemplateT<wchar_t>::SetDataSize(int sizeInBits)
        {
            m_Impl.SetDataSize(sizeInBits);
        }

        template<>
        unsigned LicenseTemplateT<char>::GetDataSize()
        {
            return m_Impl.GetDataSize();
        }

        template<>
        unsigned LicenseTemplateT<wchar_t>::GetDataSize()
        {
            return m_Impl.GetDataSize();
        }

        template<>
        void LicenseTemplateT<char>::AddDataField(const char * fieldName, int fieldType, int fieldBitSize, int offset)
        {
            USES_CONVERSION;
            m_Impl.AddDataField(A2T(fieldName), fieldType, fieldBitSize, offset);
        }

        template<>
        void LicenseTemplateT<wchar_t>::AddDataField(const wchar_t * fieldName, int fieldType, int fieldBitSize, int offset)
        {
            USES_CONVERSION;
            m_Impl.AddDataField(W2T(fieldName), fieldType, fieldBitSize, offset);
        }

        template<>
        bool LicenseTemplateT<wchar_t>::GetDataField(const wchar_t * fieldName, int * fieldType, int * fieldBitSize, int * offset)
        {
            USES_CONVERSION;
            return m_Impl.GetDataField(W2T(fieldName), fieldType, fieldBitSize, offset);
        }

        template<>
        bool LicenseTemplateT<char>::GetDataField(const char * fieldName, int * fieldType, int * fieldBitSize, int * offset)
        {
            USES_CONVERSION;
            return m_Impl.GetDataField(A2T(fieldName), fieldType, fieldBitSize, offset);
        }

        template<>
        bool LicenseTemplateT<char>::EnumDataFields(void **enumHandle, const char **fieldName, int * fieldType, int * fieldSize, int * offset)
        {
        #ifdef _UNICODE
            USES_CONVERSION;
            const char_t * name;
            static std::string staticName;
            bool result = m_Impl.EnumDataFields(enumHandle, &name, fieldType, fieldSize, offset);
            staticName = T2A(name);
            *fieldName = staticName.c_str();
            return result;
        #else
            return m_Impl.EnumDataFields(enumHandle, fieldName, fieldType, fieldSize, offset);
        #endif
        }

        template<>
        bool LicenseTemplateT<wchar_t>::EnumDataFields(void **enumHandle, const wchar_t **fieldName, int * fieldType, int * fieldSize, int * offset)
        {
        #ifdef _UNICODE
            return m_Impl.EnumDataFields(enumHandle, fieldName, fieldType, fieldSize, offset);
        #else
            USES_CONVERSION;
            const char * name;
            static std::wstring staticName;
            bool result = m_Impl.EnumDataFields(enumHandle, &name, fieldType, fieldSize, offset);
            staticName = T2W(name);
            *fieldName = staticName.c_str();
            return result;
        #endif
        }

        template<>
        void LicenseTemplateT<char>::SetValidationDataSize(int sizeInBits)
        {
            m_Impl.SetValidationDataSize(sizeInBits);
        }

        template<>
        void LicenseTemplateT<wchar_t>::SetValidationDataSize(int sizeInBits)
        {
            m_Impl.SetValidationDataSize(sizeInBits);
        }

        template<>
        unsigned LicenseTemplateT<char>::GetValidationDataSize()
        {
            return m_Impl.GetValidationDataSize();
        }

        template<>
        unsigned LicenseTemplateT<wchar_t>::GetValidationDataSize()
        {
            return m_Impl.GetValidationDataSize();
        }

        template<>
        void LicenseTemplateT<char>::AddValidationField(const char * fieldName, int fieldType, int fieldBitSize, int offset)
        {
            USES_CONVERSION;
            m_Impl.AddValidationField(A2T(fieldName), fieldType, fieldBitSize, offset);
        }

        template<>
        void LicenseTemplateT<wchar_t>::AddValidationField(const wchar_t * fieldName, int fieldType, int fieldBitSize, int offset)
        {
            USES_CONVERSION;
            m_Impl.AddValidationField(W2T(fieldName), fieldType, fieldBitSize, offset);
        }

        template<>
        bool LicenseTemplateT<char>::EnumValidationFields(void **enumHandle, const char **fieldName, int * fieldType, int * fieldSize, int * offset)
        {
        #ifdef _UNICODE
            USES_CONVERSION;
            const char_t * name;
            static std::string staticName;
            bool result = m_Impl.EnumValidationFields(enumHandle, &name, fieldType, fieldSize, offset);
            staticName = T2A(name);
            *fieldName = staticName.c_str();
            return result;
        #else
            return m_Impl.EnumValidationFields(enumHandle, fieldName, fieldType, fieldSize, offset);
        #endif
        }

        template<>
        bool LicenseTemplateT<wchar_t>::EnumValidationFields(void **enumHandle, const wchar_t **fieldName, int * fieldType, int * fieldSize, int * offset)
        {
        #ifdef _UNICODE
            return m_Impl.EnumValidationFields(enumHandle, fieldName, fieldType, fieldSize, offset);
        #else
            USES_CONVERSION;
            const char * name;
            static std::wstring staticName;
            bool result = m_Impl.EnumValidationFields(enumHandle, &name, fieldType, fieldSize, offset);
            staticName = T2W(name);
            *fieldName = staticName.c_str();
            return result;
        #endif
        }

        template<>
        bool LicenseTemplateT<wchar_t>::GetValidationField(const wchar_t * fieldName, int * fieldType, int * fieldBitSize, int * offset)
        {
            USES_CONVERSION;
            return m_Impl.GetValidationField(W2T(fieldName), fieldType, fieldBitSize, offset);
        }

        template<>
        bool LicenseTemplateT<char>::GetValidationField(const char * fieldName, int * fieldType, int * fieldBitSize, int * offset)
        {
            USES_CONVERSION;
            return m_Impl.GetValidationField(A2T(fieldName), fieldType, fieldBitSize, offset);
        }

        template<>
        void LicenseTemplateT<char>::SetSignatureSize(int signatureSize)
        {
            m_Impl.SetSignatureSize(signatureSize);
        }

        template<>
        void LicenseTemplateT<wchar_t>::SetSignatureSize(int signatureSize)
        {
            m_Impl.SetSignatureSize(signatureSize);
        }

        template<>
        unsigned LicenseTemplateT<char>::GetSignatureSize()
        {
            return m_Impl.GetSignatureSize();
        }

        template<>
        unsigned LicenseTemplateT<wchar_t>::GetSignatureSize()
        {
            return m_Impl.GetSignatureSize();
        }

        template<>
        void LicenseTemplateT<char>::SetVersion(int version)
        {
            m_Impl.SetVersion(version);
        }

        template<>
        void LicenseTemplateT<wchar_t>::SetVersion(int version)
        {
            m_Impl.SetVersion(version);
        }

        template<>
        unsigned LicenseTemplateT<char>::GetVersion()
        {
            return m_Impl.GetVersion();
        }

        template<>
        unsigned LicenseTemplateT<wchar_t>::GetVersion()
        {
            return m_Impl.GetVersion();
        }

        template<>
        void LicenseTemplateT<char>::LoadXml(const char * xmlTemplate)
        {
            m_Impl.LoadXml(xmlTemplate);
        }

        template<>
        void LicenseTemplateT<wchar_t>::LoadXml(const char * xmlTemplate)
        {
            m_Impl.LoadXml(xmlTemplate);
        }

        template<>
        const char * LicenseTemplateT<char>::SaveXml(bool savePrivateKey)
        {
            return m_Impl.SaveXml(savePrivateKey);
        }

        template<>
        const char * LicenseTemplateT<wchar_t>::SaveXml(bool savePrivateKey)
        {
            return m_Impl.SaveXml(savePrivateKey);
        }

        template<>
        void LicenseTemplateT<char>::SetSigningServiceUrl(const char * url)
        {
            USES_CONVERSION;
            m_Impl.SetSigningServiceUrl(A2T(url));
        }

        template<>
        void LicenseTemplateT<wchar_t>::SetSigningServiceUrl(const wchar_t * url)
        {
            USES_CONVERSION;
            m_Impl.SetSigningServiceUrl(W2T(url));
        }

        template<>
        const char * LicenseTemplateT<char>::GetSigningServiceUrl()
        {
        #ifdef _UNICODE
            USES_CONVERSION;
            static std::string signingServiceUrl;
            signingServiceUrl = W2A(m_Impl.GetSigningServiceUrl());
            return signingServiceUrl.c_str();
        #else
            return m_Impl.GetSigningServiceUrl();
        #endif
        }

        template<>
        const wchar_t * LicenseTemplateT<wchar_t>::GetSigningServiceUrl()
        {
        #ifdef _UNICODE
            return m_Impl.GetSigningServiceUrl();
        #else
            USES_CONVERSION;
            static std::wstring signingServiceUrl;
            signingServiceUrl = A2W(m_Impl.GetSigningServiceUrl());
            return signingServiceUrl.c_str();
        #endif
        }

        template<>
        void LicenseTemplateT<char>::SetPublicKeyCertificate(const char * base64Certificate)
        {
            USES_CONVERSION;
            m_Impl.SetPublicKeyCertificate(A2T(base64Certificate));
        }

        template<>
        void LicenseTemplateT<wchar_t>::SetPublicKeyCertificate(const wchar_t * base64Certificate)
        {
        #ifndef _UNICODE
            USES_CONVERSION;
        #endif
            m_Impl.SetPublicKeyCertificate(W2T(base64Certificate));
        }

        template<>
        const char * LicenseTemplateT<char>::GetPublicKeyCertificate()
        {
        #ifdef _UNICODE
            USES_CONVERSION;
            static std::string cert;
            cert = W2A(m_Impl.GetPublicKeyCertificate());
            return cert.c_str();
        #else
            return m_Impl.GetPublicKeyCertificate();
        #endif
        }

        template<>
        const wchar_t * LicenseTemplateT<wchar_t>::GetPublicKeyCertificate()
        {
        #ifdef _UNICODE
            return m_Impl.GetPublicKeyCertificate();
        #else
            USES_CONVERSION;
            static std::wstring cert;
            cert = A2W(m_Impl.GetPublicKeyCertificate());
            return cert.c_str();
        #endif
        }

        template<>
        void LicenseTemplateT<char>::SetPrivateKey(const char * base64Key)
        {
            USES_CONVERSION;
            m_Impl.SetPrivateKey(A2T(base64Key));
        }

        template<>
        void LicenseTemplateT<wchar_t>::SetPrivateKey(const wchar_t * base64Key)
        {
        #ifndef _UNICODE
            USES_CONVERSION;
        #endif
            m_Impl.SetPrivateKey(W2T(base64Key));
        }

        template<>
        const char * LicenseTemplateT<char>::GetPrivateKey()
        {
        #ifdef _UNICODE
            USES_CONVERSION;
            static std::string key;
            key = W2A(m_Impl.GetPrivateKey());
            return key.c_str();
        #else
            return m_Impl.GetPrivateKey();
        #endif
        }

        template<>
        const wchar_t * LicenseTemplateT<wchar_t>::GetPrivateKey()
        {
        #ifdef _UNICODE
            return m_Impl.GetPrivateKey();
        #else
            USES_CONVERSION;
            static std::wstring key;
            key = A2W(m_Impl.GetPrivateKey());
            return key.c_str();
        #endif
        }

        template<>
        void LicenseTemplateT<char>::SetProperty(const char * path, const char * name, const char * value)
        {
            USES_CONVERSION;
            USES_CONVERSION_2;

            m_Impl.SetProperty(A2T(path), (name) ? A2T_2(name) : NULL, A2T_2(value));
        }

        template<>
        void LicenseTemplateT<wchar_t>::SetProperty(const wchar_t * path, const wchar_t * name, const wchar_t * value)
        {
            USES_CONVERSION;
            m_Impl.SetProperty(W2T(path), (name) ? W2T_2(name) : NULL, W2T_2(value));
        }

        template<>
        const char * LicenseTemplateT<char>::GetProperty(const char * path, const char * name)
        {
        #ifdef _UNICODE
            USES_CONVERSION;
            USES_CONVERSION_2;
            static std::string value;
            value = W2A(m_Impl.GetProperty(A2W(path), A2W_2(name)));
            return value.c_str();
        #else
            return m_Impl.GetSigningServiceUrl();
        #endif
        }

        template<>
        const wchar_t * LicenseTemplateT<wchar_t>::GetProperty(const wchar_t * path, const wchar_t * name)
        {
        #ifdef _UNICODE
            return m_Impl.GetProperty(path, name);
        #else
            USES_CONVERSION;
            static std::wstring value;
            value = A2W(m_Impl.GetProperty(W2A(path), W2A_2(name)));
            return value.c_str();
        #endif
        }

        template<>
        void LicenseTemplateT<char>::GenerateSigningKeyPair()
        {
            m_Impl.GenerateSigningKeyPair();
        }

        template<>
        void LicenseTemplateT<wchar_t>::GenerateSigningKeyPair()
        {
            m_Impl.GenerateSigningKeyPair();
        }
    };
};
