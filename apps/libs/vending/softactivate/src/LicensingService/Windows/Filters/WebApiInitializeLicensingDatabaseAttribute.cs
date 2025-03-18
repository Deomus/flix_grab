using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Http.Controllers;
using Licensing.Web.DAL;

namespace LicensingService.Filters
{
    [AttributeUsage(AttributeTargets.Class | AttributeTargets.Method, AllowMultiple = false, Inherited = true)]
    public class WebApiInitializeLicensingDatabaseAttribute : System.Web.Http.Filters.ActionFilterAttribute
    {
        public override void OnActionExecuting(HttpActionContext filterContext)
        {
            //LicensingDatabaseInitializer.EnsureInitialized();
        }
    }
}
