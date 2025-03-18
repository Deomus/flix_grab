using System;
using System.Collections.Generic;
using System.Configuration;
using System.Linq;
using System.Net.Http;
using System.Security.Cryptography;
using System.Text;
using System.Web;
using System.Web.Http;

using LicensingService.Filters;
using Licensing.Web.DAL;
using Licensing.Web.Models;
using System.IO;
using Newtonsoft.Json;
using System.Collections.Specialized;
using System.Net;
using System.Net.Http.Formatting;
using System.ComponentModel.DataAnnotations;

namespace LicensingService.Controllers
{
    //[WebApiAuthorizeSDKLicensing(LicensingSDKEdition.Demo | LicensingSDKEdition.StandardLegacy | LicensingSDKEdition.Professional | LicensingSDKEdition.Enterprise | LicensingSDKEdition.Ultimate)]
    [RoutePrefix("IK")]
    public class InterkassaController : PaymentServiceController
    {
        public class PaymentParameters
        {
            public string ik_co_id { get; set; } //Checkout ID
            public string ik_pm_no { get; set; } //Payment No
            public string ik_desc { get; set; } //Payment Description
            public string ik_pw_via { get; set; } //Payway Via
            public decimal ik_am { get; set; }              //Amount
            public string ik_cur { get; set; }  //Currency
           
            public string ik_act { get; set; }              //Action = process
            public string ik_inv_id { get; set; }              //Invoice Id
            public string ik_x_email { get; set; }              //Additional param email;

            //Additional
            public string ik_co_prs_id { get; set; }            //Checkout Purse Id    
            public string ik_trn_id { get; set; }               //Transaction Id
            public string ik_inv_crt { get; set; }              //Invoice Created
            public string ik_inv_prc { get; set; }              //Invoice Processed
            public string ik_inv_st { get; set; }               //Invoice Status: success; fail
            public decimal ik_ps_price { get; set; }            //Paysystem Price
            public decimal ik_co_rfn { get; set; }              //Checkout Refund
            public string ik_sign { get; set; }               //Signature

        }

        private const string CurrencyPaywaysCacheName = "InterkassaCurrencyPayways";
        private const string SciUrl = "https://sci.interkassa.com/";


        // POST:
        [Route ("process")]
        [WebApiAuthorizeIPAddress("PaymentServiceIPAddresses")]
        public HttpResponseMessage PaymentProcessor(FormDataCollection form)
        {
            string[] licenses = new string[0];
                       
            NameValueCollection p = new NameValueCollection();
            foreach (var pair in form)
            {
                p.Add(pair.Key, pair.Value);
            }

            if (p["ik_inv_st"] == "success")
            {
                bool test_payment = p["ik_pw_via"] == "test_interkassa_test_xts";

                if (p["ik_co_id"] != ConfigurationManager.AppSettings["InterkassaCheckoutId"])
                    Abort(HttpStatusCode.BadRequest, "Wrong checkout id", "Wrong checkout id " + p["ik_co_id"]);

                string secret = test_payment ? ConfigurationManager.AppSettings["InterkassaTestKey"] :
                                    ConfigurationManager.AppSettings["InterkassaSecretKey"];

                string sign = GetSignature(p, secret);

                if (p["ik_sign"] != sign)
                    Abort(HttpStatusCode.InternalServerError, "Wrong signature", "Wrong signature: ik_sign=" + p["ik_sign"] + ", calculated=" + sign);

                if (p["ik_x_email"] == null)
                    Abort(HttpStatusCode.BadRequest, "No user email in request", "No user email");

                if (p["ik_inv_id"] == null)
                    Abort(HttpStatusCode.BadRequest, "No invoice id", "No invoice id");

                if (p["ik_pm_no"] == null)
                    Abort(HttpStatusCode.BadRequest, "No payment no.", "No payment no.");

                try
                {
                    String countryCode = null;
                    String countryName = null;
                    String city = null;
                    String postalCode = null;
                    if (!String.IsNullOrWhiteSpace(p["ik_x_user_ip"]))
                    {
                        LookupService geoIPService = (LookupService)HttpContext.Current.Application["GeoIPService"];

                        if (geoIPService != null )
                        {
                            Location location = geoIPService.getLocation(p["ik_x_user_ip"]);
                            
                            if (location != null)
                            {
                                postalCode = location.postalCode;
                                city = location.city;
                                countryName = location.countryName;
                                countryCode = location.countryCode;
                            }
                        }
                    }

                    licenses = ProcessKeyGenerationRequest(p["ik_inv_id"], p["ik_pm_no"],
                                                                    1, Convert.ToDecimal(p["ik_co_rfn"]), p["ik_cur"],
                                                                    null, null, null, p["ik_x_email"], null, null, postalCode, city, null, countryCode, !test_payment);
                }
                catch (Exception ex)
                {
                    return new HttpResponseMessage(HttpStatusCode.InternalServerError)
                    {
                        Content = new StringContent(ex.Message, Encoding.UTF8, "text/plain")
                    };
                }
            }
            return new HttpResponseMessage(HttpStatusCode.OK);
        }


        string GetSignature(NameValueCollection p, string secret)
        {
            string sign_data = "";
            // order the keys
            foreach (var item in p.AllKeys.OrderBy(k => k))
            {
                if (item != "ik_sign")
                    sign_data += String.Format("{0}:", p[item]);
            }

            sign_data += secret;

            using (var provider = System.Security.Cryptography.MD5.Create())
            {
                byte[] hash_bytes = provider.ComputeHash(Encoding.UTF8.GetBytes(sign_data));
                //24 sym;
                return System.Convert.ToBase64String(hash_bytes);

            }
        }

        [System.Web.Http.HttpGet]
        [Route("{id}")]
        public IHttpActionResult PaymentInvoice(string id, string email, bool test_payment = false)
        {
            //Success Page Example
            //https://www.flixgrab.com/payment_success?m_sign=83A2D10D6150C56EDCF35C0F9A82FD0427B10EFA39F2DE532C6D6A788FF102B7&m_desc=RmxpeEdyYWIrIFBSRU1JVU0gOTAtZGF5cyBsaWNlbnNlIGZvciAzIFBD&m_operation_date=26.06.2018+23%3A29%3A17&m_orderid=FGP-90&m_shop=597382377&m_amount=0.10&m_operation_ps=2609&m_operation_id=600073533&m_curr=RUB&lang=en&m_operation_pay_date=26.06.2018+23%3A29%3A29&m_status=success
            //Status 
            //http://localhost:60001/api/interkassa/Invoice?id=FGP-90&email=test@aaa
            //http://localhost:60001/api/interkassa/Invoice?id=FGP-90&email=test@aaa&test_payment=true
            //localhost:60001/ik/FGP-90?email=aaa
            //http://localhost:60001/ik/FGP-90?email=iandx@mail.ru&test_payment=true

            //TestSignature();

            var email_validator = new EmailAddressAttribute();
            if (email == null || !email_validator.IsValid(email))
                Abort(HttpStatusCode.BadRequest, "Email not valid", "Email not valid=" + email);

            PaymentServiceMapping mapping = db.PaymentServiceMappings.Where(m => m.PaymentServiceProductId == id).FirstOrDefault();
            if (mapping == null)
                Abort(HttpStatusCode.BadRequest, "Wrong invoice id", "Wrong invoice id=" + id);
                        
            Product product = db.Products.Find(mapping.ProductId);

            if (product == null)
                Abort(HttpStatusCode.BadRequest, "Invalid product id", "Invalid product id=" + mapping.ProductId);
                  
                 
            NameValueCollection p = new NameValueCollection()
            {
                {"ik_co_id", ConfigurationManager.AppSettings["InterkassaCheckoutId"] },
                {"ik_pm_no", id },
                {"ik_cur", product.Currency.ToUpperInvariant() },
                {"ik_am", product.Price.ToString() },
                {"ik_am_t", "payway" }, //Pay tariff by KASSA
   
                {"ik_desc", product.Name },
                {"ik_x_email", email },
                {"ik_x_user_ip", HttpContext.Current.Request.UserHostAddress },
                
                {"ik_act", "payways" },
                {"ik_loc", "en" },
                {"ik_int", "json" },
            };
            //string secret = ConfigurationManager.AppSettings["InterkassaSecretKey"];
            p["ik_sign"] = GetSignature(p, ConfigurationManager.AppSettings["InterkassaSecretKey"]);

            //https://sci.interkassa.com?ik_co_id=5b9fb4b23b1eaf35298b4567&ik_pm_no=FGP-90&ik_am=1&ik_cur=usd&ik_desc=AAAA&ik_act=payways&ik_int=json

            if (HttpContext.Current.Cache[CurrencyPaywaysCacheName] == null)
            {
                dynamic payways = JsonConvert.DeserializeObject(GetSciRequest(p));
                NameValueCollection cache_payways = new NameValueCollection();

                if (payways.resultCode == 0)
                {
                    foreach (var m in payways.resultData.paywaySet)
                    {
                        string cur = ((string)m.curAls).ToUpperInvariant();
                        string ser = m.ser;

                        if (cache_payways[cur] == null &&
                            (ser == "mastercard" || ser == "visa"))
                        {
                            cache_payways[cur] = m.als;
                        }
                    }
                    HttpContext.Current.Cache.Add(CurrencyPaywaysCacheName,
                                                    cache_payways,
                                                    null,
                                                    DateTime.Now.AddSeconds(120),
                                                    System.Web.Caching.Cache.NoSlidingExpiration,
                                                    System.Web.Caching.CacheItemPriority.Normal, null);
                }
                else
                    Abort(HttpStatusCode.InternalServerError, payways.resultMsg.ToString(), payways.resultMsg.ToString());
            }
            NameValueCollection currency_payways = (NameValueCollection)HttpContext.Current.Cache[CurrencyPaywaysCacheName];

            if (currency_payways != null)
            {
                p["ik_pw_via"] = currency_payways[product.Currency.ToUpperInvariant()];
            }
            else
                Abort(HttpStatusCode.InternalServerError, "Not found valid payway for currency", "Not found valid payway for currency " + product.Currency);

                     
            p["ik_act"] = "process";
            //TODO: add return url with invoice

            if (test_payment)
            {
                p["ik_pw_via"] = "test_interkassa_test_xts";
                p["ik_int"] = "web";
                p["ik_sign"] = GetSignature(p, ConfigurationManager.AppSettings["InterkassaSecretKey"]);

                return Redirect(MakeSciUri(p));
            }
            
            p["ik_sign"] = GetSignature(p, ConfigurationManager.AppSettings["InterkassaSecretKey"]);

            dynamic payform = JsonConvert.DeserializeObject(GetSciRequest(p));
            string pay_url = String.Empty;
            if (payform.resultCode == 0)
                pay_url = payform.resultData.paymentForm.action;
            else
                Abort(HttpStatusCode.InternalServerError, payform.resultMsg.ToString(), payform.resultMsg.ToString());

            return Redirect(pay_url);
        }

       

        string GetSciRequest(NameValueCollection p)
        {
            HttpWebRequest request = (HttpWebRequest)WebRequest.Create(MakeSciUri(p));

            using (HttpWebResponse response = (HttpWebResponse)request.GetResponse())
            using (Stream stream = response.GetResponseStream())
            using (StreamReader reader = new StreamReader(stream))
            {
                return reader.ReadToEnd();
            }
        }

        Uri MakeSciUri(NameValueCollection p)
        {
            UriBuilder uri_builder = new UriBuilder(SciUrl);
            var formatted = p.AllKeys.Select(k => k + "=" + WebUtility.UrlEncode(p[k]));

            uri_builder.Query = String.Join("&", formatted);
            return uri_builder.Uri;
        }

        bool TestSignature ()
        {
            NameValueCollection p = new NameValueCollection()
            {
                {"ik_co_id", "5534b1683b1eaf00758b4567" },
                {"ik_pm_no", "47533H-28042015-184310" },
                { "ik_pw_via", "test_interkassa_test_xts" },
                { "ik_am", "8990.00" },
                { "ik_cur", "RUB" },
                { "ik_desc" , "Your order No 47533H-28042015" },
                { "ik_co_prs_id" , "404025496344" },
                { "ik_inv_id" , "36051650" },
                { "ik_inv_st" , "success" },
                { "ik_inv_crt" , "2015-04-28 18:48:39" },
                { "ik_inv_prc" , "2015-04-28 18:48:39" },
                { "ik_trn_id" ,"" },

                { "ik_co_rfn" , "8810.2000" },
                { "ik_ps_price" , "9079.90" },
                { "ik_sign","n/ta1SrgrqeBOy7XwdnuiQ=="}
            };

            string sign = GetSignature(p, "VYZ1aGgkqmzyQshG");

            return (sign == p["ik_sign"]);
        }
             

        void Abort(HttpStatusCode httpCode, string httpMessage, string logMessage)
        {
            EventLog.Log(httpMessage, logMessage, EventType.Error, "Interkassa");

            HttpResponseMessage response = new HttpResponseMessage(httpCode);
            response.ReasonPhrase = httpMessage;
            throw new HttpResponseException(response);
        }
    }
}