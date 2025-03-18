#ifndef __SDKREGISTRATION_H__
#define __SDKREGISTRATION_H__

class SDKRegistrationImpl
{
public:
	static void SetLicenseKey(const char_t * key);

	static const char_t * GetLicenseKey();

	static bool IsRegistered();

	static bool IsExpired();

private:
	static bool isRegistered;

	static bool isExpired;

	static string_t licenseKey;
};

#endif
