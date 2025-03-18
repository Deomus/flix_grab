using Licensing.Web.DAL;
using Licensing.Web.Models;
using LicensingService.Controllers;
using SoftActivate.Licensing;
using System;
using System.Collections.Generic;
using System.Configuration;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Net.Http.Headers;
using System.Web;
using System.Web.Http;

namespace LicensingService.Controllers
{
    public class DownloadsController : LicensingServiceController
    {
        private LicensingServiceDbContext db = new LicensingServiceDbContext();

        [AcceptVerbs(new [] { "GET", "HEAD" })]
        public HttpResponseMessage Get(string url, string licenseKey="", string licenseKeyValidationData = "")
        {
            try {
                Download download = db.Downloads.Where(d => d.Url == url).FirstOrDefault();
                if (download == null)
                    download = db.Downloads.Where(d => d.LocalPath == url).FirstOrDefault();

                if (download == null)
                {
                    EventLog.Log("file download denied (download not found for url " + url + ")", null, EventType.Error);
                    return new HttpResponseMessage(HttpStatusCode.NotFound);
                }

                if (download.IsPublic == false)
                {
                    int licenseDuration = 0;
                    bool isKeyInDatabase = false;
                    DateTime expirationDate = default(DateTime);

                    if (String.IsNullOrWhiteSpace(licenseKey))
                    {
                        EventLog.Log("file download denied (no license key provided for non-public download)", null, EventType.Error);
                        return new HttpResponseMessage(HttpStatusCode.NotFound);
                    }

                    // it is not mandatory for the license key to be in the database in order to be able to validate the download
                    // if not found in database, we just validate the provided license key against the license key template
                    // of the product designated by download.ProductId
                    
                    LicenseKey licenseKeyRecord = db.LicenseKeys.Where(l => l.Key == licenseKey).FirstOrDefault();
                    if (licenseKeyRecord != null)
                    {
                        if (!licenseKeyRecord.Active)
                        {
                            EventLog.Log("file download denied (license key is blocked)", null, EventType.Error);
                            return new HttpResponseMessage(HttpStatusCode.NotFound);
                        }

                        if (licenseKeyRecord.ProductId != download.ProductId)
                        {
                            EventLog.Log("file download denied (provided license key is not for the product id " + download.ProductId + " )", null, EventType.Error);
                            return new HttpResponseMessage(HttpStatusCode.NotFound);
                        }

                        isKeyInDatabase = true;

                        if (licenseKeyRecord.LicenseDuration != null)
                            licenseDuration = licenseKeyRecord.LicenseDuration.Value;
                        else
                            if (licenseKeyRecord.Product.DefaultLicenseDuration != null)
                                licenseDuration = licenseKeyRecord.Product.DefaultLicenseDuration.Value;

                        if (!String.IsNullOrWhiteSpace(licenseKeyRecord.ValidationData))
                            licenseKeyValidationData = licenseKeyRecord.ValidationData;
                    }

                    // if license key is in database, try to find the order that issued it and determine the expiration date from the order date
                    // and license duration

                    if (isKeyInDatabase)
                    {
                        Order order = db.Orders.Where(o => o.OrderItems.Any(i => i.LicenseKeys.Any(l => l.Key == licenseKey))).FirstOrDefault();
                        if (order != null)
                        {
                            expirationDate = (licenseDuration != 0) ? order.Date.AddDays(licenseDuration) : DateTime.Now.AddDays(1);

                            if (expirationDate < DateTime.UtcNow)
                            {
                                EventLog.Log("file download denied ( license is expired (2) )", null, EventType.Error);
                                return new HttpResponseMessage(HttpStatusCode.NotFound);
                            }
                        }
                    }
                    
                    // if the license key was not found in database, we need to validate it using the product license key template
                    // if we were not able to determine the expiration date until now, search inside the license key for it
                    if (!isKeyInDatabase || expirationDate == default(DateTime))
                    {                        
                        Product product = db.Products.Where(p => p.Id == download.ProductId).FirstOrDefault();

                        if (product == null)
                        {
                            EventLog.Log("file download denied (product not found with product id " + download.ProductId + ")", null, EventType.Error);
                            return new HttpResponseMessage(HttpStatusCode.NotFound);
                        }

                        if (String.IsNullOrWhiteSpace(product.LicenseKeyTemplate))
                        {
                            EventLog.Log("file download denied (cannot validate license key because no license key template was provided for the product with id " + download.ProductId + ")", null, EventType.Error);
                            return new HttpResponseMessage(HttpStatusCode.NotFound);
                        }

                        if (!String.IsNullOrEmpty(ConfigurationManager.AppSettings["SDKLicenseKey"]))
                            SDKRegistration.SetLicenseKey(ConfigurationManager.AppSettings["SDKLicenseKey"]);

                        LicenseTemplate template = new LicenseTemplate(product.LicenseKeyTemplate);

                        KeyValidator validator = new KeyValidator(template, licenseKey);

                        if (!String.IsNullOrWhiteSpace(licenseKeyValidationData))
                            validator.SetValidationData(null, Convert.FromBase64String(licenseKeyRecord.ValidationData));

                        if (!validator.IsKeyValid())
                        {
                            EventLog.Log("file download denied (license key not valid for product id " + download.ProductId + ")", null, EventType.Error);
                            return new HttpResponseMessage(HttpStatusCode.NotFound);
                        }

                        LicenseKeyFieldType fieldType; int fieldSize, fieldOffset;
                        if (template.GetDataField("ExpirationDate", out fieldType, out fieldSize, out fieldOffset))
                        {
                            expirationDate = validator.QueryDateKeyData("ExpirationDate");
                            if (expirationDate < DateTime.UtcNow)
                            {
                                EventLog.Log("file download denied (license key is expired)", null, EventType.Error);
                                return new HttpResponseMessage(HttpStatusCode.NotFound);
                            }
                        }
                    }
                }

                HttpResponseMessage result = new HttpResponseMessage(HttpStatusCode.OK);

                var stream = new FileStream(HttpContext.Current.Server.MapPath("~/App_Downloads/") + download.LocalPath, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);

                result.Content = new StreamContent(stream);

                result.Content.Headers.ContentType = new MediaTypeHeaderValue(String.IsNullOrEmpty(download.MimeType) ? "application/octet-stream" : download.MimeType);

                result.Content.Headers.Add("content-disposition", string.Format("attachment;FileName=\"{0}\"", Path.GetFileName(download.LocalPath)));

                EventType eventType = String.IsNullOrWhiteSpace(download.NotificationEmail) ? EventType.Information : EventType.Information | EventType.EmailNotification;

                EventLog.Log("Download Notification", "File '" + download.LocalPath + "' was downloaded", eventType, "DownloadManager");

                return result;
            }
            catch (Exception ex)
            {
                EventLog.Log("exception while downloading file", ex.Message + ", " + ((ex.InnerException != null) ? ex.InnerException.Message : ""), EventType.Error);
                throw;
            }
        }

        [HttpGet]
        public HttpResponseMessage DownloadLegacy(string file, string lk = "")
        {
            return Get(file, lk);
        }
    }
}
