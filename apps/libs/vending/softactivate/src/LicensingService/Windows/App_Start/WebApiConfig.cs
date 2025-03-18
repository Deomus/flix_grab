using System;
using System.Collections.Generic;
using System.Linq;
using System.Web.Http;
using System.Web.Routing;

namespace LicensingService
{
    public static class WebApiConfig
    {
        public static void Register(HttpConfiguration config)
        {
            config.MapHttpAttributeRoutes();

            config.Routes.MapHttpRoute(
                name: "DefaultApi",
                routeTemplate: "api/{controller}/{action}/{id}",
                defaults: new { id = RouteParameter.Optional }
            );

            config.Routes.MapHttpRoute(
                name: "LegacyActivationApi",
                routeTemplate: "Activate.ashx",
                defaults: new { controller = "Activation" }
            );

            config.Routes.MapHttpRoute(
                name: "LegacyPayPalApi",
                routeTemplate: "PayPal.ashx",
                defaults: new { controller = "PayPal", action="OrderNotification" }
            );

            config.Routes.MapHttpRoute(
                name: "LegacyRegNowApi",
                routeTemplate: "RegNow.ashx",
                defaults: new { controller = "RegNow", action="KeyGenerator" }
            );

            config.Routes.MapHttpRoute(
                name: "LegacyShareItApi",
                routeTemplate: "ShareIt.ashx",
                defaults: new { controller = "ShareIt", action="KeyGenerator" }
            );

            config.Routes.MapHttpRoute(
                name: "Downloads1",
                routeTemplate: "Downloads/{*url}",
                defaults: new { controller = "Downloads" }
            );

            config.Routes.MapHttpRoute(
                name: "DownloadsHEAD",
                routeTemplate: "Downloads/{*url}",
                defaults: new { controller = "Downloads", action = "Get" },
                constraints: new { httpMethod = new HttpMethodConstraint("HEAD") } );

            config.Routes.MapHttpRoute(
                name: "LegacyDownloadsApi",
                routeTemplate: "Download.ashx",
                defaults: new { controller = "Downloads", Action="DownloadLegacy" }
            );

            // Uncomment the following line of code to enable query support for actions with an IQueryable or IQueryable<T> return type.
            // To avoid processing unexpected or malicious queries, use the validation settings on QueryableAttribute to validate incoming queries.
            // For more information, visit http://go.microsoft.com/fwlink/?LinkId=279712.
            //config.EnableQuerySupport();
        }
    }
}
