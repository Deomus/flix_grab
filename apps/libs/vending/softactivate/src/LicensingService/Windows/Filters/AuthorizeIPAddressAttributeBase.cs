using System;
using System.Collections.Generic;
using System.Configuration;
using System.Linq;
using System.Net.Http;
using System.Web;
using System.Web.Http.Controllers;

namespace LicensingService.Filters
{
    internal class AuthorizeIPAddressAttributeBase
    {
        public static bool IsIpAddressAllowed(string ipAddress, string addressList)
        {
            //Split the users IP address into it's 4 octets (Assumes IPv4) 
            string[] incomingOctets = ipAddress.Trim().Split(new char[] { '.' });

            //Get the valid IP addresses from the web.config 

            //Store each valid IP address in a string array 
            string[] validIpAddresses = addressList.Trim().Split(new char[] { ',' });

            //Iterate through each valid IP address 
            foreach (var validIpAddress in validIpAddresses)
            {
                //Return true if valid IP address matches the users 
                if (validIpAddress.Trim() == ipAddress)
                {
                    return true;
                }

                //Split the valid IP address into it's 4 octets 
                string[] validOctets = validIpAddress.Trim().Split(new char[] { '.' });

                bool matches = true;

                //Iterate through each octet 
                for (int index = 0; index < validOctets.Length; index++)
                {
                    //Skip if octet is an asterisk indicating an entire 
                    //subnet range is valid 
                    if (validOctets[index] != "*")
                    {
                        if (validOctets[index] != incomingOctets[index])
                        {
                            matches = false;
                            break; //Break out of loop 
                        }
                    }
                }

                if (matches)
                {
                    return true;
                }
            }

            //Found no matches 
            return false;
        }
    }
}