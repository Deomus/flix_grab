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
            SDKRegistration.licenseKey = licenseKey;
        }

        public static bool IsRegistered
        {
            get
            {
                return true;
            }
        }

        public static bool IsExpired
        {
            get
            {
                return false;
            }
        }

        internal static string LicenseKey
        {
            get
            {
                return licenseKey;
            }
        }

        private static string licenseKey;
    }
}
