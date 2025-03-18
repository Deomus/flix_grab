#ifndef __LICENSINGCLIENT_H
#define __LICENSINGCLIENT_H

#include "except.h"
#include "template.h"
#include "generator.h"
#include "validator.h"
#include "ntpclient.h"
#include "uniconv.h"

#include <time.h>

// these methods are compiler workarounds to eliminate circular references.
// They only call the corresponding LicensingClient::... methods 
// GCC complains of cross-references between LicensingClientImpl and LicensingClientT
static const wchar_t * GetCurrentHardwareIdW(void *);
static const char * GetCurrentHardwareIdA(void *);
static bool MatchCurrentHardwareIdW(void *, const wchar_t *);
static bool MatchCurrentHardwareIdA(void *, const char *);

class LicensingClientImpl
{
public:
	LicensingClientImpl(void * licensingClient, bool usesWideChar)
	{
		licenseKeyValidationData = NULL;
		licenseKeyValidationDataLen = 0;
		productId = -1;
		parentLicensingClient = licensingClient;
		parentUsesWideChar = usesWideChar;
		timeValidationMethod = PREFER_INTERNET_TIME;
		// GetCurrentHardwareIdCallback = GetCurrentHardwareId;
		// MatchCurrentHardwareIdCallback = MatchCurrentHardwareId;
	}

	~LicensingClientImpl()
	{
		if (licenseKeyValidationData)
			free(licenseKeyValidationData);
	}

	void SetLicenseTemplate(const LicenseTemplateImpl & tmpl)
	{
		// we do not set all the template properties, only set what we will need later

		licenseTemplate.SetCharactersPerGroup(tmpl.GetCharactersPerGroup());
		licenseTemplate.SetNumberOfGroups(tmpl.GetNumberOfGroups());
		licenseTemplate.SetGroupSeparator(tmpl.GetGroupSeparator());
		//licenseTemplate.SetDataSize(t.GetDataSize());
		licenseTemplate.SetSignatureSize(tmpl.GetSignatureSize());
		licenseTemplate.SetEncoding(tmpl.GetEncoding());
		licenseTemplate.SetPublicKeyCertificate(tmpl.GetPublicKeyCertificate());
	}

	void SetLicenseKey(const char_t * key)
	{
		licenseKey = key;
	}

	void SetActivationKey(const char_t * key)
	{
		activationKey = key;
	}

	const char_t * GetActivationKey()
	{
		return activationKey.c_str();
	}

	void SetHardwareId(const char_t * hwid)
	{
		hardwareId = hwid;
	}

	const char_t * GetHardwareId()
	{
		return hardwareId.c_str();
	}

	void SetProductId(int id)
	{
		productId = id;
	}

	int GetProductId()
	{
		return productId;
	}

	void SetLicenseKeyValidationData(void * data, unsigned len)
	{
		if (licenseKeyValidationData)
			free(licenseKeyValidationData);

		if ((licenseKeyValidationData = malloc(len)) == NULL)
			throw new KeyExceptionImpl(STATUS_GENERIC_ERROR, _T("insufficient memory"));

		memcpy(licenseKeyValidationData, data, len);

		licenseKeyValidationDataLen = len;
	}

	LicenseTemplateImpl * CreateActivationTemplate()
	{
		LicenseTemplateImpl * tmpl = new LicenseTemplateImpl();

		tmpl->SetCharactersPerGroup(licenseTemplate.GetCharactersPerGroup());
		tmpl->SetNumberOfGroups(licenseTemplate.GetNumberOfGroups());
		tmpl->SetGroupSeparator(licenseTemplate.GetGroupSeparator());
		//activationKeyTemplate.SetDataSize(t.GetDataSize());
		tmpl->SetSignatureSize(licenseTemplate.GetSignatureSize());
		tmpl->SetEncoding(licenseTemplate.GetEncoding());
		tmpl->SetPublicKeyCertificate(licenseTemplate.GetPublicKeyCertificate());

		int keyLen = tmpl->GetNumberOfGroups() * tmpl->GetCharactersPerGroup()
			+ (tmpl->GetNumberOfGroups() - 1) * _tcslen(tmpl->GetGroupSeparator());
		
		int hwidLen = 29;

		if (hardwareId.length() > 0)
			hwidLen = hardwareId.length();

		tmpl->SetDataSize(tmpl->GetCharactersPerGroup() * tmpl->GetNumberOfGroups() * tmpl->GetEncoding() - tmpl->GetSignatureSize());
		tmpl->SetValidationDataSize((keyLen + hwidLen + licenseKeyValidationDataLen) * 8);

		tmpl->AddDataField(_T("ExpirationDate"), FIELD_TYPE_RAW, 16, 0);
		tmpl->AddValidationField(_T("LicenseKey"), FIELD_TYPE_STRING, keyLen * 8, 0);
		tmpl->AddValidationField(_T("HardwareId"), FIELD_TYPE_STRING, hwidLen * 8, keyLen * 8);

		if (licenseKeyValidationData != NULL)
		{
			tmpl->AddValidationField(_T("LicenseKeyValidationData"), FIELD_TYPE_RAW, licenseKeyValidationDataLen * 8, (keyLen + hwidLen) * 8);
		}

		return tmpl;
	}

	bool ValidateHardwareId()
	{
		if (!MatchCurrentHardwareIdCallback(hardwareId.c_str()))
		{
			activationStatus = STATUS_INVALID_HARDWARE_ID;
			return false;
		}

		return true;
	}

	bool ValidateActivationKey(LicenseTemplateImpl * activationTemplate = NULL, bool validateExpirationDate = true)
	{
		bool ownActivationTemplate = false;

		try {
			if (!activationTemplate)
			{
				activationTemplate = CreateActivationTemplate();
				ownActivationTemplate = true;
			}

			KeyValidatorImpl validator(activationTemplate);

			validator.SetValidationData(_T("LicenseKey"), licenseKey.c_str());
			validator.SetValidationData(_T("HardwareId"), hardwareId.c_str());

			if (licenseKeyValidationData)
				validator.SetValidationData(_T("LicenseKeyValidationData"), licenseKeyValidationData, licenseKeyValidationDataLen);

			try {
				validator.SetKey(activationKey.c_str());
			}
			catch (Exception * ex)
			{
				ex->Destroy();
				activationStatus = STATUS_INVALID_ACTIVATION_KEY;
				if (ownActivationTemplate) delete activationTemplate;
				return false;
			}

			if (!validator.IsKeyValid())
			{
				activationStatus = STATUS_INVALID_ACTIVATION_KEY;
				if (ownActivationTemplate) delete activationTemplate;
				return false;
			}

			unsigned char rawKeyData[2];
			int rawKeyDataBits = 16;

			validator.QueryKeyData(_T("ExpirationDate"), rawKeyData, &rawKeyDataBits);

			unsigned short keyData = (unsigned short)(((unsigned short)rawKeyData[0]) << 8 | rawKeyData[1]);

			memset(&licenseExpirationTime, 0, sizeof(licenseExpirationTime));

			if (keyData != 0)
			{
				licenseExpirationTime.tm_year = (2000 + (keyData >> 9)) - 1900;
				licenseExpirationTime.tm_mon = ((keyData & 0x01FF) >> 5) - 1;
				licenseExpirationTime.tm_mday = keyData & 0x001F;

				if (!validateExpirationDate)
				{
					if (ownActivationTemplate) delete activationTemplate;
					return true;
				}

				time_t currentTime;

				if (timeValidationMethod != USE_LOCAL_TIME)
				{
					currentTime = NTPClient::GetCurrentTimeUTC();

					if (currentTime == 0) // if couldn't obtain internet time
					{
						if (timeValidationMethod == USE_INTERNET_TIME) // internet time was mandatory
						{
							activationStatus = STATUS_NET_ERROR;
							if (ownActivationTemplate) delete activationTemplate;
							return false;
						}

						// fallback to local time
						currentTime = time(NULL);
					}
				}
				else
					currentTime = time(NULL);

				if (difftime(mktime(&licenseExpirationTime), currentTime) < 0)
				{
					activationStatus = STATUS_LICENSE_EXPIRED;
					if (ownActivationTemplate) delete activationTemplate;
					return false;
				}
			}

			if (ownActivationTemplate) delete activationTemplate;
			return true;
		}
		catch (...)
		{
			if (ownActivationTemplate && activationTemplate)
				delete activationTemplate;

			throw;
		}
	}

	bool IsLicenseValid()
	{
		activationStatus = STATUS_GENERIC_ERROR;

		try 
		{
			if (ValidateHardwareId() && ValidateActivationKey())
			{
				activationStatus = STATUS_SUCCESS;
				return true;
			}
			else
				return false;
		}
		catch (Exception * ex)
		{
			ex->Destroy();

			activationStatus = STATUS_GENERIC_ERROR;
			return false;
		}
		catch (...)
		{
			activationStatus = STATUS_GENERIC_ERROR;
			return false;
		}
	}

	void AcquireLicense()
	{
		LicenseTemplateImpl * activationTemplate = NULL;

		try {
			if (hardwareId.empty())
				hardwareId = GetCurrentHardwareIdCallback();

			activationTemplate = CreateActivationTemplate();

			activationTemplate->SetSigningServiceUrl(licensingServiceUrl.c_str());

			if (productId != -1)
			{
				TCHAR params[64] = _T("ProductId=");
				_stprintf(params + sizeof("ProductId"), _T("%d"), productId);
				activationTemplate->SetSigningServiceParameters(params);
			}

			KeyGeneratorImpl generator(activationTemplate);

			generator.SetKeyData(_T("ExpirationDate"), (int)0);

			generator.SetValidationData(_T("LicenseKey"), licenseKey.c_str());

			generator.SetValidationData(_T("HardwareId"), hardwareId.c_str());

			if (licenseKeyValidationData != NULL)
			{
				generator.SetValidationData(_T("LicenseKeyValidationData"), licenseKeyValidationData, licenseKeyValidationDataLen * 8);
			}

			activationKey = generator.GenerateKey();

			if (ValidateActivationKey(activationTemplate, false))
			{
				activationStatus = STATUS_SUCCESS;
			}
			else
				throw new KeyExceptionImpl(STATUS_GENERIC_ERROR, _T("internal error - invalid returned activation key"));
		}
		catch (...)
		{
			if (activationTemplate)
				delete activationTemplate;

			throw;
		}
	}

	void SetLicensingServiceUrl(const char_t * url)
	{
		licensingServiceUrl = url;
	}

	int GetLicenseActivationStatus()
	{
		return activationStatus;
	}

	void GetLicenseExpirationDate(int * year, int * month, int * day)
	{
		if (licenseExpirationTime.tm_year > 0)
		{
			*year = 1900 + licenseExpirationTime.tm_year;
			*month = 1 + licenseExpirationTime.tm_mon;
			*day = licenseExpirationTime.tm_mday;
		} else
		{
			*year = 0;
			*month = 0;
			*day = 0;
		}
	}

	const char_t * GetCurrentHardwareIdCallback()
	{
#ifdef _UNICODE
		if (parentUsesWideChar) 
			return GetCurrentHardwareIdW(parentLicensingClient);
		else
		{
			USES_CONVERSION;
			static std::wstring hwid = A2W(GetCurrentHardwareIdA(parentLicensingClient));
			return hwid.c_str();
		}
#else
		if (parentUsesWideChar) 
		{
			USES_CONVERSION;
			static std::string hwid = W2A(GetCurrentHardwareIdW(parentLicensingClient));
			return hwid.c_str();
		}
		else
		{
			return GetCurrentHardwareIdA(parentLicensingClient);
		}
#endif
	}

	bool MatchCurrentHardwareIdCallback(const char_t * hwid)
	{
		USES_CONVERSION;

#ifdef _UNICODE
		if (parentUsesWideChar) 
			return MatchCurrentHardwareIdW(parentLicensingClient, hwid);
		else
		{
			USES_CONVERSION;
			return MatchCurrentHardwareIdA(parentLicensingClient, W2A(hwid));
		}
#else
		if (parentUsesWideChar) 
		{
			return MatchCurrentHardwareIdW(parentLicensingClient, A2W(hwid));
		}
		else
			return MatchCurrentHardwareIdA(parentLicensingClient, hwid);
#endif
	}

	static const char_t * GetCurrentHardwareId()
	{
		return KeyHelper::GetCurrentHardwareId();
	}

	static bool MatchCurrentHardwareId(const char_t * hwid)
	{
		return KeyHelper::MatchCurrentHardwareId(hwid);
	}

	void SetTimeValidationMethod(int method)
	{
		timeValidationMethod = method;
	}

private:
	struct tm licenseExpirationTime;
	string_t licenseKey;
	string_t activationKey;
	string_t hardwareId;
	string_t licensingServiceUrl;
	LicenseTemplateImpl licenseTemplate;
	void * licenseKeyValidationData;
	unsigned licenseKeyValidationDataLen;
	int activationStatus;
	int productId;
	void * parentLicensingClient;
	bool parentUsesWideChar;
	int timeValidationMethod;
	//const char_t * (*GetCurrentHardwareIdCallback)(void * callerContext);
	//bool (*MatchCurrentHardwareIdCallback)(void * callerContext, const char_t * hwid);
};

#endif
