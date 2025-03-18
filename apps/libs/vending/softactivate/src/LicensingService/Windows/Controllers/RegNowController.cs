using LicensingService.Filters;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Text;
using System.Web.Http;

namespace LicensingService.Controllers
{
    [WebApiAuthorizeSDKLicensing(LicensingSDKEdition.Demo | LicensingSDKEdition.StandardLegacy | LicensingSDKEdition.Professional | LicensingSDKEdition.Enterprise | LicensingSDKEdition.Ultimate)]
    public class RegNowController : PaymentServiceController
    {
        public class KeyGeneratorParameters
        {
            public string order_id { get; set; }
            public string item { get; set; }
            public int quantity { get; set; }
            public decimal order_total { get; set; }
            public string currency_id { get; set; }
            public string first_name {get; set; }
            public string last_name {get; set; }
            public string company { get; set; }
            public string addr1 { get; set; }
            public string addr2 { get; set; }
            public string zip { get; set; }
            public string city { get; set; }
            public string email { get; set; }
            public string phone { get; set; }
            public string state { get; set; }
            public string country { get; set; }
        }

        public class OrderNotificationParameters
        {
            public string order_id { get; set; }
            public string item { get; set; }
            public int quantity { get; set; }
            public decimal order_total { get; set; }
            public string currency_id { get; set; }
            public string first_name { get; set; }
            public string last_name { get; set; }
            public string company { get; set; }
            public string addr1 { get; set; }
            public string addr2 { get; set; }
            public string zip { get; set; }
            public string city { get; set; }
            public string email { get; set; }
            public string phone { get; set; }
            public string state { get; set; }
            public string country { get; set; }
        }

        // POST 

        // [AuthorizeIPAddress("PaymentServiceIPAddresses")]
        // [HttpPost]
        public HttpResponseMessage KeyGenerator([FromBody]KeyGeneratorParameters p)
        {
            if (p.quantity == 0)
                p.quantity = 1;

            string[] licenseKeys = ProcessKeyGenerationRequest(p.order_id, p.item, p.quantity, p.order_total, p.currency_id, p.first_name, p.last_name, p.company, p.email, p.phone, p.addr1 + p.addr2, p.zip, p.city, p.state, p.country);

            return new HttpResponseMessage()
            {
                Content = new StringContent(String.Join(Environment.NewLine, licenseKeys), Encoding.UTF8, "text/plain")
            };
        }

        [WebApiAuthorizeIPAddress("PaymentServiceIPAddresses")]
        [HttpPost]
        public void OrderNotification([FromBody]OrderNotificationParameters p)
        {

        }
    }
}
