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
    public class ShareItController : PaymentServiceController
    {
        public class KeyGeneratorParameters
        {
            public string PURCHASE_ID { get; set; }
            public string PRODUCT_ID { get; set; }
            public int    QUANTITY { get; set; }
            public string REG_NAME { get; set; }
            public string FIRSTNAME { get; set; }
            public string LASTNAME { get; set; }
            public string COMPANY { get; set; }
            public string EMAIL { get; set; }
            public string PHONE { get; set; }
            public string STREET { get; set; }
            public string ZIP { get; set; }
            public string CITY { get; set; }
            public string STATE { get; set; }
            public string ISO_CODE { get; set; }
        }

        // POST api/<controller>
        [WebApiAuthorizeIPAddress("PaymentServiceIPAddresses")]
        [HttpPost]
        public HttpResponseMessage KeyGenerator([FromBody]KeyGeneratorParameters p)
        {
            if (p.QUANTITY == 0)
                p.QUANTITY = 1;

            string firstName = p.REG_NAME; 
            string lastName = null;

            if (!String.IsNullOrEmpty(p.FIRSTNAME))
            {
                firstName = p.FIRSTNAME;
                lastName = p.LASTNAME;
            }

            string[] licenseKeys = ProcessKeyGenerationRequest(p.PURCHASE_ID, p.PRODUCT_ID, p.QUANTITY, 0, null, firstName, lastName, p.COMPANY, p.EMAIL, null, null, null, null, null, null);

            return new HttpResponseMessage()
            {
                Content = new StringContent(String.Join(Environment.NewLine, licenseKeys), System.Text.Encoding.UTF8, "text/plain")
            };
        }
    }
}
