using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using SoftActivate.Licensing;
using System.Configuration;
using Licensing.Web.DAL;
using Licensing.Web.Models;

namespace LicensingService.Filters
{
    public enum LicensingSDKEdition
    {
        Demo = 1,
        Standard = 2,
        Professional = 4,
        Enterprise = 8,
        Ultimate = 16,
        StandardLegacy = 32,
    }

    public class SDKLicensingAuthorizer
    {
        private enum RawLicensingSDKEdition
        {
            Demo = 0,
            Standard = 1,
            Professional = 2,
            EnterpriseDotNet = 3,
            EnterpriseNative = 4,
            Ultimate = 5
        }

        private enum LegacyLicensingSDKProductId
        {
            Demo = 34434,
            Standard = 41090,
            Professional = 39554,
        }

        public SDKLicensingAuthorizer(string sdkLicenseKey = null)
        {
            string licenseKey = (sdkLicenseKey == null) ? ConfigurationManager.AppSettings["SDKLicenseKey"] : sdkLicenseKey;

            if (String.IsNullOrEmpty(licenseKey))
                return;

            LicenseTemplate licensingSDKTemplate = new LicenseTemplate(5, 5, "AE4vDQELjMGgCJew5QSVMBWSAN2PaBa0zzYpjSYe0rcu88lYggE=", null, 109, 16, "ProductId");
            KeyValidator validator = new KeyValidator(licensingSDKTemplate, licenseKey);
            
            if (!validator.IsKeyValid())
                return;

            int e = validator.QueryIntKeyData("ProductId");

            bool legacyLicenseKey = true;

            switch (e)
            {
                case (int)LegacyLicensingSDKProductId.Demo:
                    edition = LicensingSDKEdition.Demo;
                    break;

                case (int)LegacyLicensingSDKProductId.Standard:
                    edition = LicensingSDKEdition.StandardLegacy;
                    break;

                case (int)LegacyLicensingSDKProductId.Professional:
                    edition = LicensingSDKEdition.Professional;
                    break;

                default:
                    {
                        legacyLicenseKey = false;

                        licensingSDKTemplate = new LicenseTemplate(5, 5, "AE4vDQELjMGgCJew5QSVMBWSAN2PaBa0zzYpjSYe0rcu88lYggE=", null, 109, 16, null);
                        licensingSDKTemplate.AddDataField("Edition", LicenseKeyFieldType.Integer, 3, 0);
                        licensingSDKTemplate.AddDataField("ExpDate", LicenseKeyFieldType.Date13, 13, 3);
                        validator = new KeyValidator(licensingSDKTemplate, licenseKey);
                        if (validator.IsKeyValid())
                        {
                            int rawEdition = validator.QueryIntKeyData("Edition");

                            if (Enum.IsDefined(typeof(RawLicensingSDKEdition), rawEdition))
                            {
                                switch ((RawLicensingSDKEdition)rawEdition)
                                {
                                    case RawLicensingSDKEdition.Demo:
                                        edition = LicensingSDKEdition.Demo;
                                        break;

                                    case RawLicensingSDKEdition.Standard:
                                        edition = LicensingSDKEdition.Standard;
                                        break;

                                    case RawLicensingSDKEdition.Professional:
                                        edition = LicensingSDKEdition.Professional;
                                        break;

                                    case RawLicensingSDKEdition.EnterpriseNative:
                                    case RawLicensingSDKEdition.EnterpriseDotNet:
                                        edition = LicensingSDKEdition.Enterprise;
                                        break;

                                    case RawLicensingSDKEdition.Ultimate:
                                        edition = LicensingSDKEdition.Ultimate;
                                        break;

                                    default:
                                        edition = LicensingSDKEdition.Demo;
                                        break;
                                }

                                expirationDate = validator.QueryDateKeyData("ExpDate");
                            }
                            else
                                edition = LicensingSDKEdition.Demo;
                            
                            return;
                        }
                    }

                    break;
            }

            if (legacyLicenseKey)
            {
                LicensingServiceDbContext db = new LicensingServiceDbContext();
                LicenseKey licenseKeyRecord = db.LicenseKeys.Where(l => l.Key == sdkLicenseKey).SingleOrDefault();
                if (licenseKeyRecord != null)
                {
                    Order order = db.Orders.Where(o => o.OrderItems.Any(i => i.LicenseKeys.Any(l => l.Key == sdkLicenseKey))).FirstOrDefault();
                    if (order != null)
                        expirationDate = order.Date.AddYears(1);
                }
            }
        }

        public LicensingSDKEdition Edition
        {
            get
            {
                return edition;
            }
        }

        public DateTime ExpirationDate
        {
            get
            {
                return expirationDate;
            }
        }

        private LicensingSDKEdition edition = LicensingSDKEdition.Demo;
        private DateTime expirationDate = DateTime.UtcNow.AddDays(-1);
    }
}
