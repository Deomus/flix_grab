using LicensingService.Filters;
using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Security.Cryptography;
using System.Text;
using System.Web;
using System.Web.Http;

namespace LicensingService.Controllers
{
    [WebApiAuthorizeSDKLicensing(LicensingSDKEdition.Demo | LicensingSDKEdition.StandardLegacy | LicensingSDKEdition.Professional | LicensingSDKEdition.Enterprise | LicensingSDKEdition.Ultimate)]
    public class OrderController : PaymentServiceController
    {
        public class KeyGeneratorParameters
        {
            public string order_id { get; set; }
            public int product_id { get; set; }
            public int quantity { get; set; }
            public decimal order_total { get; set; }
            public string currency_id { get; set; }
            public string first_name {get; set; }
            public string last_name {get; set; }
            public string email { get; set; }
            public string country { get; set; }
            public bool is_send_order { get; set; }
        }

        private static string HashHMAC(string key, string message)
        {
            var encoding = new UTF8Encoding();
            var hmac = new HMACSHA256(encoding.GetBytes(key));
            var hash = hmac.ComputeHash(encoding.GetBytes(message));
            return BitConverter.ToString(hash).Replace("-", "").ToLower();
        }
        // POST 
              

        [WebApiAuthorizeIPAddress("PaymentServiceIPAddresses")]
        [HttpPost]
        public HttpResponseMessage KeyGenerator()
        {
            byte[] rawRequest = HttpContext.Current.Request.BinaryRead(System.Web.HttpContext.Current.Request.ContentLength);
            string strRequest = System.Text.Encoding.UTF8.GetString(rawRequest);
            string signature = HttpContext.Current.Request.Headers.Get("Signature");
            KeyGeneratorParameters p = JsonConvert.DeserializeObject<KeyGeneratorParameters>(strRequest);

            var mappings = db.PaymentServiceMappings.Where(m => m.ProductId == p.product_id).ToList();
            var valid_service = mappings.Where(m => HashHMAC(m.PaymentServiceProductId, strRequest) == signature).FirstOrDefault();

            if (p.quantity > 0 && valid_service != null)
            {               
                string[] licenseKeys = ProcessKeyGenerationRequest(p.order_id, valid_service.PaymentServiceProductId, p.quantity, p.order_total, p.currency_id, p.first_name, p.last_name, null, p.email, null, null, null, null, null, p.country, p.is_send_order);

                return new HttpResponseMessage()
                {
                    Content = new StringContent(String.Join(Environment.NewLine, licenseKeys), Encoding.UTF8, "text/plain")
                };
            }

            return new HttpResponseMessage();

        }

    }
}
