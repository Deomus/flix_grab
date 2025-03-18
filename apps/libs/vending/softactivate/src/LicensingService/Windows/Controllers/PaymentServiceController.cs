using System;
using System.Collections.Generic;
using System.Configuration;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Web.Http;
using SoftActivate.Licensing;
using System.Text;
using Licensing.Web.DAL;
using Licensing.Web.Models;
using System.Web.Mvc;

namespace LicensingService.Controllers
{
    public class PaymentServiceController : LicensingServiceController
    {
        protected LicensingServiceDbContext db = new LicensingServiceDbContext();

        public string[] ProcessKeyGenerationRequest(string requestId,
                                                    string productId,
                                                    int count,
                                                    decimal totalPrice,
                                                    string currency,
                                                    string firstName,
                                                    string lastName,
                                                    string companyName,
                                                    string email,
                                                    string phoneNumber,
                                                    string street,
                                                    string zipCode,
                                                    string city,
                                                    string state,
                                                    string countryCode,
                                                    bool sendOrderEmail = false,
                                                    string paymentServiceAccountId = null)
        {
            try
            {
                PaymentServiceMapping mapping = db.PaymentServiceMappings.Where(m => m.PaymentServiceProductId == productId).FirstOrDefault();
                if (mapping == null)
                    throw new Exception("invalid product id '" + productId + "'");

                if (!String.IsNullOrEmpty(mapping.PaymentServiceAccountId))
                {
                    if (String.IsNullOrEmpty(paymentServiceAccountId))
                        throw new Exception("invalid payment account id");

                    if (mapping.PaymentServiceAccountId.ToLower() != paymentServiceAccountId.ToLower())
                        throw new Exception("invalid payment account id");
                }

                Product p = db.Products.Find(mapping.ProductId);

                if (p == null)
                {
                    EventLog.Log("Invalid product id", "Product id '" + productId + "' is invalid", EventType.Error, "PaymentServiceKeyGenerator");
                    throw new Exception("invalid product id");
                }

                if (!String.IsNullOrEmpty(requestId))
                {
                    Order order = db.Orders.Where(o => o.TransactionId == requestId).FirstOrDefault();
                    if (order != null)
                    {
                        EventLog.Log("Duplicate order id " + requestId, null, EventType.Error);
                        throw new Exception("duplicate order id");
                    }
                }

                string registrationName = String.IsNullOrEmpty(lastName) ? firstName : firstName + " " + lastName;

                string[] licenseKeys = GenerateLicenseKeys(count, p.LicenseKeyTemplate, p.Id, registrationName, email, (p.DefaultLicenseDuration > 0) ? DateTime.Now.AddDays(p.DefaultLicenseDuration.Value) : DateTime.Now);

                if (sendOrderEmail)
                {
                    //
                    // Add the license key and product name as template parameters, and send the email. 
                    // The TemplateMailer class will replace the placeholders in the email (eg. '##LICENSE_KEY##' occurences) with these parameters
                    // Note that the TemplateMailer will also send a BCC to your email address (you can disable that by setting the bcc parameter to null)
                    //

                    Dictionary<string, string> parameters = new Dictionary<string, string>();

                    parameters.Add("REGISTRATION_EMAIL", email ?? "");
                    parameters.Add("REGISTRATION_NAME", registrationName ?? "");
                    parameters.Add("LICENSE_KEY", String.Join(Environment.NewLine, licenseKeys));
                    parameters.Add("PRODUCT_NAME", p.Name);

                    TemplateMailer mailer = new TemplateMailer(p.SupportEmail,
                                                               ConfigurationManager.AppSettings["SmtpServer"],
                                                               Int32.Parse(ConfigurationManager.AppSettings["SmtpPort"]),
                                                               ConfigurationManager.AppSettings["SmtpUsername"],
                                                               ConfigurationManager.AppSettings["SmtpPassword"],
                                                               Boolean.Parse(ConfigurationManager.AppSettings["SmtpSsl"]));

                    mailer.SendEmail(p.OrderEmailTemplate, parameters, email, p.SupportEmail);
                }

                try
                {
                    Order order = new Order();

                    if (!String.IsNullOrWhiteSpace(requestId))
                        order.TransactionId = requestId;

                    order.OrderTotal = totalPrice;
                    order.OrderTotalExclFees = totalPrice;
                    order.Currency = currency;
                    order.FirstName = firstName;
                    order.LastName = lastName;
                    order.Company = companyName;
                    order.Email = email;
                    order.PhoneNumber = phoneNumber;
                    order.Address = street;
                    order.ZipCode = zipCode;
                    order.City = city;
                    order.State = state;
                    order.CountryCode = countryCode;
                    order.Date = DateTime.UtcNow;

                    db.Orders.Add(order);

                    db.SaveChanges();

                    OrderItem orderItem = new OrderItem();

                    orderItem.OrderId = order.Id;
                    orderItem.ProductId = p.Id;
                    orderItem.Quantity = (short)count;

                    db.OrderItems.Add(orderItem);

                    db.SaveChanges();

                    for (int i = 0; i < licenseKeys.Count(); i++)
                    {
                        LicenseKey licenseKey = new LicenseKey();
                        licenseKey.OrderItemId = orderItem.Id;
                        licenseKey.ProductId = p.Id;
                        licenseKey.DateAdded = DateTime.UtcNow;
                        licenseKey.Key = licenseKeys[i];

                        db.LicenseKeys.Add(licenseKey);
                    }

                    db.SaveChanges();
                }
                catch (Exception ex)
                {
                    EventLog.Log("Exception while adding order info to database", ex.Message + ((ex.InnerException != null) ? "; " + ex.InnerException.Message : ""), EventType.Error);

                    // We want to avoid sending multiple order emails in case the order request is repeated due to an error at the database level
                    // so if the database insertion fails, we only throw an exception (and expect a retry from the order server) if we didn't send an order email to the customer
                    // if we sent an order email and database ops fail, we just let the admin know, and let him handle the situation
                    if (!sendOrderEmail)
                        throw;
                }

                return licenseKeys;
            }
            catch (Exception ex)
            {
                EventLog.Log("Payment Service Error", ex.Message + ((ex.InnerException != null) ? "; " + ex.InnerException.Message : ""), EventType.Error);
                throw;
            }
        }

        public void ProcessOrderNotificationRequest(string requestId,
                                                    string productId,
                                                    int quantity,
                                                    decimal totalPrice,
                                                    string currency,
                                                    string firstName,
                                                    string lastName,
                                                    string companyName,
                                                    string email,
                                                    string phoneNumber,
                                                    string street,
                                                    string zipCode,
                                                    string city,
                                                    string state,
                                                    string countryCode)
        {

        }

        static public string[] GenerateLicenseKeys(int count, string licenseKeyTemplate, int productId, string name, string email, DateTime expirationDate)
        {
            // Generate a license key with SoftActivate Licensing SDK, according to the template found in the Products table
            if (!String.IsNullOrEmpty(ConfigurationManager.AppSettings["SDKLicenseKey"]))
                SDKRegistration.SetLicenseKey(ConfigurationManager.AppSettings["SDKLicenseKey"]);

            LicenseTemplate template = new LicenseTemplate(licenseKeyTemplate);
            KeyGenerator generator = new KeyGenerator(template);
            KeyValidator validator = new KeyValidator(template);

            LicenseKeyFieldType type;
            int size;
            int start;

            if (template.GetDataField("ProductId", out type, out size, out start))
            {
                int bits = 32;

                while (bits-- > 0)
                {
                    if ((productId & (1 << bits)) != 0)
                        break;
                }

                if (bits + 1 > size)
                    throw new Exception("The ProductId field of the license key template (" + size + " bits) is too small to hold a product id of " + productId);

                generator.SetKeyData("ProductId", productId);
            }

            if (template.GetDataField("ExpirationDate", out type, out size, out start))
            {
                if (size < 13)
                    throw new Exception("The 'ExpirationDate' template field is smaller than the required 13 bits");

                generator.SetKeyData("ExpirationDate", expirationDate);
            }

            if (template.GetValidationDataField("RegistrationName", out type, out size, out start))
            {
                if (String.IsNullOrEmpty(name))
                    throw new Exception("Customer name empty or not provided but it was needed for license key generation according to the license key template");

                if (Encoding.UTF8.GetBytes(name).Length * 8 > size)
                    throw new Exception("The received customer name is longer than the RegistrationName validation field from the provided key template");

                generator.SetValidationData("RegistrationName", name);
                validator.SetValidationData("RegistrationName", name);
            }

            if (template.GetValidationDataField("RegistrationEmail", out type, out size, out start))
            {
                if (String.IsNullOrEmpty(email))
                    throw new Exception("Customer email empty or not provided but it was needed for license key generation according to the license key template");

                if (Encoding.UTF8.GetBytes(email).Length * 8 > size)
                    throw new Exception("The received customer email is longer than the RegistrationEmail validation field from the provided key template");

                generator.SetValidationData("RegistrationEmail", email);
                validator.SetValidationData("RegistrationEmail", email);
            }

            string[] licenseKeys = new string[count];

            for (int i = 0; i < count; i++)
            {
                licenseKeys[i] = generator.GenerateKey();
                validator.SetKey(licenseKeys[i]);
                if (!validator.IsKeyValid())
                {
                    throw new Exception("Invalid Generated License Key '" + licenseKeys[i] + "' for product id '" + productId + "'");
                }
            }

            return licenseKeys;
        }
    }
}
