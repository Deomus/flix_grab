using System;
using System.Collections.Generic;
using System.Configuration;
using System.Linq;
using System.Net.Http;
using System.Security.Cryptography;
using System.Text;
using System.Web;
using System.Web.Http;
using System.Web.Mvc;
using LicensingService.Filters;
using Licensing.Web.DAL;
using Licensing.Web.Models;
using System.IO;
using Newtonsoft.Json;

namespace LicensingService.Controllers
{
    [WebApiAuthorizeSDKLicensing(LicensingSDKEdition.Demo | LicensingSDKEdition.StandardLegacy | LicensingSDKEdition.Professional | LicensingSDKEdition.Enterprise | LicensingSDKEdition.Ultimate)]
    public class PayeerController : PaymentServiceController
    {
        public class PaymentParameters
        {
            public string m_operation_id { get; set; }
            public string m_operation_ps { get; set; }
            public string m_operation_date { get; set; } //The date and time of the transaction in the format DD.MM.HH.YYYY HH:CC (UTC+3)
            public string m_operation_pay_date { get; set; }
            public string m_shop { get; set; }              //merchant id
            public string m_orderid { get; set; }
            public decimal m_amount { get; set; }
            public string m_curr { get; set; }              //currency
            public string m_desc { get; set; }              //BASE64
            public string m_status { get; set; }            //success, fail        
            public string m_sign { get; set; }           
            public string client_email { get; set; }             
            public string client_account { get; set; }             
            public string transfer_id { get; set; }
            public string summa_out { get; set; }
            public string m_params { get; set; }            //A JSON array of data of additional parameters

        }


        // POST: Payeer
        [WebApiAuthorizeIPAddress("PaymentServiceIPAddresses")]
        public HttpResponseMessage PaymentProcessor([FromBody]PaymentParameters p)
        {
            var m_key = ConfigurationManager.AppSettings["PayeerSecretKey"];

            var sign_array = new List<string> { p.m_operation_id, 
                                                p.m_operation_ps, 
                                                p.m_operation_date, 
                                                p.m_operation_pay_date, 
                                                p.m_shop, 
                                                p.m_orderid, 
                                                p.m_amount.ToString(), 
                                                p.m_curr, 
                                                p.m_desc, 
                                                p.m_status, m_key };

            if (p.m_params != null)
            {
                sign_array.Add(p.m_params);
            }

            var sign = SignHash(String.Join(":", sign_array));
            if (sign == p.m_sign && p.m_status == "success")
            {
                try
                {
                    string[] licenseKeys = ProcessKeyGenerationRequest(p.m_operation_id, p.m_orderid, 1, p.m_amount, p.m_curr, null, null, null, p.client_email, null, null, null, null, null, null, true);
                    return new HttpResponseMessage()
                    {
                        Content = new StringContent(p.m_orderid + "|success", Encoding.UTF8, "text/plain")
                    };
                }
                catch (Exception ex)
                {
                    //NO Exception Handling;
                    EventLog.Log("Exception while ProcessKeyGenerationRequest ", ex.Message + ((ex.InnerException != null) ? "; " + ex.InnerException.Message : ""), EventType.Error);
                }
            }
            return new HttpResponseMessage()
            {
                Content = new StringContent(p.m_orderid + "|fail", Encoding.UTF8, "text/plain")
            };
        }

        [System.Web.Http.HttpGet]
        public IHttpActionResult Invoice(string id)
        {
            //Success Page Example
            //https://www.flixgrab.com/payment_success?m_sign=83A2D10D6150C56EDCF35C0F9A82FD0427B10EFA39F2DE532C6D6A788FF102B7&m_desc=RmxpeEdyYWIrIFBSRU1JVU0gOTAtZGF5cyBsaWNlbnNlIGZvciAzIFBD&m_operation_date=26.06.2018+23%3A29%3A17&m_orderid=FGP-90&m_shop=597382377&m_amount=0.10&m_operation_ps=2609&m_operation_id=600073533&m_curr=RUB&lang=en&m_operation_pay_date=26.06.2018+23%3A29%3A29&m_status=success
            //Status 
            //http://localhost:60001/api/payeer/Invoice?id=FGP-90
            var m_key = ConfigurationManager.AppSettings["PayeerSecretKey"];

            PaymentServiceMapping mapping = db.PaymentServiceMappings.Where(m => m.PaymentServiceProductId == id).FirstOrDefault();
            if (mapping == null)
                return ResponseMessage(new HttpResponseMessage()
                {
                    Content = new StringContent("Invalid invoice id '" + id + "'", Encoding.UTF8, "text/plain")
                });
                                   
            Product product = db.Products.Find(mapping.ProductId);

            if (product == null)
            {
                EventLog.Log("Invalid product id", "Product id '" + mapping.ProductId + "' is invalid", EventType.Error, "PaymentServiceKeyGenerator");
                return ResponseMessage(new HttpResponseMessage()
                {
                    Content = new StringContent("Invalid product id '" + mapping.ProductId + "'", Encoding.UTF8, "text/plain")
                });
            }

            UriBuilder uriBuilder = new UriBuilder("https://payeer.com/merchant");

            String description = String.Format("{0} PREMIUM", product.Name);

            if (product.DefaultLicenseDuration > 0)
                description += String.Format(" {0}-days", product.DefaultLicenseDuration);

            description += " license";

            if (product.DefaultMaxUniqueHardwareIdsPerLicenseKey > 0)
                description += String.Format(" for {0} PC", product.DefaultMaxUniqueHardwareIdsPerLicenseKey);
                        
            var parameters = new List<KeyValuePair<string, string>>
            {
                new KeyValuePair<string, string>("m_shop", ConfigurationManager.AppSettings["PayeerMerchantId"]), 
                new KeyValuePair<string, string>("m_orderid", id), 
                new KeyValuePair<string, string>("m_amount", product.Price.ToString()), 
                new KeyValuePair<string, string>("m_curr", product.Currency.ToUpper()), 
                new KeyValuePair<string, string>("m_desc", Base64Encode(description)),
            };

            //Sign
            var sign_array = new List<string>();
            foreach (var parameter in parameters)
            {
                sign_array.Add(parameter.Value);
            }
            sign_array.Add(m_key);
            var sign = SignHash(String.Join(":", sign_array));

            parameters.Add(new KeyValuePair<string, string>("m_sign", sign));

            var validParameters = parameters.Where(p => !String.IsNullOrEmpty(p.Value));
            var formattedParameters = validParameters.Select(p => p.Key + "=" + p.Value);
            uriBuilder.Query = String.Join("&", formattedParameters);
                        
            return Redirect(uriBuilder.Uri.ToString());
        }

        [System.Web.Http.HttpGet]
        public IHttpActionResult Invoice(string id, string success_url, string fail_url)
        {
            //Success Page Example
            //https://www.flixgrab.com/payment_success?m_sign=83A2D10D6150C56EDCF35C0F9A82FD0427B10EFA39F2DE532C6D6A788FF102B7&m_desc=RmxpeEdyYWIrIFBSRU1JVU0gOTAtZGF5cyBsaWNlbnNlIGZvciAzIFBD&m_operation_date=26.06.2018+23%3A29%3A17&m_orderid=FGP-90&m_shop=597382377&m_amount=0.10&m_operation_ps=2609&m_operation_id=600073533&m_curr=RUB&lang=en&m_operation_pay_date=26.06.2018+23%3A29%3A29&m_status=success
            //Status 
            //http://localhost:60001/api/payeer/Invoice?id=FGP-90&success_url=flixgrab.com&fail_url=flixgrab.com
            var m_key = ConfigurationManager.AppSettings["PayeerSecretKey"];

            PaymentServiceMapping mapping = db.PaymentServiceMappings.Where(m => m.PaymentServiceProductId == id).FirstOrDefault();
            if (mapping == null)
                return ResponseMessage(new HttpResponseMessage()
                {
                    Content = new StringContent("Invalid invoice id '" + id + "'", Encoding.UTF8, "text/plain")
                });

            Product product = db.Products.Find(mapping.ProductId);

            if (product == null)
            {
                EventLog.Log("Invalid product id", "Product id '" + mapping.ProductId + "' is invalid", EventType.Error, "PaymentServiceKeyGenerator");
                return ResponseMessage(new HttpResponseMessage()
                {
                    Content = new StringContent("Invalid product id '" + mapping.ProductId + "'", Encoding.UTF8, "text/plain")
                });
            }

            UriBuilder uriBuilder = new UriBuilder("https://payeer.com/merchant");

            String description = String.Format("{0} PREMIUM", product.Name);

            if (product.DefaultLicenseDuration > 0)
                description += String.Format(" {0}-days", product.DefaultLicenseDuration);

            description += " license";

            if (product.DefaultMaxUniqueHardwareIdsPerLicenseKey > 0)
                description += String.Format(" for {0} PC", product.DefaultMaxUniqueHardwareIdsPerLicenseKey);


            var addParams = new Dictionary<string, object>
            {
                //{ "success_url", "http://google.com/new_success_url" },
                //{ "fail_url", "http://google.com/new_success_url" },
                //{ "status_url", "http://google.com/new_success_url" },
                { "reference", new Dictionary<string, string>
                    {
                        { "redirect_success", success_url },
                        { "redirect_fail", fail_url },
                    }
                }
            };

            string json_params = JsonConvert.SerializeObject(addParams);
            string m_params = Uri.EscapeDataString((EncryptAes256( json_params, m_key+id)));
      
            string params_decrypted = DecryptAes256(Uri.UnescapeDataString(m_params), m_key + id);

            if (json_params != params_decrypted)
                EventLog.Log("AES description error", "Can't decrypt json message", EventType.Error, "Payeer.Invoice");

            var parameters = new List<KeyValuePair<string, string>>
            {
                new KeyValuePair<string, string>("m_shop", ConfigurationManager.AppSettings["PayeerMerchantId"]),
                new KeyValuePair<string, string>("m_orderid", id),
                new KeyValuePair<string, string>("m_amount", product.Price.ToString()),
                new KeyValuePair<string, string>("m_curr", product.Currency.ToUpper()),
                new KeyValuePair<string, string>("m_desc", Base64Encode("Test")),
                new KeyValuePair<string, string>("m_params", m_params),
            };

            //Sign
            var sign_array = new List<string>();
            foreach (var parameter in parameters)
            {
                sign_array.Add(parameter.Value);
            }
            sign_array.Add(m_key);
            var sign = SignHash(String.Join(":", sign_array));

            parameters.Add(new KeyValuePair<string, string>("m_sign", sign));

            //Add Encryption scheme
            parameters.Add(new KeyValuePair<string, string>("m_cipher_method", "AES-256-CBC"));

            var validParameters = parameters.Where(p => !String.IsNullOrEmpty(p.Value));
            var formattedParameters = validParameters.Select(p => p.Key + "=" + Uri.EscapeDataString(p.Value));
            uriBuilder.Query = String.Join("&", formattedParameters);

            var url = uriBuilder.Uri.AbsoluteUri;
            return Redirect(uriBuilder.Uri.AbsoluteUri);
        }

        public static string SignHash(string text)
        {
            byte[] data = Encoding.Default.GetBytes(text);
            var result = new SHA256Managed().ComputeHash(data);
            return BitConverter.ToString(result).Replace("-", "").ToUpper();
        }

        public static string Base64Encode(string plainText)
        {
            var plainTextBytes = System.Text.Encoding.UTF8.GetBytes(plainText);
            return System.Convert.ToBase64String(plainTextBytes);
        }        public static string Base64Decode(string base64)
        {
            return Encoding.UTF8.GetString(System.Convert.FromBase64String(base64));
        }

        public static string EncryptAes256(string text, string secretKey)
        {
            string result;
            var e = new AesManaged()
            {
                Mode = CipherMode.CBC,
            };
            var key_hash = CalculateMD5Hash(secretKey);
            byte[] key = Encoding.ASCII.GetBytes(key_hash);

            byte[] iv = null;
            Array.Resize(ref key, 32); //add zeros;
            Array.Resize(ref iv, 16); //Init zeros


            using (MemoryStream msEncrypt = new MemoryStream())
            {
                using (ICryptoTransform encryptor = e.CreateEncryptor(key, iv))
                {
                    using (CryptoStream csEncrypt = new CryptoStream(msEncrypt, encryptor, CryptoStreamMode.Write))
                    {
                        using (StreamWriter swEncrypt = new StreamWriter(csEncrypt))
                        {
                            swEncrypt.Write(text);
                        }
                    }
                }
                result = Convert.ToBase64String(msEncrypt.ToArray());
            }
            e.Clear();

            return result;
        }

        public static string DecryptAes256(string data, string secretKey)
        {
            string result;
            var e = new AesManaged()
            {
                Mode = CipherMode.CBC,
            };

            var key_hash = CalculateMD5Hash(secretKey);
            byte[] key = Encoding.ASCII.GetBytes(key_hash);
            byte[] iv = null;
            Array.Resize(ref key, 32); //add zeros;
            Array.Resize(ref iv, 16); //Init zeros

            using (MemoryStream msDecrypt = new MemoryStream(Convert.FromBase64String(data)))
            {
                using (ICryptoTransform decryptor = e.CreateDecryptor(key, iv))
                {
                    using (CryptoStream csDecrypt = new CryptoStream(msDecrypt, decryptor, CryptoStreamMode.Read))
                    {
                        using (StreamReader swDecrypt = new StreamReader(csDecrypt))
                        {
                            result = swDecrypt.ReadToEnd();
                        }
                    }
                }
            }
            e.Clear();
            return result;
        }
        public static string CalculateMD5Hash(string input)
        {
            MD5 md5 = System.Security.Cryptography.MD5.Create();
            byte[] inputBytes = System.Text.Encoding.ASCII.GetBytes(input);
            var hash = md5.ComputeHash(inputBytes);

            StringBuilder sb = new StringBuilder();

            for (int i = 0; i < hash.Length; i++)
            {
                sb.Append(hash[i].ToString("x2"));
            }

            return sb.ToString();

        }
    }
}