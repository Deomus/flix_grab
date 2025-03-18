using LicensingService.Filters;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Web.Http;

namespace LicensingService.Controllers
{
    [WebApiAuthorizeSDKLicensing(LicensingSDKEdition.Demo | LicensingSDKEdition.StandardLegacy | LicensingSDKEdition.Professional | LicensingSDKEdition.Enterprise | LicensingSDKEdition.Ultimate)]
    public class FastSpringController : PaymentServiceController
    {
        public class KeyGeneratorParameters
        {
            public string reference { get; set; }
            public string internalProductName { get; set; }
            public int quantity { get; set; }
            public string email { get; set; }
            public string company { get; set; }
        }

        public class OrderNotificationParameters
        {
            public string reference { get; set; }
            public string internalProductName { get; set; }
            public int quantity { get; set; }
            public string email { get; set; }
            public string company { get; set; }
        }

        [HttpPost]
        public HttpResponseMessage KeyGenerator(KeyGeneratorParameters p)
        {
            if (p.quantity == 0)
                p.quantity = 1;

            string[] licenseKeys = ProcessKeyGenerationRequest(p.reference, p.internalProductName, p.quantity, 0, null, null, null, p.company, p.email, null, null, null, null, null, null);

            return new HttpResponseMessage()
            {
                Content = new StringContent(String.Join(Environment.NewLine, licenseKeys), System.Text.Encoding.UTF8, "text/plain")
            };
        }

        [HttpPost]
        public void OrderNotification(OrderNotificationParameters p)
        {

        }
    }
}
