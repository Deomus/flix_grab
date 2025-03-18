#include "precomp.h"
#include "uniconv.h"
#include "template.h"
#include "validator.h"
#include "sdkregistration.h"
#include "version.h"

#include <time.h>

bool SDKRegistrationImpl::isRegistered = false;
bool SDKRegistrationImpl::isExpired = false;
string_t SDKRegistrationImpl::licenseKey;

void SDKRegistrationImpl::SetLicenseKey(const char_t * key)
{
    LicenseTemplateImpl keyTemplate;

    keyTemplate.SetCharactersPerGroup(5);
    keyTemplate.SetNumberOfGroups(5);
    keyTemplate.SetSignatureSize(109);
    keyTemplate.SetDataSize(16);
    keyTemplate.AddDataField(T_STR("KeyData"), FIELD_TYPE_INTEGER, 16, 0); 
    keyTemplate.SetPublicKeyCertificate(T_STR("AE4vDQELjMGgCJew5QSVMBWSAN2PaBa0zzYpjSYe0rcu88lYggE="));
    KeyValidatorImpl keyValidator(&keyTemplate, key);

    if (keyValidator.IsKeyValid())
    {
        licenseKey = key;

        unsigned short data;
        int len = 2;
        keyValidator.QueryKeyData(T_STR("KeyData"), &data, &len);
        if ((data & 0xFFFE) == 0xA082 || (data & 0xFFFE) == 0x9A82)
            isRegistered = false;
        else
            if (data == 0x8682)
                isRegistered = false;
            else
            {
                LicenseTemplateImpl keyTemplate;

                keyTemplate.SetCharactersPerGroup(5);
                keyTemplate.SetNumberOfGroups(5);
                keyTemplate.SetSignatureSize(109);
                keyTemplate.SetDataSize(16);
                keyTemplate.AddDataField(T_STR("Edition"), FIELD_TYPE_INTEGER, 3, 0); 
                keyTemplate.AddDataField(T_STR("ExpDate"), FIELD_TYPE_DATE13, 13, 3); 
                keyTemplate.SetPublicKeyCertificate(T_STR("AE4vDQELjMGgCJew5QSVMBWSAN2PaBa0zzYpjSYe0rcu88lYggE="));
                KeyValidatorImpl keyValidator(&keyTemplate, key);

                if (keyValidator.IsKeyValid())
                {
                    if (keyValidator.QueryIntKeyData(T_STR("Edition")) == 0)
                        isRegistered = false;
                    else
                    {
                        isRegistered = true;

                        int year, month, day;
                        keyValidator.QueryDateKeyData(T_STR("ExpDate"), &year, &month, &day);

                        tm t;

                        memset(&t, 0, sizeof(t));

                        t.tm_year = year - 1900;
                        t.tm_mon = month - 1;
                        t.tm_mday = day;
                        t.tm_hour = 23;
                        t.tm_min = 59;
                        t.tm_sec = 59;

                        time_t expTime = mktime(&t);

                        if (BUILD_TIMESTAMP > expTime)
                            isExpired = true;
                    }
                }
            }
    }
}

bool SDKRegistrationImpl::IsRegistered()
{
    return isRegistered;
}

bool SDKRegistrationImpl::IsExpired()
{
    return isExpired;
}

const char_t * SDKRegistrationImpl::GetLicenseKey()
{
    return licenseKey.c_str();
}

namespace FreeGrabApp {
    namespace Licensing {
        template<>
        void SDKRegistrationT<char>::SetLicenseKey(const char * key)
        {
        #ifdef _UNICODE
            USES_CONVERSION;
            SDKRegistrationImpl::SetLicenseKey(A2W(key));
        #else
            SDKRegistrationImpl::SetLicenseKey(key);
        #endif
        }

        template<>
        void SDKRegistrationT<wchar_t>::SetLicenseKey(const wchar_t * key)
        {
        #ifdef _UNICODE
            SDKRegistrationImpl::SetLicenseKey(key);
        #else
            USES_CONVERSION;
            SDKRegistrationImpl::SetLicenseKey(W2A(key));
        #endif
        }
    };
};