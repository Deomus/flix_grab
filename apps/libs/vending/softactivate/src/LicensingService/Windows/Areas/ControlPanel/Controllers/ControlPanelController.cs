using LicensingService;
using LicensingService.Filters;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;

namespace Licensing.ControlPanel.Controllers
{
    [MvcAuthorizeIPAddress("ControlPanelIPAddresses")]
    [Authorize]
    [MvcInitializeLicensingDatabase]
    public class ControlPanelController : Controller
    {
        protected override void OnActionExecuting(ActionExecutingContext filterContext)
        {
            SDKLicensingAuthorizer authorizer = filterContext.HttpContext.Application["SDKLicensingAuthorizer"] as SDKLicensingAuthorizer;

            if (authorizer == null)
            {
                EventLog.Log("Error checking the SDK licensing. Is the SDK license key set in web.config ?", null, EventType.Error, "SDK Licensing");
                filterContext.Result = new HttpStatusCodeResult(403, "Error checking the SDK licensing");
            }

            ViewBag.LicensingSDKEdition = authorizer.Edition;

            base.OnActionExecuting(filterContext);
        }
    }
}
