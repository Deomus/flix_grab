using System;
using System.Collections.Generic;
using System.Reflection;
using System.Text;

namespace SoftActivate.Licensing
{
    public class SDKRegistration
    {
        public static void SetLicenseKey(string licenseKey)
        {
            DateTime sdkBuildDate = new DateTime(1970, 1, 1, 0, 0, 0, 0).AddSeconds(SoftActivate.Licensing.Version.BuildTimestamp);

            LicenseTemplate keyTemplate = new LicenseTemplate(5, 5, "EClDDQELjMGgCJew5QSVMBWSAFnlium1FRWDpwB8MjS+pzV4dwE=", null, 109, 16, "KeyData");

            KeyValidator validator = new KeyValidator(keyTemplate, licenseKey);
            if (validator.IsKeyValid())
            {
                SDKRegistration.licenseKey = licenseKey;

                byte[] rawKeyData = validator.QueryKeyData("KeyData");
                ushort data = (ushort)((ushort)(rawKeyData[0]) | ((ushort)(rawKeyData[1]) << 8));

                if (data == 0xA082 || data == 0x9A82 || data == 0xA182 || data == 0x8882 || data == 0x8982 || data == 0xA482)
                    isRegistered = false; //true;
                else
                    if (data == 0x8682) // demo
                        isRegistered = false;
                    else
                    {
                        // v2 version of license keys
                        keyTemplate = new LicenseTemplate(5, 5, "EClDDQELjMGgCJew5QSVMBWSAFnlium1FRWDpwB8MjS+pzV4dwE=", null, 109, 16, null);
                        keyTemplate.AddDataField("Edition", LicenseKeyFieldType.Integer, 3, 0);
                        keyTemplate.AddDataField("ExpDate", LicenseKeyFieldType.Date13, 13, 3);

                        validator = new KeyValidator(keyTemplate, licenseKey);
                        if (validator.IsKeyValid())
                        {
                            // demo is 0
                            isRegistered = (validator.QueryIntKeyData("Edition") == 0) ? false : true;

                            if (isRegistered)
                            {
                                DateTime expDate = validator.QueryDateKeyData("ExpDate");
                                if (sdkBuildDate > expDate)
                                    isExpired = true;
                            }
                        }
                    }
            }
        }

        public static bool IsRegistered
        {
            get
            {
                return isRegistered;
            }
        }

        public static bool IsExpired
        {
            get
            {
                return isExpired;
            }
        }

        internal static string LicenseKey
        {
            get
            {
                return licenseKey;
            }
        }

        private static bool isRegistered = false;
        private static bool isExpired = false;
        private static string licenseKey;
    }
}
