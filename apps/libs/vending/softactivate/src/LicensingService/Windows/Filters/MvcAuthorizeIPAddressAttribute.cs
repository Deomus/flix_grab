using System;
using System.Collections.Generic;
using System.Configuration;
using System.Linq;
using System.Net.Http;
using System.Web;
using System.Web.Mvc;

namespace LicensingService.Filters
{
    public class MvcAuthorizeIPAddressAttribute : System.Web.Mvc.ActionFilterAttribute
    {
        public MvcAuthorizeIPAddressAttribute(string appSettingSection)
        {
            m_addresses = Convert.ToString(ConfigurationManager.AppSettings[appSettingSection]);
        }

        public override void OnActionExecuting(ActionExecutingContext filterContext)
        {
            //Get users IP Address 
            string ipAddress = HttpContext.Current.Request.UserHostAddress;

            //EventLog.Log(EventType.Information, "RegNow", "RegNow Key Generator Called", null, filterContext.Request.RequestUri.AbsoluteUri, null, ipAddress);

            if (!AuthorizeIPAddressAttributeBase.IsIpAddressAllowed(ipAddress.Trim(), m_addresses))
            {
                //Send back a HTTP Status code of 403 Forbidden  
                throw new HttpException(403, "Forbidden");
            }

            base.OnActionExecuting(filterContext);
        }

        private string m_addresses;
    }
}
