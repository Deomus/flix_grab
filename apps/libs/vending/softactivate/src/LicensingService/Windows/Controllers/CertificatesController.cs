using Licensing.Web.DAL;
using Licensing.Web.Models;
using LicensingService.Filters;
using SoftActivate.Licensing;
using System;
using System.Collections.Generic;
using System.Configuration;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Text;
using System.Web;
using System.Web.Http;

namespace LicensingService.Controllers
{
    public class CertificatesController : LicensingServiceController
    {
        [HttpGet]
        public HttpResponseMessage Issuer(string csr, string sdkLicenseKey = null)
        {
            EventLog.Log("Certificate Requested", null, EventType.Information, "CertAuth"); 

            LicensingServiceDbContext db = new LicensingServiceDbContext();

            if (!SDKRegistration.IsRegistered)
            {
                SDKRegistration.SetLicenseKey(ConfigurationManager.AppSettings["SDKLicenseKey"]);
            }

            string signingPrivateKey = db.Configuration.Where(r => r.Name == "RootPrivateKey").SingleOrDefault().Value;
            string signingCertificate = db.Configuration.Where(r => r.Name == "RootCertificate").SingleOrDefault().Value;

            DateTime certExpirationDate = DateTime.UtcNow.AddDays(90);

            if (!String.IsNullOrEmpty(sdkLicenseKey))
            {
                SDKLicensingAuthorizer authorizer = new SDKLicensingAuthorizer(sdkLicenseKey);
                if (authorizer.Edition != LicensingSDKEdition.Demo)
                {
                    /*
                    if (authorizer.ExpirationDate < DateTime.UtcNow)
                    {
                        EventLog.Log("Failed to issue certificate (expired license key)", "License key: " + sdkLicenseKey + ", expirationDate: " + authorizer.ExpirationDate.ToString(), EventType.Warning, "CertAuth");
                        
                        HttpResponseMessage exitMessage = new HttpResponseMessage(HttpStatusCode.Forbidden);
                        exitMessage.ReasonPhrase = "The SDK license key cannot be used with this build of the SDK because license validity has expired. You can however use it with builds released before the expiration";
                        return exitMessage;
                    }
                    */
                }

                LicenseKey licenseKeyRecord = db.LicenseKeys.Where(l => l.Key == sdkLicenseKey).FirstOrDefault();
                if (licenseKeyRecord != null)
                    if (!licenseKeyRecord.Active)
                    {
                        EventLog.Log("Failed to issue certificate (blocked license key)", "License key: " + sdkLicenseKey, EventType.Warning, "CertAuth");
                        HttpResponseMessage exitMessage = new HttpResponseMessage(HttpStatusCode.Forbidden);
                        exitMessage.ReasonPhrase = "invalid license key. Please contact technical support";
                        return exitMessage;
                    }
            }

            if (String.IsNullOrEmpty(signingCertificate) || String.IsNullOrEmpty(signingPrivateKey))
            {
                EventLog.Log("Failed to issue certificate (no signing keys)", null, EventType.Warning, "CertAuth");
                
                throw new HttpException(500, "No signing certificate and private key available");
            }

            Certificate certificate = new Certificate(csr);

            string cert = Certificate.Sign(certificate.ToString(),
                                           null,
                                           signingPrivateKey,
                                           signingCertificate, 
                                           null, 
                                           DateTime.UtcNow.AddDays(60));

            var resp = new HttpResponseMessage(HttpStatusCode.OK);
            resp.Content = new StringContent(cert, Encoding.ASCII, "text/plain");

            EventLog.Log("Certificate Issued", "License Key: " + sdkLicenseKey, EventType.Information, "CertAuth"); 

            return resp;
        }
    }
}
