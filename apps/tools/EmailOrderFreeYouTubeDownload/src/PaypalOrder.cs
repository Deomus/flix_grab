using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Threading.Tasks;
using System.Text.RegularExpressions;
using System.Globalization;

namespace EmailOrder
{
    
    class PaypalOrder : Order
    {
        
        public PaypalOrder(string product_name, int product_id) {
            ProductName = product_name;
            ProductId = product_id;
        }

        public override bool ParseEmail(string html, DateTime date)
        {
            if (html == null)
                return false;
            Date = date;
            Quantity = 0;
            TransactionId = FirstName = LastName = Email = null;

            return (ParseTransactionId(html) &&
                    ParseBuyerDetails(html) &&
                    ParseQuantityAndAmount(html));
                    //&& ParseAddress(html));
            
        }


        bool ParseTransactionId(string html)
        {            
            //<a\W+href=""([^<>]*?)""[^<>]*?>\W*(\w*)\W*<\/a>
            var matches = Regex.Matches(html, @"<a\W+href=""([^<>]*?)""[^<>]*?>\W*(\w*)\W*<\/a>");
            
            foreach(Match match in matches)
            {
                if ( match.Groups[1].Value.Contains(match.Groups[2].Value) &&
                    !match.Groups[2].Value.Contains(@"paypal"))
                {
                    this.TransactionId = match.Groups[2].Value;
                }
            }
         
            return TransactionId != null;
        }

        //<span[^<>]*?display:[^<>]*?>\W*([^<>\t\n]+)\W*<(?:br|BR)\s{0,1}\/{0,1}>\W*<\/span>\W*<span[^<>]*?display:[^<>]*?>\W*([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z0-9]{2,})\W*<(?:br|BR)\s{0,1}\/{0,1}>\W*<\/span>
        bool ParseBuyerDetails(string html)
        {
            //Extract span blocks of name and email;
            var matches = Regex.Matches(html, @"<span[^<>]*?display:[^<>]*?>\W*([^<>\r\n]*[^<>\s])\W*<(?:br|BR)\s{0,1}\/{0,1}>\W*<\/span>\W*<span[^<>]*?display:[^<>]*?>\W*([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z0-9]{2,})\W*<(?:br|BR)\s{0,1}\/{0,1}>\W*<\/span>");
            foreach (Match match in matches)
            {
                if (match.Groups.Count > 1)
                {
                    var names = match.Groups[1].Value.Split(' ');

                    FirstName = names.First();
                    
                    if (names.Count() > 1)
                    {
                        LastName = names.Last();

                        if (names.Count() > 2 && FirstName.Count() <= 1)
                            FirstName += ' ' + names[1];
                    }
                        
                    Email = match.Groups[2].Value;
                }
            }

            return Email != null;
        }

    
        bool ParseQuantityAndAmount(string html)
        {
            //<tr.*>\s*<td.*>[\w<>;:\-="\/\s]*FreeYouTubeDownload[\w<>;:\-="\/\s]*<\/td>\s*(?:<td.*>\s*([\w\W]*?)\s*<\/td>\s*){3}<\/tr>


            //var matches = Regex.Matches(html, @"(?:<td[^<>]*>\s*(?:(?:<span[^<>]*>|<br\/>)\s*)*([^<>]*)(?:(?:<\/span>|<br\/>)\s*)*<\/td>\s*){4}");
            var matches = Regex.Matches(html, @"(?:<td[^<>]*>\s*(?:(?:(?:<span[^<>]*>|<br\/>)\s*)*([^<>]*)(?:(?:<\/span>|<br\/>)\s*)*){1,2}<\/td>\s*){4}");

            Quantity = 0;
            foreach (Match match in matches)
            {
                if (match.Groups.Count > 1 
                    && match.Groups[1].Captures.Count >= 8
                    && match.Groups[1].Captures[0].Value.Contains(ProductName))
                {
                    int quantity = 0;
                    if (Int32.TryParse(match.Groups[1].Captures[4].Value, out quantity))
                        Quantity = quantity;

                    Match item_match = Regex.Match(match.Groups[1].Captures[1].Value, @":\s*(\w+)");

                    //TODO: use item number
                    string item_number = item_match.Groups[1].Value;

                    NumberFormatInfo MyNFI = new NumberFormatInfo();
                    MyNFI.NegativeSign = "-";
                    MyNFI.CurrencyDecimalSeparator = ",";
                    MyNFI.CurrencyGroupSeparator = ".";
                    MyNFI.CurrencySymbol = "$";

                    string amount_string = match.Groups[1].Captures[6].Value.Replace("\n", "");
                    amount_string = amount_string.Replace("\r", "");
                    var amount_details = amount_string.Split(' ');

                    if (amount_details.Count() > 1)
                    {
                        decimal amount = 0;
                        if (decimal.TryParse(amount_details.First(), NumberStyles.Currency, MyNFI, out amount))
                            Amount = amount;

                        Currency = amount_details.LastOrDefault();
                    }

                    Console.WriteLine("Paypal Product Quantity={0}, Amount={1} ", Quantity, Amount);
                    break;
                }
            }

            return Quantity > 0;

        }

        
        bool ParseAddress(string html)
        {
            Match match = Regex.Match(html, @"<[^<>]*AddressDisplayWrapper[^<>]*>\s*(.*)\s*<[^<>]*AddressDisplayWrapper[^<>]*>");
            if (match.Success && match.Groups.Count > 1)
            {
                var blocks = Regex.Matches(match.Groups[1].Value, @">([^<>]+?)<");
                List<string> address = new List<string>();
                int buyer_name_index = 0;
                foreach (Match block in blocks)
                {
                    if ((FirstName != null && block.Groups[1].Value.IndexOf(FirstName, StringComparison.OrdinalIgnoreCase) >= 0) ||
                        (LastName != null && block.Groups[1].Value.IndexOf(LastName, StringComparison.OrdinalIgnoreCase) >= 0))
                        buyer_name_index = address.Count;
                    address.Add(block.Groups[1].Value);
                }

                if (buyer_name_index >= 0)
                {
                    int country_index = (buyer_name_index + address.Count - 1) % address.Count;
                    Country = address[country_index];
                }
                Console.WriteLine( "Paypal Address: Country '{0}'", Country);
                return true;
            }
            return false;
        }

       

        public static void Main(string[] args)
        {
            string html = File.ReadAllText("d:/fga_master/apps/tools/emails/Item - Payment received from Ivica Tezak (ivek@alice-dsl.de).html");

            PaypalOrder order = new PaypalOrder("FreeYouTubeDownload", 98);

            var val = order.ParseEmail(html, DateTime.Now);

            var result = order.Place("http://localhost:60001/Api/Order/KeyGenerator", "FreeYouTubeDownload");
        }
    }
}
