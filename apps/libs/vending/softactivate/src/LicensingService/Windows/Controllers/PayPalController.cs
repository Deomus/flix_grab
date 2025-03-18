using System;
using System.Collections.Generic;
using System.Configuration;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Text;
using System.Threading.Tasks;
using System.Web;
using System.Web.Http;
using System.Web.Mvc;

namespace LicensingService.Controllers {

    public class PayPalController : PaymentServiceController
    {
        public class OrderNotificationParameters
        {
            public string txn_type;
            public string txn_id;
            public string item_number;
            public string payer_email;
            public int quantity;
            public decimal mc_gross;
            public string mc_currency;
            public string first_name;
            public string last_name;
            public string payer_business_name;
            public string contact_phone;
            public string address_street;
            public string address_zip;
            public string address_city;
            public string address_state;
            public string residence_country;
            public string receiver_email;

            public OrderNotificationParameters(Dictionary<string, string> postParams)
            {
                postParams.TryGetValue("txn_type", out txn_type);
                postParams.TryGetValue("txn_id", out txn_id);
                postParams.TryGetValue("item_number", out item_number);
                postParams.TryGetValue("payer_email", out payer_email);
                
                string quantity_str; postParams.TryGetValue("quantity", out quantity_str);

                if (!String.IsNullOrEmpty(quantity_str))
                {
                    if (!Int32.TryParse(quantity_str, out quantity))
                        throw new Exception("invalid PayPal quantity");
                }

                string mc_gross_str; postParams.TryGetValue("mc_gross", out mc_gross_str);

                if (!String.IsNullOrEmpty(mc_gross_str)) 
                {
                    if (!Decimal.TryParse(mc_gross_str, out mc_gross))
                        throw new Exception("invalid PayPal mc_gross parameter");
                }

                postParams.TryGetValue("mc_currency", out mc_currency);
                postParams.TryGetValue("first_name", out first_name);
                postParams.TryGetValue("last_name", out last_name);
                postParams.TryGetValue("payer_business_name", out payer_business_name);
                postParams.TryGetValue("contact_phone", out contact_phone);
                postParams.TryGetValue("address_street", out address_street);
                postParams.TryGetValue("address_zip", out address_zip);
                postParams.TryGetValue("address_city", out address_city);
                postParams.TryGetValue("address_state", out address_state);
                postParams.TryGetValue("residence_country", out residence_country);
                postParams.TryGetValue("receiver_email", out receiver_email);
            }
        }

        // can't use WebApi [FromBody] binding because we need the raw request for PayPal authorization
        public void OrderNotification()
        {
            byte[] rawRequest = System.Web.HttpContext.Current.Request.BinaryRead(System.Web.HttpContext.Current.Request.ContentLength);

            string strRequest = System.Text.Encoding.ASCII.GetString(rawRequest);

            Dictionary<string, string> postParams = new Dictionary<string, string>();
            string[] rawParams = strRequest.Split('&');
            
            foreach (string param in rawParams)
            {
                string[] kvPair = param.Split('=');
                string key = kvPair[0];
                string value = HttpUtility.UrlDecode(kvPair[1]);
                postParams.Add(key, value);
            }

            OrderNotificationParameters p = new OrderNotificationParameters(postParams);

            if (p.txn_type != "web_accept")
                return;

            if (!(ConfigurationManager.AppSettings["Testing"] == "true" || ConfigurationManager.AppSettings["Testing"] == "1"))
            {
                if (!ValidatePayPalRequest(strRequest))
                {
                    return;
                }
            }

            if (p.quantity == 0)
                p.quantity = 1;

            ProcessKeyGenerationRequest(p.txn_id, p.item_number, p.quantity, p.mc_gross, p.mc_currency, p.first_name, p.last_name, p.payer_business_name, p.payer_email, p.contact_phone, p.address_street, p.address_zip, p.address_city, p.address_state, p.residence_country, true, p.receiver_email);
        }

        bool ValidatePayPalRequest(string strRequest)
        {
            //
            // Check with PayPal to see if this payment notification is legit
            //            

            //Post back to either sandbox or live
            //string strSandbox = "https://www.sandbox.paypal.com/cgi-bin/webscr";
            string strLive = "https://www.paypal.com/cgi-bin/webscr";
            HttpWebRequest req = (HttpWebRequest)WebRequest.Create(strLive);

            //Set values for the request back
            req.Method = "POST";
            req.ContentType = "application/x-www-form-urlencoded";
            strRequest = strRequest + "&cmd=_notify-validate";
            req.ContentLength = strRequest.Length;

            //for proxy
            //WebProxy proxy = new WebProxy(new Uri("http://url:port#"));
            //req.Proxy = proxy;

            StreamWriter streamOut = new StreamWriter(req.GetRequestStream(), System.Text.Encoding.ASCII);
            streamOut.Write(strRequest);
            streamOut.Close();
            StreamReader streamIn = new StreamReader(req.GetResponse().GetResponseStream());
            string strResponse = streamIn.ReadToEnd();
            streamIn.Close();

            // uncomment the following comment when in production ! For testing purposes we don't check the result
            if (strResponse != "VERIFIED")
                return false;

            if (System.Web.HttpContext.Current.Request.Form["payment_status"] != "Completed")
            {
                return false;
            }

            return true;
        }
    }
}
