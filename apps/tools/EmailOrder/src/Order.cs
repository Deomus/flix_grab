using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net.Http;
using System.Net;
using Newtonsoft.Json;
using System.Security.Cryptography;

namespace EmailOrder
{
    public class Order
    {
        public int Quantity { get; protected set; }
        public string TransactionId { get; protected set; }

        //Person data;
        public string FirstName { get; protected set; }
        public string LastName { get; protected set; }
        public string Address { get; protected set; }
        public string Email { get; set; }
        public string ZipCode { get; protected set; }

        public string City { get; protected set; }
        public string Country { get; protected set; }

        public decimal Amount { get; protected set; }
        public string Currency { get; protected set; }

        public string ProductName { get; protected set; }
        public int ProductId { get; protected set; }

        public DateTime Date { get; protected set; }

        public string[] ActivationKeys { get; protected set; }


        //Methods
        virtual public bool ParseEmail(string html, DateTime date)
        {
            return false;
        }

        private class KeyGeneratorParameters
        {
            public string order_id { get; set; }
            public int product_id { get; set; }
            public int quantity { get; set; }
            public decimal order_total { get; set; }
            public string currency_id { get; set; }
            public string first_name { get; set; }
            public string last_name { get; set; }
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

        private static readonly HttpClient client = new HttpClient();
        //Place order to licensing server;
        public bool Place(string url, string signature_key)
        {
            KeyGeneratorParameters p = new KeyGeneratorParameters();

            p.product_id = ProductId;
            p.order_id = TransactionId;
            p.quantity = Quantity;
            p.order_total = Amount;
            p.currency_id = Currency;

            p.first_name = FirstName;
            p.last_name = LastName;
            p.email = Email;
            p.country = Country;
            p.is_send_order = true;
            
            string json = JsonConvert.SerializeObject(p);
            string signature = HashHMAC(signature_key, json);
            var stringContent = new StringContent(json, Encoding.UTF8, "application/json");

            stringContent.Headers.Add("Signature", signature);

            try
            {
                var response = client.PostAsync(url, stringContent).Result;

                if (response.StatusCode == HttpStatusCode.OK)
                {
                    string responseString = response.Content.ReadAsStringAsync().Result;
                    ActivationKeys = responseString.Split(new string[] { Environment.NewLine }, StringSplitOptions.None);
                    return ActivationKeys.Count() > 0;
                }
                else
                {
                    LogHelper.WriteLog(LogType.Error, "HttpClient response code '{0}'", response.StatusCode);
                }
            }
            catch (HttpRequestException e)
            {
                LogHelper.WriteLog(LogType.Error, "HttpClient Post Error '{0}'", e.ToString());
            }
            catch(Exception e)
            {
                LogHelper.WriteLog(LogType.Error, "Unknown exception '{0}'", e.ToString());
            }

            return false;
                        
        }


        public string ToValues()
        {
            return string.Format("{0}\t{1}\t{2}\t{3}\t{4}\t{5}\t{6}\t{7}", Date, TransactionId, Email, FirstName, LastName, Quantity, Amount, Country);
        }

        static public string ToColumnNames()
        {
            return string.Format("Date\tTransactionId\tEmail\tFirstName\tLastName\tQuantity\tAmount\tCountry");
        }
    }
}
