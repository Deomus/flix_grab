using System;
using System.Data.Entity;
using System.Data.Entity.Infrastructure;
using System.Threading;
using System.Web.Mvc;
using WebMatrix.WebData;
using Licensing.ControlPanel.Models;
using System.Web.Security;
using Licensing.Web.DAL;

namespace LicensingService.Filters
{
    [AttributeUsage(AttributeTargets.Class | AttributeTargets.Method, AllowMultiple = false, Inherited = true)]
    public sealed class MvcInitializeLicensingDatabaseAttribute : System.Web.Mvc.ActionFilterAttribute
    {
        public override void OnActionExecuting(ActionExecutingContext filterContext)
        {
            // Ensure the licensing database is initialized only once per app start
            //LicensingDatabaseInitializer.EnsureInitialized();
        }
    }
}
