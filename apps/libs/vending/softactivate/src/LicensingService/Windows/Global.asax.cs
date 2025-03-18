using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data.Entity;
using System.IO;
using System.Linq;
using System.Text;
using System.Web;
using System.Web.Http;
using System.Web.Mvc;
using System.Web.Optimization;
using System.Web.Routing;
using WebMatrix.WebData;
using LicensingService.Filters;

namespace LicensingService
{
    // Note: For instructions on enabling IIS6 or IIS7 classic mode, 
    // visit http://go.microsoft.com/?LinkId=9394801

    public class MvcApplication : System.Web.HttpApplication
    {
        protected void Application_Start()
        {
            AreaRegistration.RegisterAllAreas();
            
            //MVC4
            //WebApiConfig.Register(GlobalConfiguration.Configuration);
            GlobalConfiguration.Configure(WebApiConfig.Register);

            FilterConfig.RegisterGlobalFilters(GlobalFilters.Filters);
            RouteConfig.RegisterRoutes(RouteTable.Routes);

            BundleConfig.RegisterBundles(BundleTable.Bundles);
            AuthConfig.RegisterAuth();

            Application["GeoIPService"] = new LookupService(Server.MapPath("~/") + "App_Data\\GeoIPCity.dat");

            Application["TemplateMailer"] = new TemplateMailer(ConfigurationManager.AppSettings["SiteEmail"],
                                                               ConfigurationManager.AppSettings["SmtpServer"],
                                                               Int32.Parse(ConfigurationManager.AppSettings["SmtpPort"]),
                                                               ConfigurationManager.AppSettings["SmtpUsername"],
                                                               ConfigurationManager.AppSettings["SmtpPassword"],
                                                               Boolean.Parse(ConfigurationManager.AppSettings["SmtpSsl"]));

            Application["SDKLicensingAuthorizer"] = new SDKLicensingAuthorizer();
        }

        protected void Application_BeginRequest(object sender, EventArgs e)
        {
            HttpContext context = base.Context;
        }

        protected void Application_Error(object sender, EventArgs e)
        {
            Exception exception = Server.GetLastError();
            string message = "";
            int depth = 0;

            while (exception != null && depth < 10)
            {
                message = message + exception.Message + "; ";
                exception = exception.InnerException;
                depth++;
            }

            string ip = null;
            string url = null;

            try
            {
                if (HttpContext.Current != null && HttpContext.Current != null)
                {
                    ip = HttpContext.Current.Request.UserHostAddress;
                    url = HttpContext.Current.Request.Url.AbsoluteUri;
                }
            } catch (Exception)
            {

            }

            EventLog.Log("Unhandled exception caught", message, EventType.Error, "Global");
        }
    }
}
