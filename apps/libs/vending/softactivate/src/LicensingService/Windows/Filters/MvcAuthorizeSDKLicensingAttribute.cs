using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;

namespace LicensingService.Filters
{
    public class MvcAuthorizeSDKLicensingAttribute : System.Web.Mvc.ActionFilterAttribute
    {
        public MvcAuthorizeSDKLicensingAttribute(LicensingSDKEdition editions)
        {
            this.editions = editions;
        }

        public override void OnActionExecuting(ActionExecutingContext filterContext)
        {
            SDKLicensingAuthorizer authorizer = filterContext.HttpContext.Application["SDKLicensingAuthorizer"] as SDKLicensingAuthorizer;

            if (authorizer == null)
            {
                EventLog.Log("Error checking the SDK licensing. Is the SDK license key set in web.config ?", null, EventType.Error, "SDK Licensing");
                filterContext.Result = new HttpStatusCodeResult(403, "Error checking the SDK licensing");
            }

            if ((authorizer.Edition & editions) == 0)
            {
                EventLog.Log("Feature unavailable in this Licensing SDK edition", null, EventType.Error, "SDK Licensing");
                filterContext.Result = new HttpStatusCodeResult(403, "Feature unavailable in this Licensing SDK edition");
            }
        }

        LicensingSDKEdition editions;
    }
}
