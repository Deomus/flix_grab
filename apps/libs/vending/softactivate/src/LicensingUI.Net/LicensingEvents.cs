using System;
using System.Collections.Generic;
using System.Text;

namespace SoftActivate.Licensing
{
    public class LicensingEventArgs
    {
        public LICENSE_STATUS LicenseStatus { get; set; }
        public KeyValidator KeyValidator { get; internal set; }
        public string RegistrationName { get; internal set; }
        public string RegistrationEmail { get; internal set; }
        public bool IsTrial { get; internal set; }
    }

    public delegate void LicensingEventDelegate(object sender, LicensingEventArgs args);
}
