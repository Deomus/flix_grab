#ifndef __TEMPLATE_H
#define __TEMPLATE_H

#include <map>
#include "ecc.h"
#include "base64.h"
#include "uniconv.h"
#include "except.h"
#include "sdkregistration.h"
#include "certificate.h"
#include "propertycollection.h"

class LicenseTemplateImpl {
friend class LicenseTemplate;

public:
    LicenseTemplateImpl(const char * xmlTemplate = NULL)
    {
        Init();

        if ( xmlTemplate )
            LoadXml(xmlTemplate);
    }

    ~LicenseTemplateImpl()
    {
        if (m_signingKey)
            delete m_signingKey;

        if (m_verificationKey)
            delete m_verificationKey;

        delete m_keyDataMap;
        delete m_validationDataMap;
    }

    void SetVersion(int version)
    {
        if (version != 1)
            throw new KeyExceptionImpl(STATUS_UNSUPPORTED_VERSION);

        m_version = version;
    }

    unsigned GetVersion()
    {
        return m_version;
    }

    void SetNumberOfGroups(int numGroups)
    {
        m_numGroups = numGroups;
    }

    unsigned GetNumberOfGroups() const
    {
        return m_numGroups;
    }

    void SetCharactersPerGroup(int charsPerGroup)
    {
        m_charsPerGroup = charsPerGroup;
    }

    unsigned GetCharactersPerGroup() const
    {
        return m_charsPerGroup;
    }

    void SetGroupSeparator(const char_t * groupSep)
    {
        m_groupSeparator = groupSep;
    }

    const char_t * GetGroupSeparator() const
    {
        return m_groupSeparator.c_str();
    }

    void SetEncoding(int encoding)
    {
        m_keyEncoding = encoding;
    }

    int GetEncoding() const
    {
        return m_keyEncoding;
    }

    void SetHeader(const char_t * header)
    {
        m_header = header;
    }

    const char_t * GetHeader() const
    {
        return m_header.c_str();
    }

    void SetFooter(const char_t * footer)
    {
        m_footer = footer;
    }

    const char_t * GetFooter() const
    {
        return m_footer.c_str();
    }

    void SetDataSize(int sizeInBits)
    {
        m_dataSize = sizeInBits;
    }
    
    unsigned GetDataSize() const
    {
        return m_dataSize;
    }

    void AddDataField(const char_t * fieldName, int fieldType, int fieldSizeInBits, int fieldOffset = -1 )
    {
        m_keyDataMap->AddField(fieldName, (BitStruct::FIELD_TYPE)fieldType, fieldSizeInBits, fieldOffset);
    }

    bool GetDataField(const char_t * fieldName, int * fieldType, int * fieldSizeInBits, int * fieldOffset)
    {
        BitStruct::FIELD_TYPE type;

        if (!m_keyDataMap->GetField(fieldName, &type, fieldSizeInBits, fieldOffset))
            return false;

        *fieldType = type;

        return true;
    }

    bool EnumDataFields(void **enumHandle, const char_t **fieldName, int * fieldType, int * fieldSizeInBits, int * fieldOffset)
    {
        BitStruct::FIELD_TYPE type;
        bool result = m_keyDataMap->EnumFields(enumHandle, fieldName, &type, fieldSizeInBits, fieldOffset);
        *fieldType = type;
        return result;
    }

    void SetValidationDataSize(int sizeInBits)
    {
        m_validationDataSize = sizeInBits;
    }

    unsigned GetValidationDataSize()
    {
        return m_validationDataSize;
    }

    void AddValidationField(const char_t * fieldName, int fieldType, int fieldBitSize, int fieldOffset = 0)
    {
        m_validationDataMap->AddField(fieldName, (BitStruct::FIELD_TYPE)fieldType, fieldBitSize, fieldOffset);
    }

    bool GetValidationField(const char_t * fieldName, int * fieldType, int * fieldSizeInBits, int * fieldOffset)
    {
        BitStruct::FIELD_TYPE type;

        if (!m_validationDataMap->GetField(fieldName, &type, fieldSizeInBits, fieldOffset))
            return false;

        *fieldType = type;

        return true;
    }

    bool EnumValidationFields(void **enumHandle, const char_t **fieldName, int * fieldType, int * fieldSize, int * fieldOffset)
    {
        BitStruct::FIELD_TYPE type;
        bool result = m_validationDataMap->EnumFields(enumHandle, fieldName, &type, fieldSize, fieldOffset);
        *fieldType = type;
        return result;
    }

    void SetSignatureSize(int sizeInBits)
    {
        if (sizeInBits < 76 || sizeInBits > 322)
            throw new KeyExceptionImpl(STATUS_INVALID_SIGNATURE_SIZE);

        m_signatureSize = sizeInBits;

        if (sizeInBits <= 108)
        {
            m_signatureKeyType = ECC::ECC_54;
            m_signatureKeySize = 54;
        } else
        if (sizeInBits <= 146)
        {
            m_signatureKeyType = ECC::ECC_73;
            m_signatureKeySize = 73;
        } else
        if (sizeInBits <= 158)
        {
            m_signatureKeyType = ECC::ECC_79;
            m_signatureKeySize = 79;
        } else
        if (sizeInBits <= 182)
        {
            m_signatureKeyType = ECC::ECC_91;
            m_signatureKeySize = 91;
        } else
        if (sizeInBits <= 200)
        {
            m_signatureKeyType = ECC::ECC_100;
            m_signatureKeySize = 100;
        } else
        if (sizeInBits <= 240)
        {
            m_signatureKeyType = ECC::ECC_120;
            m_signatureKeySize = 120;
        } else
        if (sizeInBits <= 262)
        {
            m_signatureKeyType = ECC::ECC_131;
            m_signatureKeySize = 131;
        } else
        if (sizeInBits <= 282)
        {
            m_signatureKeyType = ECC::ECC_141;
            m_signatureKeySize = 141;
        } else
        {
            m_signatureKeyType = ECC::ECC_161;
            m_signatureKeySize = 161;
        }
    }

    unsigned GetSignatureSize() const
    {
        return m_signatureSize;
    }

    void LoadXml(const char * xmlTemplate);
    const char * SaveXml(bool savePrivateKey);

    void SetSigningServiceUrl(const char_t * url)
    {
        m_signingServiceUrl = url;
    }

    const char_t * GetSigningServiceUrl()
    {
        return m_signingServiceUrl.c_str();
    }

    void SetSigningServiceTemplateId(int templateId)
    {
        m_signingServiceTemplateId = templateId;
    }

    int GetSigningServiceTemplateId()
    {
        return m_signingServiceTemplateId;
    }

    void SetSigningServiceParameters(const char_t * params)
    {
        m_signingServiceParameters = params;
    }

    void SetPublicKeyCertificate(const char_t * base64Certificate)
    {
        if (m_verificationKey)
        {
            delete m_verificationKey;
            m_verificationKey = NULL;
        }

        if (m_certificate)
        {
            delete m_certificate;
            m_certificate = NULL;
        }

        m_certificate = new Certificate(base64Certificate);
        
        ValidateCertificate();

        unsigned char keyBuf[256]; int keyLen = 256;

        m_certificate->GetPublicKey(keyBuf, &keyLen);
        m_verificationKey = new Key(keyBuf, keyLen);
    }

    const char_t * GetPublicKeyCertificate() const
    {
        if (!m_certificate)
            throw new KeyExceptionImpl(STATUS_GENERIC_ERROR, _T("the key template does not contain a certificate"));

        return m_certificate->ToString();
    }

    void SetPrivateKey(const char_t * base64Key)
    {
        USES_CONVERSION;
        BASE64 base64;
        int keyLen;
        int keyLenSizeT;
        unsigned char * keyData;

        keyData = base64.decode(T2A(base64Key), (unsigned)t_strlen(base64Key), &keyLenSizeT);
        if (keyData == NULL)
            throw new KeyExceptionImpl(STATUS_INVALID_PARAM);

        keyLen = (unsigned)keyLenSizeT;

        SetPrivateKey(keyData, keyLen);
    }

    const char_t * GetPrivateKey() const
    {
        if (!m_signingKey)
            throw new KeyExceptionImpl(STATUS_GENERIC_ERROR, _T("the key template does not contain a private key"));

        return m_signingKey->ToString();
    }

    void GenerateSigningKeyPair()
    {
        if (m_certificate)
            delete m_certificate;

        Key * privateKey = NULL,
            * publicKey = NULL;

        if (!SDKRegistrationImpl::IsRegistered())
        {
            privateKey = new Key(); privateKey->Load(&m_demoPrivateKeys[m_signatureKeyType][1], m_demoPrivateKeys[m_signatureKeyType][0], m_signatureKeyType);
            publicKey = new Key(); publicKey->Load(&m_demoPublicKeys[m_signatureKeyType][1], m_demoPublicKeys[m_signatureKeyType][0], m_signatureKeyType);
        } else
        {
            ECC::GenerateKeyPair(m_signatureKeyType, &privateKey, &publicKey);
        }

        m_certificate = Certificate::Generate(m_signatureSize, privateKey, publicKey, NULL, NULL, _T("http://vending.freegrabapp.com/LicensingService/api/certificates/issuer"), 0);

        ValidateCertificate();

        if (m_signingKey) delete m_signingKey;
        if (m_verificationKey) delete m_verificationKey;

        m_signingKey = privateKey;
        m_verificationKey = publicKey;
    }

    void SetProperty(const char_t * path, const char_t * name, const char_t * value)
    {
        m_properties.SetProperty(path, name, value);
    }

    const char_t * GetProperty(const char_t * path, const char_t * name)
    {
        return m_properties.GetProperty(path, name);
    }

public:
    // general
    int m_version;

    // key format
    int m_numGroups;
    int m_charsPerGroup;
    int m_keyEncoding;
    string_t m_groupSeparator;
    string_t m_header;
    string_t m_footer;
    string_t m_signingServiceUrl;
    int m_signingServiceTemplateId;
    string_t m_signingServiceParameters;

    // key data
    BitStruct * m_keyDataMap;
    BitStruct * m_validationDataMap;

    // key signature
    ECC::Key * m_verificationKey;
    ECC::Key * m_signingKey;

    Certificate * m_certificate;
    bool m_hasPrivateKey;
    
    ECC::KEY_TYPE m_signatureKeyType;
    int m_signatureKeySize;
    int m_signatureSize;
    int m_dataSize;
    int m_validationDataSize;

    // buffer used to hold the xml representation of the template
    std::string m_xmlTemplate;

    PropertyCollection m_properties;

protected:
    static const unsigned char m_demoPublicKeys[10][23];
    static const unsigned char m_demoPrivateKeys[10][23];

private:
    void Init()
    {
        m_version = 3;
        m_dataSize = 0;
        m_validationDataSize = 0;
        m_groupSeparator = _T("-");
        m_keyEncoding = ENCODING_BASE32X;
        m_signingServiceTemplateId = 0;
        m_certificate = NULL;
        m_keyDataMap = new BitStruct();
        m_validationDataMap = new BitStruct();
        m_signingKey = NULL;
        m_verificationKey = NULL;
        m_hasPrivateKey = false;
    }

    void SetPrivateKey(const void * keyBuf, int keyLen)
    {
        ValidatePrivateKey(keyBuf, keyLen);

        if (m_signingKey)
        {
            delete m_signingKey;
        }

        m_signingKey = new Key(keyBuf, keyLen);
        m_hasPrivateKey = true;
    }

    void ValidatePrivateKey(const void * keyBuf, int keyLen)
    {
        if (keyLen <= 2)
        {
            throw new KeyExceptionImpl(STATUS_INVALID_PARAM, T_STR("Invalid private key length"));
        }

        keyLen -= 2;
        keyBuf = (unsigned char *)keyBuf + 2;

        if (!SDKRegistrationImpl::IsRegistered())
        {
            if (keyLen != m_demoPrivateKeys[m_signatureKeyType][0] || memcmp(keyBuf, &m_demoPrivateKeys[m_signatureKeyType][1], keyLen) != 0)
            {
                delete m_signingKey; m_signingKey = NULL;
                throw new KeyExceptionImpl(STATUS_INVALID_PARAM, T_STR("Invalid public/private key. The SDK is in DEMO mode because a SDK license key was not set. In this mode, only certain public/private keys can be set. Did you set a valid SDK license key via FreeGrabApp::Licensing::SDKRegistration::SetLicenseKey() ?"));
            }
        }
    }

    void ValidateCertificate()
    {
        Certificate * validationCertificate = new Certificate(_T("AE4vDQELjMGgCJew5QSVMBWSAN2PaBa0zzYpjSYe0rcu88lYggE="));

        if (!Certificate::VerifySignature(m_certificate, validationCertificate))
        {
            delete validationCertificate;
            throw new KeyExceptionImpl(STATUS_GENERIC_ERROR, _T("certificate signature is not valid"));
        }

        delete validationCertificate;
    }

    static const char_t * m_certificateAuthorityUrl;
};

#endif
