#ifndef __CERTIFICATE_H
#define __CERTIFICATE_H

#include "ecc.h"
#include "bitstruct.h"
#include "except.h"
#include "base64.h"
#include "download.h"
#include "sdkregistration.h"

#include <time.h>

using namespace ECC;

class Certificate
{
public:
	Certificate (const char_t * base64Certificate)
	{
		USES_CONVERSION;
		BASE64 base64;
		int len;

		Init();

		unsigned char * buf = base64.decode(T2A(base64Certificate), t_strlen(base64Certificate), &len);
		if (buf)
		{
			Load(buf, len);
			free(buf);
		} else
			throw new KeyExceptionImpl(STATUS_GENERIC_ERROR, _T("invalid base64 encoding for certificate"));
	}

	Certificate(void * buf, int len)
	{
		Init();
		Load(buf, len);
	}

	void * GetBuffer(int * len)
	{
		int size;
		void * buf = m_certBits->GetBuffer(&size);
		size = (size + 7) >> 3;
		*len = size;
		return buf;
	}

	const char_t * ToString()
	{
		USES_CONVERSION;
		BASE64 base64;
		int len;
		static string_t base64cert;
		unsigned char * buf = (unsigned char *)m_certBits->GetBuffer(&len); 
		len = (len + 7) >> 3;

		char * base64Buf = base64.encode(buf, len, true);
		if (!base64Buf)
			throw new KeyExceptionImpl(STATUS_GENERIC_ERROR, _T("could not encode buffer to base64"));

		base64cert = A2T(base64Buf);
		free(base64Buf);

		return base64cert.c_str();
	}

	~Certificate()
	{
		if (m_certBits)
			delete m_certBits;
	}

	/*
	static Certificate * Generate(int signatureSize, time_t expirationDate = 0)
	{
		return Generate(signatureSize, (ECC::Key *)NULL, NULL, NULL, NULL, expirationDate);
	}

	static Certificate * Generate(int signatureSize, Certificate * signingCertificate, time_t expirationDate = 0)
	{
		return Generate(signatureSize, NULL, NULL, signingCertificate, NULL, expirationDate);
	}

	static Certificate * Generate(int signatureSize, const char_t * certificateAuthorityUrl, time_t expirationDate = 0)
	{
		return Generate(signatureSize, (ECC::Key *)NULL, NULL, NULL, certificateAuthorityUrl, expirationDate);
	}
	*/

	static bool VerifySignature(Certificate * cert, Certificate * validationCert)
	{
		unsigned char rawPublicKey[256]; int rawPublicKeyLen = 256;

		if (validationCert == NULL || cert->m_certBits->GetInt(_T("IsSelfSigned")) != 0)
			cert->m_certBits->Get(_T("PublicKey"), rawPublicKey, &rawPublicKeyLen);
		else
			validationCert->m_certBits->Get(_T("PublicKey"), rawPublicKey, &rawPublicKeyLen);

		Key key(rawPublicKey, rawPublicKeyLen);

		Verifier verifier;

        verifier.SetPublicKey(&key);

		unsigned char * certBuffer = (unsigned char *)cert->m_certBits->GetBuffer();

		int signedPartLen = 4 + cert->m_certBits->GetInt(_T("PublicKeyLen")),
			signatureOffset = 2 + signedPartLen,
			signatureBitLen = cert->m_certBits->GetInt(_T("SignatureSizeBits"));
			
        if (!verifier.Verify(certBuffer, signedPartLen, certBuffer + signatureOffset, (signatureBitLen + 7) >> 3, signatureBitLen))
            return false;

		return true;
	}

	void GetPublicKey(void * buf, int * len)
	{
		m_certBits->Get(_T("PublicKey"), buf, len);
	}

	static Certificate * Generate(int signatureSize, const char_t * base64PrivateKey, const char_t * base64PublicKey, const char_t * base64SigningPrivateKey, const char_t * base64SigningCertificate, const char_t * certificateAuthorityUrl, time_t expirationDate)
	{
		Key * privateKey = NULL,
			* publicKey = NULL,
			* privateSigningKey;

		Certificate * signingCertificate = NULL;
		Certificate * result;

		try {
			if (base64PublicKey != NULL)
				publicKey = new Key(base64PublicKey);

			if (base64PrivateKey != NULL)
				privateKey = new Key(base64PrivateKey);

			if (base64SigningPrivateKey != NULL)
				privateSigningKey = new Key(base64SigningPrivateKey);

			if (base64SigningCertificate != NULL)
				signingCertificate = new Certificate(base64SigningCertificate);

			result = Generate(signatureSize, privateKey, publicKey, privateSigningKey, signingCertificate, certificateAuthorityUrl, expirationDate);
		} catch (...)
		{
			if (privateKey) delete privateKey;
			if (publicKey) delete publicKey;
			if (privateSigningKey) delete privateSigningKey;
			if (signingCertificate) delete signingCertificate;

			throw;
		}

		if (privateKey) delete privateKey;
		if (publicKey) delete publicKey;
		if (privateSigningKey) delete privateSigningKey;
		if (signingCertificate) delete signingCertificate;

		return result;
	}
	
	static Certificate * Generate(int signatureSize, Key * privateKey, Key * publicKey, Key * signingPrivateKey, Certificate * signingCertificate, const char_t * certificateAuthorityUrl, time_t expirationDate)
	{
		USES_CONVERSION;
    
		if (publicKey == NULL)
		{
			throw new KeyExceptionImpl(STATUS_GENERIC_ERROR, _T("public key must be provided for the certificate"));
		}

        Key * signingKey = NULL;
        bool selfSigned;

        if (signingPrivateKey == NULL && certificateAuthorityUrl == NULL)
        {
            selfSigned = true;
            signingKey = privateKey;
        }
        else
        {
            selfSigned = false;
            if (signingPrivateKey != NULL)
				signingKey = signingPrivateKey;
			// else use certificate authority url for signing
        }

		unsigned char rawPublicKey[256]; int rawPublicKeyLen = 256;
        publicKey->Store(rawPublicKey, &rawPublicKeyLen);

		// unsigned char rawPrivateKey[256]; int rawPrivateKeyLen = 256;
        // signingKey->Store(rawPrivateKey, &rawPrivateKeyLen);

		int certBitsLen = 4 * 8 + rawPublicKeyLen * 8;
        BitStruct * certBits = new BitStruct(certBitsLen);

        certBits->BeginFieldOffset();

        certBits->AddField(_T("Version"), BitStruct::INTEGER, 4);
        certBits->AddField(_T("IsSelfSigned"), BitStruct::INTEGER, 1);
        certBits->AddField(_T("ReservedFlags"), BitStruct::INTEGER, 3);
        certBits->AddField(_T("ExpirationDate"), BitStruct::DATE16, 16);
        certBits->AddField(_T("PublicKeyLen"), BitStruct::INTEGER, 8);
        certBits->AddField(_T("PublicKey"), BitStruct::RAW, rawPublicKeyLen * 8);

        certBits->Set(_T("Version"), 0);
        certBits->Set(_T("IsSelfSigned"), (selfSigned) ? 1 : 0);
        certBits->Set(_T("ReservedFlags"), 0);

		expirationDate = time(NULL);

		struct tm * t = gmtime(&expirationDate);

        certBits->Set(_T("ExpirationDate"), t->tm_year + 1900, t->tm_mon, t->tm_mday);
        certBits->Set(_T("PublicKeyLen"), rawPublicKeyLen);
        certBits->Set(_T("PublicKey"), rawPublicKey, rawPublicKeyLen);

        unsigned char certSignature[256];
        int sigSizeBytes = 256;
		int sigSizeBits;

        if (signingKey != NULL)
        {
            Signer signer;
            signer.SetPrivateKey(signingKey);
            signer.Sign(certBits->GetBuffer(), (certBitsLen + 7) >> 3, certSignature, &sigSizeBytes, &sigSizeBits);
        }
        else
        {
			if (!certificateAuthorityUrl)
				throw new KeyExceptionImpl(STATUS_GENERIC_ERROR, _T("no means for certificate signing were provided"));

			string_t certificateAuthorityRequest(certificateAuthorityUrl);
			certificateAuthorityRequest.append(_T("?csr="));
			BASE64 base64;
			int len = 1024;
			char buffer[1024];

			char * base64csr = base64.encode((unsigned char *)certBits->GetBuffer(), (certBits->GetSize() + 7) >> 3, true);
			string_t escapedBase64csr(A2T(base64csr));

			StrReplace(escapedBase64csr, _T("+"), _T("%2B"));
			StrReplace(escapedBase64csr, _T("/"), _T("%2F"));
			StrReplace(escapedBase64csr, _T("="), _T("%3D"));

			certificateAuthorityRequest.append(escapedBase64csr);

			free(base64csr);

			const char_t * sdkLicenseKey = SDKRegistrationImpl::GetLicenseKey();
			
			if (sdkLicenseKey && _tcslen(sdkLicenseKey) > 0)
			{
				certificateAuthorityRequest.append(_T("&sdklicensekey="));
				certificateAuthorityRequest.append(sdkLicenseKey);
			}

			UrlDownloadToString(certificateAuthorityRequest.c_str(), buffer, &len);

			unsigned char * rawBuffer = base64.decode(buffer, len, &len);

			Certificate * issuedCert = new Certificate(rawBuffer, len);
		
			sigSizeBits = issuedCert->m_certBits->GetInt(_T("SignatureSizeBits"));
			sigSizeBytes = (sigSizeBits + 7) >> 3; 
			issuedCert->m_certBits->Get(_T("Signature"), certSignature, &sigSizeBytes);

			memcpy(certBits->GetBuffer(), rawBuffer, 4 + rawPublicKeyLen);

			free(rawBuffer);

			delete issuedCert;
        }

		int certBufLen = ((certBitsLen + 7) >> 3)   // header + public key
                        + 2                         // signature size in bits
                        + ((sigSizeBits + 7) >> 3); // signature

        unsigned char * certBuf = new unsigned char[certBufLen];

        memcpy(certBuf, certBits->GetBuffer(), (certBitsLen + 7) >> 3);
        
        certBits->Attach(certBuf, certBufLen * 8);

        certBits->AddField(_T("SignatureSizeBits"), BitStruct::INTEGER, 16);
        certBits->AddField(_T("Signature"), BitStruct::RAW, ((sigSizeBits + 7) >> 3) << 3);

        certBits->Set(_T("SignatureSizeBits"), sigSizeBits);
        certBits->Set(_T("Signature"), certSignature, (sigSizeBits + 7) >> 3);

        Certificate * cert = new Certificate();
        cert->m_certBits = certBits;

        return cert;
	}

	static const char_t * Sign(const char_t * csr, const char_t * privateKey, const char_t * signingPrivateKey, const char_t * signingCertificate, const char_t * certificateAuthorityUrl, int expYear, int expMonth, int expDay)
	{
		Certificate * _csr = new Certificate(csr);
		Key * _privateKey = (privateKey) ? new Key(privateKey) : NULL;
		Key * _signingPrivateKey = (signingPrivateKey) ? new Key(signingPrivateKey) : NULL;
		Certificate * _signingCertificate = (signingCertificate) ? new Certificate(signingCertificate) : NULL;
		struct tm _expTm;
		time_t _expTime = 0;

		if (expYear >= 1900 && expMonth >= 1 && expMonth <= 12 && expDay >= 1 && expDay <= 31)
		{
			memset(&_expTm, 0, sizeof(_expTm));

			_expTm.tm_year = expYear - 1900;
			_expTm.tm_mon = expMonth - 1;
			_expTm.tm_mday = expDay;

			_expTime = mktime(&_expTm);
		}
		else
			if (expYear != 0 || expMonth != 0 || expDay != 0)
				throw new KeyExceptionImpl(STATUS_GENERIC_ERROR, _T("Invalid certificate expiration date"));

		Certificate * _signedCert = Sign(_csr, _privateKey, _signingPrivateKey, _signingCertificate, certificateAuthorityUrl, 0);

		if (_csr) delete _csr;
		if (_privateKey) delete _privateKey;
		if (_signingPrivateKey) delete _signingPrivateKey;
		if (_signingCertificate) delete _signingCertificate;

		const char_t * signedCert = _signedCert->ToString();

		delete _signedCert;

		return signedCert;
	}

	static Certificate * Sign(Certificate * csr, Key * privateKey, Key * signingPrivateKey, Certificate * signingCertificate, const char_t * certificateAuthorityUrl, time_t expirationDate)
	{
		unsigned char buf[1024];
		int len = 1024;
		Key * publicKey;

		csr->GetPublicKey(buf, &len);
		publicKey = new Key(buf, len);

		Certificate * cert = Certificate::Generate(0, privateKey, publicKey, signingPrivateKey, signingCertificate, certificateAuthorityUrl, expirationDate);
	
		delete publicKey;

		return cert;
	}

private:
	Certificate()
	{
		Init();
	}

	void Load(void * buf, int len)
	{
		if (m_certBits)
			delete m_certBits;

        m_certBits = new BitStruct(len * 8);

		memcpy(m_certBits->GetBuffer(), buf, len);

        m_certBits->BeginFieldOffset();

        m_certBits->AddField(_T("Version"), BitStruct::INTEGER, 4);
        m_certBits->AddField(_T("IsSelfSigned"), BitStruct::INTEGER, 1);
        m_certBits->AddField(_T("ReservedFlags"), BitStruct::INTEGER, 3);
        m_certBits->AddField(_T("ExpirationDate"), BitStruct::DATE16, 16);
        m_certBits->AddField(_T("PublicKeyLen"), BitStruct::INTEGER, 8);        
        m_certBits->AddField(_T("PublicKey"), BitStruct::RAW, m_certBits->GetInt(_T("PublicKeyLen")) * 8);

        int signedPartLen = (m_certBits->GetCurrentFieldOffset() + 7) >> 3;

		if (len <= signedPartLen)
			return;

        m_certBits->AddField(_T("SignatureSizeBits"), BitStruct::INTEGER, 16);

        int signatureBitLen = m_certBits->GetInt(_T("SignatureSizeBits"));
        int signatureLen = (signatureBitLen + 7) >> 3;

        int signatureOffset = m_certBits->GetCurrentFieldOffset() >> 3;

        m_certBits->AddField(_T("Signature"), BitStruct::RAW, signatureLen << 3);
	}

	static ECC::KEY_TYPE GetKeyType(int sigSize)
	{
		if (sigSize < 76 || sigSize > 322)
			throw new KeyExceptionImpl(STATUS_INVALID_SIGNATURE_SIZE);

		ECC::KEY_TYPE keyType;
		int keySize;

		if (sigSize <= 108)
		{
			keyType = ECC::ECC_54;
			keySize = 54;
		} else
		if (sigSize <= 146)
		{
			keyType = ECC::ECC_73;
			keySize = 73;
		} else
		if (sigSize <= 158)
		{
			keyType = ECC::ECC_79;
			keySize = 79;
		} else
		if (sigSize <= 182)
		{
			keyType = ECC::ECC_91;
			keySize = 91;
		} else
		if (sigSize <= 200)
		{
			keyType = ECC::ECC_100;
			keySize = 100;
		} else
		if (sigSize <= 240)
		{
			keyType = ECC::ECC_120;
			keySize = 120;
		} else
		if (sigSize <= 262)
		{
			keyType = ECC::ECC_131;
			keySize = 131;
		} else
		if (sigSize <= 282)
		{
			keyType = ECC::ECC_141;
			keySize = 141;
		} else
		{
			keyType = ECC::ECC_161;
			keySize = 161;
		}

		return keyType;
	}

	static void StrReplace(string_t& str, const string_t& oldStr, const string_t& newStr)
	{
	  size_t pos = 0;
	  while((pos = str.find(oldStr, pos)) != std::string::npos)
	  {
		 str.replace(pos, oldStr.length(), newStr);
		 pos += newStr.length();
	  }
	}

	void Init()
	{
		m_certBits = NULL;
	}

	BitStruct * m_certBits;
};

#endif
