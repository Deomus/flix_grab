#ifndef __VALIDATOR_H
#define __VALIDATOR_H

#include "bitstream.h"
#include "base64.h"
#include "base32.h"

#ifdef _WIN32
#include <tchar.h>
#endif

class KeyValidatorImpl {
public:
	KeyValidatorImpl()
	{
		Init();
	}

	KeyValidatorImpl(const LicenseTemplateImpl * templ, const char_t * key = NULL)
	{
		Init();

		SetKeyTemplate(templ);
		
		if (key)
			SetKey(key);
	}

	~KeyValidatorImpl()
	{
		if (m_rawKey)
		{
			free(m_rawKey->GetBuffer());
			delete m_rawKey;
		}

		if (m_validationData)
			delete m_validationData;
	}

	void SetKeyTemplate(const LicenseTemplateImpl * templ)
	{
		m_keyTemplate = templ;

		if (m_validationData)
			delete m_validationData;

		m_validationData = new BitStream(m_keyTemplate->m_validationDataSize);
	}

	void SetValidationData(const char_t * fieldName, const void * buf, int len)
	{
		int sizeInBits;
		void * buffer = m_validationData->GetBuffer(&sizeInBits);
		m_keyTemplate->m_validationDataMap->Attach(buffer, sizeInBits);
		m_keyTemplate->m_validationDataMap->Set(fieldName, buf, len);
	}

	void SetValidationData(const char_t * fieldName, const char_t * data)
	{
		int sizeInBits;
		void * buffer = m_validationData->GetBuffer(&sizeInBits);
		m_keyTemplate->m_validationDataMap->Attach(buffer, sizeInBits);
		m_keyTemplate->m_validationDataMap->Set(fieldName, data);
	}

	void SetValidationData(const char_t * fieldName, int data)
	{
		int sizeInBits;
		void * buffer = m_validationData->GetBuffer(&sizeInBits);
		m_keyTemplate->m_validationDataMap->Attach(buffer, sizeInBits);
		m_keyTemplate->m_validationDataMap->Set(fieldName, data);
	}

	void SetKey(const char_t * licKey)
	{
		if (!licKey)
			throw new KeyExceptionImpl(STATUS_INVALID_PARAM, _T("invalid parameter"));

		USES_CONVERSION;
		string_t key(licKey);

		if (key.length() < m_keyTemplate->GetCharactersPerGroup() * m_keyTemplate->GetNumberOfGroups() +
			                      _tcslen(m_keyTemplate->GetHeader()) +
								  _tcslen(m_keyTemplate->GetFooter()) + 
								  _tcslen(m_keyTemplate->GetGroupSeparator()) * (m_keyTemplate->GetNumberOfGroups() - 1))
		{
			throw new KeyExceptionImpl(STATUS_INVALID_LICENSE_KEY, _T("invalid license key (too short)"));
		}

		// remove header and footer (if any)
		if (!m_keyTemplate->m_header.empty())
		{
			key.erase(0, m_keyTemplate->m_header.length() + 2);
		}

		if (!m_keyTemplate->m_footer.empty())
		{
			key.erase(key.length() - m_keyTemplate->m_footer.length() - 2, m_keyTemplate->m_footer.length() + 2);
		}

		// ungroup license key
		for (int i = 0, erasePos = 0; i < m_keyTemplate->m_numGroups - 1; i++)
		{
			erasePos += m_keyTemplate->m_charsPerGroup;
			key.erase(erasePos, m_keyTemplate->m_groupSeparator.length());
		}

		// decode license key
		switch ( m_keyTemplate->m_keyEncoding )
		{
		case ENCODING_BASE32X:
			{
				BASE32 base32;
				int padLen;
				int len = base32.encode_pad_length(((int)key.length() * 5 + 7) >> 3, &padLen);

				if (len > (int)key.length())
					key.append(len - key.length(), 'A');

				if (padLen)
					key.append(padLen,'=');

				unsigned char * keyBuf = base32.decode( T2A(key.c_str()), (int)key.length(), &len );

				if (!keyBuf)
					throw new KeyExceptionImpl(STATUS_INVALID_LICENSE_KEY);

				// reverse last byte
				keyBuf[ len - 1 ] = (unsigned char)(((keyBuf[ len - 1 ] * 0x0802LU & 0x22110LU) | (keyBuf[ len - 1 ] * 0x8020LU & 0x88440LU)) * 0x10101LU >> 16);

				if (m_rawKey)
				{
					free(m_rawKey->GetBuffer());
					delete m_rawKey;
				}

				m_rawKey = new BitStream(keyBuf, m_keyTemplate->m_keyEncoding * m_keyTemplate->m_charsPerGroup * m_keyTemplate->m_numGroups);
			}
			break;

		case ENCODING_BASE64X:
			{
				BASE64 base64;
				int padLen;
				int len = base64.encode_pad_length(((int)key.length() * 6 + 7) >> 3, &padLen);

				if (len > (int)key.length())
					key.append(len - key.length(), 'A');

				if (padLen)
					key.append(padLen,'=');

				unsigned char * keyBuf = base64.decode( T2A(key.c_str()), (int)key.length(), &len );
				if (!keyBuf)
					throw new KeyExceptionImpl(STATUS_INVALID_LICENSE_KEY);

				// reverse last byte
				keyBuf[ len - 1 ] = (unsigned char)(((keyBuf[ len - 1 ] * 0x0802LU & 0x22110LU) | (keyBuf[ len - 1 ] * 0x8020LU & 0x88440LU)) * 0x10101LU >> 16);
				m_rawKey->Attach( keyBuf, m_keyTemplate->m_keyEncoding * m_keyTemplate->m_charsPerGroup * m_keyTemplate->m_numGroups );
			}
			break;

		default:
			throw new KeyExceptionImpl(STATUS_INVALID_KEY_ENCODING);
		}
	}

	bool IsKeyValid()
	{
		ECC::Verifier verifier;
		BitStream signedData,
				  signature;

		signedData.Create(m_keyTemplate->m_dataSize + m_keyTemplate->m_validationDataSize);
		signedData.Clear();
		if (m_keyTemplate->m_dataSize)
			signedData.Write(m_rawKey->GetBuffer(), m_keyTemplate->m_dataSize);
		if (m_keyTemplate->m_validationDataSize)
			signedData.Write(m_validationData->GetBuffer(), m_keyTemplate->m_validationDataSize);

		signature.Create(m_keyTemplate->m_signatureSize);
		m_rawKey->Seek(m_keyTemplate->m_dataSize);
		m_rawKey->Read(signature.GetBuffer(), m_keyTemplate->m_signatureSize);
		signature.ReleaseBuffer(m_keyTemplate->m_signatureSize);
		signature.Seek(m_keyTemplate->m_signatureSize);
		signature.ZeroPadToNextByte();

		signature.ReleaseBuffer(m_keyTemplate->m_signatureSize);

		// we use a different algorithm than ECDSA when the signature size must be smaller than twice the key size
		if (m_keyTemplate->m_signatureSize < (m_keyTemplate->m_signatureKeySize << 1))
			verifier.SetHashSize(m_keyTemplate->m_signatureSize - m_keyTemplate->m_signatureKeySize);
		else
			verifier.SetHashSize(0);

		verifier.SetPublicKey(m_keyTemplate->m_verificationKey);

		return verifier.Verify(signedData.GetBuffer(), (signedData.GetSize() + 7) >> 3, signature.GetBuffer(), (signature.GetSize() + 7) >> 3 , m_keyTemplate->m_signatureSize);
	}

	void QueryKeyData(const char_t * fieldName, void * buf, int * len)
	{
		int sizeInBits;
		void * buffer = m_rawKey->GetBuffer(&sizeInBits);
		m_keyTemplate->m_keyDataMap->Attach(buffer, sizeInBits);
		m_keyTemplate->m_keyDataMap->Get(fieldName, buf, len);
	}

	int QueryIntKeyData(const char_t * fieldName)
	{
		int sizeInBits;
		void * buffer = m_rawKey->GetBuffer(&sizeInBits);
		m_keyTemplate->m_keyDataMap->Attach(buffer, sizeInBits);
		return m_keyTemplate->m_keyDataMap->GetInt(fieldName);
	}

	void QueryDateKeyData(const char_t * fieldName, int * year, int * month, int * day)
	{
		int sizeInBits;
		void * buffer = m_rawKey->GetBuffer(&sizeInBits);
		m_keyTemplate->m_keyDataMap->Attach(buffer, sizeInBits);
		m_keyTemplate->m_keyDataMap->GetDate(fieldName, year, month, day);
	}

	void QueryValidationData(const char_t * fieldName, void * buf, int * len)
	{
		int sizeInBits;
		void * buffer = m_validationData->GetBuffer(&sizeInBits);
		m_keyTemplate->m_validationDataMap->Attach(buffer, sizeInBits);
		return m_keyTemplate->m_validationDataMap->Get(fieldName, buf, len);
	}

public:
	
	BitStream * m_rawKey;
	BitStream * m_validationData;
	const LicenseTemplateImpl * m_keyTemplate;

private:

	void Init()
	{
		m_rawKey = NULL;
		m_validationData = NULL;
		m_keyTemplate = NULL;
	}
};

#endif
