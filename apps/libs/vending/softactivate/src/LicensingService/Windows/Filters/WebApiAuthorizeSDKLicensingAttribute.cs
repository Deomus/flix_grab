using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Http;
using System.Web;
using System.Web.Http.Controllers;

namespace LicensingService.Filters
{
    public class WebApiAuthorizeSDKLicensingAttribute : System.Web.Http.Filters.ActionFilterAttribute
    {
        public WebApiAuthorizeSDKLicensingAttribute(LicensingSDKEdition editions)
        {
            this.editions = editions;
        }

        public override void OnActionExecuting(HttpActionContext filterContext)
        {
            SDKLicensingAuthorizer authorizer = HttpContext.Current.Application["SDKLicensingAuthorizer"] as SDKLicensingAuthorizer;

            if (authorizer == null)
            {
                EventLog.Log("Error checking the SDK licensing. Is the SDK license key set in web.config ?", null, EventType.Error, "SDK Licensing");
                filterContext.Response = new HttpResponseMessage(System.Net.HttpStatusCode.Forbidden);
            }

            if ((authorizer.Edition & editions) == 0)
                filterContext.Response = new HttpResponseMessage(System.Net.HttpStatusCode.Forbidden);
        }

        LicensingSDKEdition editions;
    }
}
