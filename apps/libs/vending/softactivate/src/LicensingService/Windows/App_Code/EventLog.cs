using Licensing.Web.DAL;
using Licensing.Web.Models;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace LicensingService
{
    public enum EventType
    {
        Information = 0,
        Warning = 1,
        Error = 2,
        EmailNotification = 16
    }

    public class EventLog
    {
        public static void Log(string subject, string message = null, EventType eventType = EventType.Information, string source = null, string url = null, string urlparams = null, string ipAddress = null)
        {
            try
            {
                LicensingServiceDbContext db = new LicensingServiceDbContext();

                Event e = new Event();

                e.Type = (byte)eventType;
                e.Subject = subject;
                e.Message = message;
                
                e.Url = url;

                if (String.IsNullOrWhiteSpace(url))
                    if (HttpContext.Current != null)
                        if (HttpContext.Current.Request != null)
                        {
                            if (HttpContext.Current.Request.Url != null)
                                e.Url = HttpContext.Current.Request.Url.AbsoluteUri;

                            if (HttpContext.Current.Request.UrlReferrer != null)
                            {
                                e.Url += " (referrer: " + HttpContext.Current.Request.UrlReferrer.AbsoluteUri + ")";
                            }
                        }

                e.UrlParams = urlparams;

                if (String.IsNullOrWhiteSpace(urlparams))
                    if (HttpContext.Current != null)
                        if (HttpContext.Current.Request != null)
                            if (HttpContext.Current.Request.QueryString != null)
                                e.UrlParams = HttpContext.Current.Request.QueryString.ToString();
                
                e.IPAddress = ipAddress;

                if (String.IsNullOrWhiteSpace(ipAddress))
                    if (HttpContext.Current != null)
                        if (HttpContext.Current.Request != null)
                            e.IPAddress = HttpContext.Current.Request.UserHostAddress;

                string locationStr = null;

                if (!String.IsNullOrWhiteSpace(e.IPAddress))
                {
                    LookupService geoIPService = (LookupService)HttpContext.Current.Application["GeoIPService"];
                    
                    if (geoIPService != null)
                    {
                        Location location = null;

                        try
                        {
                            location = geoIPService.getLocation(e.IPAddress);
                            locationStr = "\"Unknown Country (Unknown City)\"";
                        }
                        catch (Exception)
                        {

                        }

                        if (location != null)
                        {
                            locationStr = "\"";
                            locationStr += string.IsNullOrEmpty(location.countryName) ? "Unknown Country" : location.countryName;
                            locationStr += " (";
                            locationStr += string.IsNullOrEmpty(location.city) ? "Unknown City" : location.city;
                            locationStr += ")\"";

                            e.Location = locationStr;
                        }
                    }
                }

                e.Date = DateTime.UtcNow;

                db.EventLog.Add(e);

                db.SaveChanges();

                bool notifyAllErrors = false;
                ConfigurationItem item = db.Configuration.Where(c => c.Name == "EmailNotificationOnErrors").SingleOrDefault();
                if (item != null)
                    if (item.Value == "1" || item.Value.ToLower() == "true")
                        notifyAllErrors = true;
                
                if ((eventType & EventType.EmailNotification) != 0 || 
                    ((eventType & EventType.Error) != 0 && notifyAllErrors))
                {
                    //TemplateMailer mailer = new TemplateMailer(null, null, 25, null, null, false);
                    item = db.Configuration.Where(c => c.Name == "EventLogNotificationEmail").SingleOrDefault();
                    
                    if (item != null)
                        if (!String.IsNullOrWhiteSpace(item.Value))
                        {
                            if (HttpContext.Current != null)
                                if (HttpContext.Current.Application != null)
                                {
                                    TemplateMailer mailer = (TemplateMailer)HttpContext.Current.Application["TemplateMailer"];
                                    if (mailer != null)
                                    {
                                        string body = subject + "\n" + message + "\nSource:" + source + "\nIP Address:" + e.IPAddress + "\nLocation: " + locationStr;

                                        TemplateMailer.SendEmailCaller caller = new TemplateMailer.SendEmailCaller(mailer.SendEmail);
                                        caller.BeginInvoke(item.Value, "[Licensing Service] " + subject, body, null, null, null, null, null);
                                    }
                                }
                        }
                }
            }
            catch (Exception)
            {

            }
        }
    }
}
