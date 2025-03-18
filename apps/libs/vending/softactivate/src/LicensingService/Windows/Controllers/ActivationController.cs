using Licensing.Web.DAL;
using Licensing.Web.Models;
using LicensingService.Controllers;
using SoftActivate.Licensing;
using System;
using System.Configuration;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Web;
using System.Web.Http;

namespace LicensingService
{
    public class ActivationController : LicensingServiceController
    {
        private LicensingServiceDbContext db = new LicensingServiceDbContext();

        // GET api/<controller>
        public HttpResponseMessage Get(string licenseKey, string hardwareId, string licenseKeyValidationData = null, int productId = -1)
        {
            // a license key and a hardware id must be provided in order to generate an activation key
            if (string.IsNullOrEmpty(hardwareId))
            {
                Abort(HttpStatusCode.BadRequest, "Invalid activation query (1)", "Null or empty hardware id provided");
            }

            // a license key and a hardware id must be provided in order to generate an activation key
            if (string.IsNullOrEmpty(licenseKey))
            {
                Abort(HttpStatusCode.BadRequest, "Invalid activation query (2)", "Null or empty license key provided");
            }

            if (!string.IsNullOrEmpty(licenseKeyValidationData))
            {
                licenseKeyValidationData = Uri.UnescapeDataString(licenseKeyValidationData);
            }

            byte[] rawLicenseKeyValidationData = null;

            if (!String.IsNullOrEmpty(licenseKeyValidationData))
            {
                rawLicenseKeyValidationData = Convert.FromBase64String(licenseKeyValidationData);
            }

            if (productId == 0)
            {
                productId = -1;
            }

            DateTime minActivationDate = new DateTime(1970, 1, 1);
            int maxActivations = 1;//Int16.Parse(ConfigurationManager.AppSettings["MaximumActivationsPerLicenseKey"]);
            int maxUniqueHardwareIds = 0;
            int maxActivationsPerHardwareId = 0;
            int expirationDays = 0;//short.Parse(ConfigurationManager.AppSettings["ActivationExpirationDays"]);
            string xmlLicenseKeyTemplate = ConfigurationManager.AppSettings["LicenseKeyTemplate"];

            Product product = db.Products.Find(productId);
            if (product != null)
            {
                if (product.DefaultMaxUniqueHardwareIdsPerLicenseKey != null)
                    maxUniqueHardwareIds = product.DefaultMaxUniqueHardwareIdsPerLicenseKey.Value;

                if (product.DefaultMaxActivationsPerHardwareId != null)
                    maxActivationsPerHardwareId = product.DefaultMaxActivationsPerHardwareId.Value;

                if (product.DefaultMaxActivationsPerLicenseKey != null)
                    maxActivations = product.DefaultMaxActivationsPerLicenseKey.Value;
                
                if (product.DefaultLicenseDuration != null)
                    expirationDays = product.DefaultLicenseDuration.Value;

                if (product.LicenseKeyTemplate != null)
                    xmlLicenseKeyTemplate = product.LicenseKeyTemplate;
            }

            DateTime firstActivationDate = default(DateTime);

            var licenseKeyRecord = db.LicenseKeys.Where(info => info.Key == licenseKey).FirstOrDefault();
            if (licenseKeyRecord != null)
            {
                if (!licenseKeyRecord.Active)
                    Abort(HttpStatusCode.Forbidden, "invalid license key", "license key is blocked");

                if (product == null)
                    product = licenseKeyRecord.Product;

                if (product != null)
                    if (product.LicenseKeyTemplate != null)
                        xmlLicenseKeyTemplate = product.LicenseKeyTemplate;

                if (licenseKeyRecord.PerDeviceActivationLimit != null)
                    maxActivationsPerHardwareId = licenseKeyRecord.PerDeviceActivationLimit.Value;

                if (licenseKeyRecord.DeviceLimit != null)
                    maxUniqueHardwareIds = licenseKeyRecord.DeviceLimit.Value;

                if (licenseKeyRecord.ActivationLimit != null)
                    maxActivations = licenseKeyRecord.ActivationLimit.Value;

                if (licenseKeyRecord.LicenseDuration != null)
                    expirationDays = licenseKeyRecord.LicenseDuration.Value;

                if (licenseKeyRecord.LicenseHistoryDuration != null)
                    minActivationDate = DateTime.UtcNow.AddDays(0 - licenseKeyRecord.LicenseHistoryDuration.Value);

                Activation firstActivation = db.Activations.Where(a => a.LicenseKeyId == licenseKeyRecord.Id && a.HardwareId == hardwareId).OrderBy(a => a.ActivationDate).FirstOrDefault();
                if (firstActivation != null)
                {
                    firstActivationDate = firstActivation.ActivationDate;
                }
            }

            if (product == null)
                Abort(HttpStatusCode.BadRequest, "invalid activation query (3)", "invalid product id");

            if (!String.IsNullOrEmpty(ConfigurationManager.AppSettings["SDKLicenseKey"]))
                SDKRegistration.SetLicenseKey(ConfigurationManager.AppSettings["SDKLicenseKey"]);

            LicenseTemplate keyTemplate = new LicenseTemplate(xmlLicenseKeyTemplate);

            if (keyTemplate.DataSize < 16)
            {
                Abort(HttpStatusCode.InternalServerError, "Licensing server internal error (1)", "The data size of the license key template must be at least 16 bits in order to use activation");
            }

            if (!IsLicenseKeyValid(keyTemplate, licenseKey, rawLicenseKeyValidationData))
            {
                Abort(HttpStatusCode.Forbidden, "Invalid license key", "Invalid license key");
            }

            if (maxActivations > 0)
            {
                if (db.Activations.Count(a => a.LicenseKey.Key == licenseKey) >= maxActivations)
                    Abort(HttpStatusCode.Forbidden, "Activation limit exceeded for this key", "Activation limit exceeded for this key (too many activations)");
            }

            if (maxUniqueHardwareIds > 0)
            {
                if (db.Activations.Where(a => a.LicenseKey.Key == licenseKey && a.HardwareId != hardwareId && a.ActivationDate > minActivationDate)
                                .Select(a => a.HardwareId)
                                .Distinct()
                                .Count() >= maxUniqueHardwareIds)
                {
                    Abort(HttpStatusCode.Forbidden, "Activation limit exceeded for this key", "Activation limit exceeded for this key (too many hardware ids)");
                }
            }

            if (maxActivationsPerHardwareId > 0)
            {
                if (db.Activations.Count(a => a.LicenseKey.Key == licenseKey && a.HardwareId == hardwareId && a.ActivationDate > minActivationDate) >= maxActivationsPerHardwareId)
                    Abort(HttpStatusCode.Forbidden, "Activation limit exceeded for this key", "Activation limit exceeded for this key (too many activations for the same hardware id)");
            }

            LicenseTemplate activationKeyTemplate = new LicenseTemplate(keyTemplate.NumberOfGroups,
                                                                keyTemplate.CharactersPerGroup,
                                                                keyTemplate.GetPublicKeyCertificate(),
                                                                keyTemplate.GetPrivateKey(),
                                                                keyTemplate.SignatureSize,
                                                                keyTemplate.DataSize,
                                                                "ExpirationDate");

            activationKeyTemplate.ValidationDataSize = (licenseKey.Length + hardwareId.Length) * 8;

            activationKeyTemplate.AddValidationField("LicenseKey", LicenseKeyFieldType.String, 8 * licenseKey.Length, 0);
            activationKeyTemplate.AddValidationField("HardwareId", LicenseKeyFieldType.String, 8 * hardwareId.Length, 8 * licenseKey.Length);

            if (licenseKeyValidationData != null)
            {
                activationKeyTemplate.ValidationDataSize += rawLicenseKeyValidationData.Length * 8;
                activationKeyTemplate.AddValidationField("LicenseKeyValidationData", LicenseKeyFieldType.Raw, rawLicenseKeyValidationData.Length * 8, 8 * (licenseKey.Length + hardwareId.Length));
            }

            KeyGenerator generator = new KeyGenerator(activationKeyTemplate);

            DateTime expDate = default(DateTime);

            if (expirationDays > 0)
            {
                // the license will expire in one year
                expDate = (firstActivationDate != default(DateTime)) ? firstActivationDate.AddDays(expirationDays) : DateTime.UtcNow.AddDays(expirationDays);

                if (expDate < DateTime.UtcNow)
                {
                    Abort(HttpStatusCode.Forbidden, "Activation timeframe exceeded for this computer and license key", "Activation duration exceeded for this computer. First activation: " + firstActivationDate + ", expirationDays: " + expirationDays);
                }

                ushort expDateBits = (ushort)(((expDate.Year - 2000) << 9) | (expDate.Month << 5) | expDate.Day);
                byte[] rawExpDate = new byte[2] { (byte)(expDateBits >> 8), (byte)(expDateBits & 0xFF) };
                generator.SetKeyData("ExpirationDate", rawExpDate);
            }

            generator.SetValidationData("LicenseKey", licenseKey);
            generator.SetValidationData("HardwareId", hardwareId);

            if (licenseKeyValidationData != null)
                generator.SetValidationData("LicenseKeyValidationData", rawLicenseKeyValidationData);

            string activationKey = generator.GenerateKey();

            // validate the generated key just to be sure
            KeyValidator validator = new KeyValidator(activationKeyTemplate, activationKey);

            validator.SetValidationData("LicenseKey", licenseKey);
            validator.SetValidationData("HardwareId", hardwareId);

            if (licenseKeyValidationData != null)
                validator.SetValidationData("LicenseKeyValidationData", rawLicenseKeyValidationData);

            if (!validator.IsKeyValid())
            {
                Abort(HttpStatusCode.InternalServerError, "Activation failure. Please contact technical support", "Activation failure. Activation key validation failed.");
            }

            string clientIPAddress = HttpContext.Current.Request.ServerVariables["REMOTE_ADDR"];
            if (clientIPAddress == null)
                clientIPAddress = "";

            // register the activation event into the database

            if (licenseKeyRecord == null)
            {
                licenseKeyRecord = new LicenseKey();
                licenseKeyRecord.Key = licenseKey;
                licenseKeyRecord.ValidationData = licenseKeyValidationData;
                licenseKeyRecord.ProductId = product.Id;
                licenseKeyRecord.DateAdded = DateTime.UtcNow;
                licenseKeyRecord.ActivationCount = 1;
                db.LicenseKeys.Add(licenseKeyRecord);
            }
            else
            {
                licenseKeyRecord.ActivationCount = (licenseKeyRecord.ActivationCount == null) ? 1 : licenseKeyRecord.ActivationCount + 1;
                licenseKeyRecord.LastActivationTime = DateTime.UtcNow;

                db.Entry(licenseKeyRecord).State = System.Data.Entity.EntityState.Modified;
            }

            db.SaveChanges();

            Activation activationRecord = new Activation();
            activationRecord.ActivationKey = activationKey;
            activationRecord.LicenseKeyId = licenseKeyRecord.Id;
            activationRecord.HardwareId = hardwareId;
            activationRecord.ExpirationDate = (expirationDays > 0) ? expDate : (DateTime?)null;
            activationRecord.ActivationDate = DateTime.UtcNow;
            activationRecord.IPAddress = clientIPAddress;

            db.Activations.Add(activationRecord);
            db.SaveChanges();

            return new HttpResponseMessage()
            {
                Content = new StringContent(activationKey, System.Text.Encoding.UTF8, "text/plain")
            };
        }

        // GET api/<controller>/5
        public string Get(int id)
        {
            return "value";
        }

        // POST api/<controller>
        public void Post([FromBody]string value)
        {

        }
        
        // PUT api/<controller>/5
        public void Put(int id, [FromBody]string value)
        {

        }
/*
        public HttpResponseMessage Delete(string activationKey)
        {
            Activation activation = db.Activations.Where(a => a.ActivationKey == activationKey).FirstOrDefault();

            if (activation == null)
            {
                return new HttpResponseMessage(HttpStatusCode.Forbidden);
            }

            if (activation.IsActive == false)
                return new HttpResponseMessage(HttpStatusCode.Forbidden);

            if (activation.LicenseKey.ActivationCount > 0)
                activation.LicenseKey.ActivationCount--;

            activation.IsActive = false;
            activation.ExpirationDate = DateTime.UtcNow;

            db.Entry(activation).State = System.Data.Entity.EntityState.Modified;
            db.SaveChanges();

            return new HttpResponseMessage(HttpStatusCode.OK);
        }
*/

        private bool IsLicenseKeyValid(LicenseTemplate template, string licenseKey, byte[] licenseKeyValidationData)
        {
            KeyValidator validator = new KeyValidator(template, licenseKey);

            if (licenseKeyValidationData != null)
                validator.SetValidationData(null, licenseKeyValidationData);

            return validator.IsKeyValid();
        }

        void Abort(HttpStatusCode httpCode, string httpMessage, string logMessage)
        {
            EventLog.Log(httpMessage, logMessage, EventType.Error, "Activation");

            HttpResponseMessage response = new HttpResponseMessage(httpCode);
            response.ReasonPhrase = httpMessage;

            throw new HttpResponseException(response);
        }
    }
}
