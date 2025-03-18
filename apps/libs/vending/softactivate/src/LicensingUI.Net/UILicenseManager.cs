using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Threading;
using System.Windows.Forms;
using SoftActivate.Licensing;
using System.Xml;
using System.Reflection;
using System.Diagnostics;

namespace SoftActivate.Licensing
{
    public class UILicenseManager
    {
        public UILicenseManager(string xmlLicenseTemplate)
        {
            m_licenseTemplate = new LicenseTemplate(xmlLicenseTemplate);
        }

        public UILicenseManager(LicenseTemplate licenseTemplate)
        {
            m_licenseTemplate = licenseTemplate;
        }

        public void SetLicenseFilePath(string path)
        {
            m_licenseFilePath = path;
        }

        public static void RunInternal()
        {
            string n = "TestApp1";

            string xmlLicenseTemplate = new StreamReader(Assembly.GetCallingAssembly().GetManifestResourceStream(n + ".LicenseTemplate.xml")).ReadToEnd();
            (new UILicenseManager(xmlLicenseTemplate)).Run();
        }

        public void Run()
        {
            m_uiSyncContext = SynchronizationContext.Current;

            DialogResult formExitResult = DialogResult.Cancel;

            Thread thread = new Thread(() =>
            {
                LoadAndValidateLicense();

                // display registration dialog when the license 
                // is either invalid, not found, or a trial license
                if (!m_isValidLicense || m_isTrialLicense)
                {
                    m_validPendingLicenseKey = false;
                    m_name = null;
                    m_email = null;
                    KeyValidator validator;

                    if (!String.IsNullOrEmpty(m_pendingLicenseKey))
                        if (ValidateAndDecodeLicenseKey(m_pendingLicenseKey, ref m_name, ref m_email, ref m_pendingLicenseKeyValidationData, out validator))
                            m_validPendingLicenseKey = true;

                    Application.EnableVisualStyles();

                    if (m_uiSyncContext != null)
                    {
                        m_uiSyncContext.Send(callback =>
                        {
                            formExitResult = ShowLicensingForm();
                        }, this);
                    }
                    else
                    {
                        formExitResult = ShowLicensingForm();
                    }

                    if (!m_isValidLicense)
                    {
                        ProcessLicenseValidationEvent(LICENSE_STATUS.Invalid, null, null, false, null, false);

                        m_pendingLicenseKeyValidationData = null;
                        m_name = m_licensingForm.RegistrationName;
                        m_email = m_licensingForm.RegistrationEmail;

                        if (!String.IsNullOrEmpty(m_licensingForm.LicenseKey))
                        {
                            if (ValidateAndDecodeLicenseKey(m_licensingForm.LicenseKey, ref m_name, ref m_email, ref m_pendingLicenseKeyValidationData, out validator))
                            {
                                m_pendingLicenseKey = m_licensingForm.LicenseKey;
                                m_validPendingLicenseKey = true;
                            }
                        }
                    }

                    if (m_isValidLicense || m_validPendingLicenseKey)
                    {
                        SaveLicense();
                    }
                }
                else
                    formExitResult = DialogResult.OK;
            });

            thread.SetApartmentState(ApartmentState.STA);

            thread.Start();
        }

        private bool ValidateAndDecodeLicenseKey(string licenseKey, ref string registrationName, ref string registrationEmail, ref byte[] validationData, out KeyValidator validator)
        {
            validator = new KeyValidator(m_licenseTemplate, licenseKey);

            LicenseKeyFieldType fieldType;
            int fieldSize, fieldOffset;

            if (validationData == null)
            {
                if (m_licenseTemplate.GetValidationDataField("RegistrationName", out fieldType, out fieldSize, out fieldOffset))
                {
                    validator.SetValidationData("RegistrationName", registrationName);
                }

                if (m_licenseTemplate.GetValidationDataField("RegistrationEmail", out fieldType, out fieldSize, out fieldOffset))
                {
                    validator.SetValidationData("RegistrationEmail", registrationEmail);
                }
            }
            else
                validator.SetValidationData(null, validationData);

            bool isValid = validator.IsKeyValid();

            if (isValid)
                if (m_licenseTemplate.ValidationDataSize > 0)
                {
                    if (validationData == null)
                        validationData = validator.QueryValidationData(null);
                    else
                    {
                        if (m_licenseTemplate.GetValidationDataField("RegistrationName", out fieldType, out fieldSize, out fieldOffset))
                        {
                            registrationName = Encoding.UTF8.GetString(validator.QueryValidationData("RegistrationName"));
                        }

                        if (m_licenseTemplate.GetValidationDataField("RegistrationEmail", out fieldType, out fieldSize, out fieldOffset))
                        {
                            registrationEmail = Encoding.UTF8.GetString(validator.QueryValidationData("RegistrationEmail"));
                        }
                    }
                }

            return isValid;
        }

        private void SaveLicense()
        {
            TextWriter s = new Utf8StringWriter();
            XmlWriter x = XmlWriter.Create(s, new XmlWriterSettings { Indent = true });

            x.WriteStartDocument();
                x.WriteStartElement("License");
                    x.WriteAttributeString("version", "1");
                    x.WriteElementString("LicenseKey", m_licenseKey);
            
                    if (m_licenseKeyValidationData != null)
                        x.WriteElementString("LicenseKeyValidationData", Convert.ToBase64String(m_licenseKeyValidationData));

                    x.WriteElementString("ActivationKey", m_activationKey);
                    x.WriteElementString("HardwareId", m_hardwareId);

                    if (!String.IsNullOrEmpty(m_pendingLicenseKey))
                    {
                        x.WriteStartElement("PendingLicense");
                            x.WriteElementString("LicenseKey", m_pendingLicenseKey);
                            if (m_pendingLicenseKeyValidationData != null)
                                x.WriteElementString("LicenseKeyValidationData", Convert.ToBase64String(m_pendingLicenseKeyValidationData));
                            
                            if (!String.IsNullOrEmpty(m_pendingHardwareId))
                                x.WriteElementString("HardwareId", m_pendingHardwareId);
                        
                        x.WriteEndElement();
                    }
                x.WriteEndElement();
            x.WriteEndDocument();

            x.Close();

            string path = m_licenseFilePath;

            if (String.IsNullOrEmpty(m_licenseFilePath))
            {
                FileInfo file = new FileInfo(Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData) + "\\" + m_licenseTemplate.GetProperty("/General/CompanyName", null) + "\\" + m_licenseTemplate.GetProperty("/General/ProductName", null) + "\\license.dat");
                file.Directory.Create();
                path = file.FullName;
            }

            File.WriteAllText(path, Convert.ToBase64String(Encoding.UTF8.GetBytes(s.ToString())));
        }

        private void LoadLicense()
        {
            string path = m_licenseFilePath;

            if (String.IsNullOrEmpty(path))
            {
                path = Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData)
                            + "\\"
                            + m_licenseTemplate.GetProperty("/General/CompanyName", null)
                            + "\\"
                            + m_licenseTemplate.GetProperty("/General/ProductName", null)
                            + "\\license.dat";
            }

            XmlDocument x = new XmlDocument();

            x.LoadXml(
                Encoding.UTF8.GetString(
                    Convert.FromBase64String(
                        File.ReadAllText(path)
                    )
                )
            );

            XmlElement e = (XmlElement)(x.GetElementsByTagName("License")[0]);
            if (e.GetAttribute("version") != "1")
                throw new Exception("invalid license version");

            m_licenseKey = ((XmlElement)(e.GetElementsByTagName("LicenseKey")[0])).InnerText;

            XmlNodeList nodes = e.GetElementsByTagName("LicenseKeyValidationData");
            if (nodes != null && nodes.Count > 0)
            {
                m_licenseKeyValidationData = Convert.FromBase64String(((XmlElement)(nodes[0])).InnerText);
            } else
                m_licenseKeyValidationData = null;

            m_activationKey = ((XmlElement)(e.GetElementsByTagName("ActivationKey")[0])).InnerText;
            
            m_hardwareId = ((XmlElement)(e.GetElementsByTagName("HardwareId")[0])).InnerText;

            nodes = e.GetElementsByTagName("PendingLicense");
            if (nodes != null && nodes.Count > 0)
            {
                e = (XmlElement)(nodes[0]);

                nodes = e.GetElementsByTagName("LicenseKey");
                if (nodes != null && nodes.Count > 0)
                    m_pendingLicenseKey = ((XmlElement)(nodes[0])).InnerText;

                nodes = e.GetElementsByTagName("LicenseKeyValidationData");
                if (nodes != null && nodes.Count > 0)
                    m_pendingLicenseKeyValidationData = Convert.FromBase64String(((XmlElement)(nodes[0])).InnerText);

                nodes = e.GetElementsByTagName("HardwareId");
                if (nodes != null && nodes.Count > 0)
                    m_pendingHardwareId = ((XmlElement)(nodes[0])).InnerText;
            }
        }

        private LICENSE_STATUS ValidateLicense(string licenseKey, ref string registrationName, ref string registrationEmail, ref byte[] licenseKeyValidationData, string hardwareId, string activationKey, out bool isTrial, out DateTime expirationDate)
        {
            KeyValidator validator;

            expirationDate = DateTime.UtcNow.AddDays(-1);
            isTrial = false;

            if (!ValidateAndDecodeLicenseKey(licenseKey, ref registrationName, ref registrationEmail, ref licenseKeyValidationData, out validator))
            {              
                return LICENSE_STATUS.Invalid;
            }

            LicensingClient client = new LicensingClient(m_licenseTemplate, licenseKey, licenseKeyValidationData, hardwareId, activationKey);
            LICENSE_STATUS status = client.IsLicenseValid() ? LICENSE_STATUS.Valid : client.LicenseStatus;

            if (status == LICENSE_STATUS.Valid)
            {
                isTrial = (licenseKey == m_licenseTemplate.GetProperty("/Trial/TrialLicenseKey", null));

                status = ProcessLicenseValidationEvent(status, registrationName, registrationEmail, isTrial, validator, true);
            }

            if (status == LICENSE_STATUS.Valid || status == LICENSE_STATUS.Expired)
            {
                expirationDate = client.LicenseExpirationDate;
            }

            return status;
        }

        private void LoadAndValidateLicense()
        {
            m_isValidLicense = false;

            string val = m_licenseTemplate.GetProperty("/UserInterface", "onlineRegistrationEnabled");

            if (!String.IsNullOrEmpty(val))
            {
                m_onlineRegistrationEnabled = (val.ToLower() == "true") || (val == "1");
            }

            val = m_licenseTemplate.GetProperty("/UserInterface", "offlineRegistrationEnabled");

            if (!String.IsNullOrEmpty(val))
            {
                m_offlineRegistrationEnabled = (val.ToLower() == "true") || (val == "1");
               
                if (m_offlineRegistrationEnabled)
                {
                    m_offlineRegistrationEmail = m_licenseTemplate.GetProperty("/UserInterface/OfflineRegistrationEmail", null);
                    m_offlineRegistrationPhone = m_licenseTemplate.GetProperty("/UserInterface/OfflineRegistrationPhone", null);
                }
            }

            m_allowTrial = !String.IsNullOrEmpty(m_licenseTemplate.GetProperty("/Trial/TrialLicenseKey", null));
            
            try
            {
                LoadLicense();

                if (ValidateLicense(m_licenseKey,
                            ref m_registrationName,
                            ref m_registrationEmail,
                            ref m_licenseKeyValidationData,
                            m_hardwareId,
                            m_activationKey,
                            out m_isTrialLicense,
                            out m_licenseExpirationDate) == LICENSE_STATUS.Valid)
                {
                    m_isValidLicense = true;
                }
            }
            catch (Exception)
            {
                
            }
        }

        private LICENSE_STATUS ProcessLicenseValidationEvent(LICENSE_STATUS licenseStatus, string registrationName, string registrationEmail, bool isTrial, KeyValidator keyValidator, bool wait = true)
        {
            LicensingEventArgs args = new LicensingEventArgs();

            args.LicenseStatus = licenseStatus;

            if (licenseStatus == LICENSE_STATUS.Valid)
            {
                args.KeyValidator = keyValidator;
                args.RegistrationEmail = registrationEmail;
                args.RegistrationName = registrationName;
                args.IsTrial = isTrial;
            }

            if (m_uiSyncContext != null)
            {
                if (wait)
                {
                    m_uiSyncContext.Send(callback =>
                    {
                        LicenseValidation(this, args);
                    }, this);
                }
                else
                {
                    m_uiSyncContext.Post(callback =>
                    {
                        LicenseValidation(this, args);
                    }, this);
                }
            }
            else
                LicenseValidation(this, args);

            return args.LicenseStatus;
        }
        
        private DialogResult ShowLicensingForm()
        {
            m_licensingForm = new LicensingForm(m_licenseTemplate);

            m_licensingForm.OnStartTrial += new StartTrialEventHandler(OnStartTrial);
            m_licensingForm.OnContinueTrial += new ContinueTrialEventHandler(ContinueTrial);
            m_licensingForm.OnRegister += new RegisterEventHandler(OnRegister);
            m_licensingForm.OnBuyNow += new BuyNowEventHandler(OnBuyNow);
            m_licensingForm.OnContactSupport += new ContactSupportEventHandler(OnContactSupport);

            if (m_allowTrial)
            {
                if (m_isTrialLicense)
                    m_licensingForm.SetTrialDaysLeft(1 + m_licenseExpirationDate.Subtract(DateTime.UtcNow).Days);

                m_licensingForm.SetViewMode((m_isTrialLicense) ? (m_isLicenseExpired ? LicensingForm.ViewMode.TrialExpired : LicensingForm.ViewMode.Trial) : LicensingForm.ViewMode.FirstRunWithTrial);
            }
            else
                m_licensingForm.SetViewMode(LicensingForm.ViewMode.FirstRunNoTrial);

            // fill saved values to make it easier for the customer
            if (m_validPendingLicenseKey)
            {
                m_licensingForm.LicenseKey = m_pendingLicenseKey;
                m_licensingForm.RegistrationName = m_name;
                m_licensingForm.RegistrationEmail = m_email;
            }

            return m_licensingForm.ShowDialog();
        }

        void OnStartTrial(object sender, EventArgs args)
        {
            Thread thread = new Thread(() =>
            {
                try
                {
                    m_licensingForm.StartProgress();

                    string trialLicenseKey = m_licenseTemplate.GetProperty("/Trial/TrialLicenseKey", null);

                    if (String.IsNullOrEmpty(trialLicenseKey))
                    {
                        throw new Exception("a trial license key was not set in the license template");
                    }

                    string strTrialLicenseKeyValidationData = m_licenseTemplate.GetProperty("/Trial/TrialKeyValidationData", null);
                    string trialRegistrationName = null, trialRegistrationEmail = null;
                    byte[] trialLicenseKeyValidationData = null;

                    KeyValidator validator = new KeyValidator(m_licenseTemplate, trialLicenseKey);

                    if (String.IsNullOrEmpty(strTrialLicenseKeyValidationData))
                    {
                        LicenseKeyFieldType fieldType;
                        int fieldSize, fieldOffset;

                        if (m_licenseTemplate.GetValidationDataField("RegistrationName", out fieldType, out fieldSize, out fieldOffset))
                        {
                            trialRegistrationName = m_licenseTemplate.GetProperty("/Trial/TrialRegistrationName", null);
                            if (string.IsNullOrEmpty(trialRegistrationName))
                                throw new Exception("a trial registration name was not set in license template");

                            validator.SetValidationData("RegistrationName", trialRegistrationName);
                        }

                        if (m_licenseTemplate.GetValidationDataField("RegistrationEmail", out fieldType, out fieldSize, out fieldOffset))
                        {
                            trialRegistrationEmail = m_licenseTemplate.GetProperty("/Trial/TrialRegistrationEmail", null);
                            if (string.IsNullOrEmpty(trialRegistrationEmail))
                                throw new Exception("a trial registration email was not set in license template");

                            validator.SetValidationData("RegistrationEmail", trialRegistrationEmail);
                        }

                        trialLicenseKeyValidationData = validator.QueryValidationData(null);
                    }
                    else
                    {
                        if (m_licenseTemplate.ValidationDataSize > 0)
                        {
                            trialLicenseKeyValidationData =  Convert.FromBase64String(strTrialLicenseKeyValidationData);
                            validator.SetValidationData(null, trialLicenseKeyValidationData);
                        }
                    }

                    if (!validator.IsKeyValid())
                        throw new Exception("trial license key is not valid");

                    LicensingClient licensingClient = new LicensingClient( m_licenseTemplate.SigningServiceUrl, 
                                                                           m_licenseTemplate, 
                                                                           trialLicenseKey, 
                                                                           trialLicenseKeyValidationData);
                    licensingClient.AcquireLicense();

                    m_licenseKey = trialLicenseKey;
                    m_licenseKeyValidationData = trialLicenseKeyValidationData;
                    m_activationKey = licensingClient.ActivationKey;
                    m_hardwareId = licensingClient.HardwareId;

                    m_isValidLicense = true;
                    m_isTrialLicense = true;

                    m_licensingForm.SetDialogResult(DialogResult.OK);
                    m_licensingForm.SetViewMode(LicensingForm.ViewMode.Success);
                }
                catch (Exception ex)
                {
                    m_licensingForm.MemorizeCurrentViewMode();
                    m_licensingForm.DisplayError(ex.Message);
                }
                finally
                {

                }
            });

            thread.Start();
        }

        void ContinueTrial(object sender, EventArgs args)
        {
            m_licensingForm.SetDialogResult(DialogResult.OK);
            m_licensingForm.Close();
        }

        void OnRegister(object sender, EventArgs args)
        {
            LicensingFormEventArgs eventArgs = (LicensingFormEventArgs)args;

            if (m_licensingForm.GetViewMode() != LicensingForm.ViewMode.Register
                && m_licensingForm.GetViewMode() != LicensingForm.ViewMode.RegisterWithEmail
                && m_licensingForm.GetViewMode() != LicensingForm.ViewMode.RegisterWithName
                && m_licensingForm.GetViewMode() != LicensingForm.ViewMode.RegisterOffline
                && m_licensingForm.GetViewMode() != LicensingForm.ViewMode.RegisterOfflineWithEmail
                && m_licensingForm.GetViewMode() != LicensingForm.ViewMode.RegisterOfflineWithName)
            {
                LicenseKeyFieldType fieldType;
                int fieldSize, fieldOffset;

                if (eventArgs.OfflineRegistration)
                {
                    Thread thread = new Thread(() =>
                    {
                        try
                        {
                            // what we are trying to do here is to preserve a previously saved but still valid hardware id
                            // this is because the customer may have sent the previous hardware id to the software publisher
                            // for generating an activation key, and it may have changed in the mean time while the customer 
                            // waited for a response from the publisher (which can take days or so) 
                            // However, if the previously saved hardware id is not valid, 
                            // we display the current hardware id

                            if (!string.IsNullOrEmpty(m_pendingHardwareId))
                            {
                                if (!KeyHelper.MatchCurrentHardwareId(m_hardwareId))
                                    m_pendingHardwareId = KeyHelper.GetCurrentHardwareId();
                            }
                            else
                                m_pendingHardwareId = KeyHelper.GetCurrentHardwareId();

                            m_licensingForm.HardwareId = m_pendingHardwareId;
                        }
                        catch (Exception ex)
                        {
                            m_licensingForm.MemorizeCurrentViewMode();
                            m_licensingForm.DisplayError(ex.Message);
                        }
                    });

                    thread.Start();
                }

                m_licensingForm.MemorizeCurrentViewMode();

                if (m_licenseTemplate.GetValidationDataField("RegistrationEmail", out fieldType, out fieldSize, out fieldOffset))
                {
                    m_licensingForm.SetViewMode((eventArgs.OfflineRegistration) ? LicensingForm.ViewMode.RegisterOfflineWithEmail : LicensingForm.ViewMode.RegisterWithEmail);
                }
                else
                    if (m_licenseTemplate.GetValidationDataField("RegistrationName", out fieldType, out fieldSize, out fieldOffset))
                    {
                        m_licensingForm.SetViewMode((eventArgs.OfflineRegistration) ? LicensingForm.ViewMode.RegisterOfflineWithName : LicensingForm.ViewMode.RegisterWithName);
                    }
                    else
                    {
                        m_licensingForm.SetViewMode((eventArgs.OfflineRegistration) ? LicensingForm.ViewMode.RegisterOffline : LicensingForm.ViewMode.Register);
                    }
            }
            else
            {
                Thread thread = new Thread(() =>
                {
                    try
                    {
                        m_licensingForm.StartProgress();

                        string name = ((LicensingFormEventArgs)args).RegistrationName,
                               email = ((LicensingFormEventArgs)args).RegistrationEmail;
                        
                        byte[] validationData = null;
                        KeyValidator validator;

                        if (!ValidateAndDecodeLicenseKey(((LicensingFormEventArgs)args).LicenseKey, ref name, ref email,
                                               ref validationData, out validator))
                        {
                            throw new Exception("invalid license key");
                        }

                        LicensingClient licensingClient;

                        if (!((LicensingFormEventArgs)args).OfflineRegistration)
                        {
                            licensingClient = new LicensingClient(m_licenseTemplate.SigningServiceUrl,
                                                                                  m_licenseTemplate,
                                                                                  ((LicensingFormEventArgs)args).LicenseKey,
                                                                                  validationData,
                                                                                  m_licenseTemplate.SigningServiceTemplateId);

                            licensingClient.AcquireLicense();

                            // at this point the license is surely valid

                            if (ProcessLicenseValidationEvent(
                                    LICENSE_STATUS.Valid,
                                    name,
                                    email,
                                    false,
                                    validator,
                                    true
                                ) != LICENSE_STATUS.Valid)
                            {
                                throw new Exception("license is invalid");
                            }

                            m_isValidLicense = true;
                            m_isTrialLicense = false;

                            m_licenseKey = ((LicensingFormEventArgs)args).LicenseKey;
                            m_licenseKeyValidationData = validationData;
                            m_hardwareId = licensingClient.HardwareId;
                            m_activationKey = licensingClient.ActivationKey;
                            m_pendingLicenseKey = null;
                        }
                        else
                        {
                            licensingClient = new LicensingClient(m_licenseTemplate,
                                                                  ((LicensingFormEventArgs)args).LicenseKey,
                                                                  validationData,
                                                                  m_pendingHardwareId,
                                                                  ((LicensingFormEventArgs)args).ActivationKey);

                            if (!licensingClient.IsLicenseValid())
                                throw new Exception("license is invalid");

                            // at this point the license is surely valid

                            if (ProcessLicenseValidationEvent(
                                    LICENSE_STATUS.Valid,
                                    name,
                                    email,
                                    false,
                                    validator, 
                                    true
                                ) != LICENSE_STATUS.Valid)
                            {
                                throw new Exception("license is invalid");
                            }

                            m_isValidLicense = true;
                            m_isTrialLicense = false;

                            m_licenseKey = ((LicensingFormEventArgs)args).LicenseKey;
                            m_activationKey = ((LicensingFormEventArgs)args).ActivationKey;
                            m_licenseKeyValidationData = validationData;
                            m_hardwareId = m_pendingHardwareId;
                            m_pendingLicenseKey = null;
                        }

                        m_licensingForm.SetDialogResult(DialogResult.OK);

                        m_licensingForm.SetViewMode(LicensingForm.ViewMode.Success);
                    }
                    catch (Exception ex)
                    {
                        m_licensingForm.MemorizeCurrentViewMode();
                        m_licensingForm.DisplayError(ex.Message);
                    }
                    finally
                    {

                    }
                });

                thread.Start();
            }
        }

        void OnBuyNow(object sender, EventArgs args)
        {
            string url = m_licenseTemplate.GetProperty("/General/BuyNowUrl", null);

            if (!String.IsNullOrEmpty(url))
            {
                Process.Start(url);
            }
        }

        void OnContactSupport(object sender, EventArgs args)
        {
            string url = m_licenseTemplate.GetProperty("/General/ProductSupportUrl", null);

            if (!String.IsNullOrEmpty(url))
            {
                Process.Start(url);
            }
        }

        private class Utf8StringWriter : StringWriter
        {
            public override Encoding Encoding
            {
                get
                {
                    return Encoding.UTF8;
                }
            }
        }

        private LicensingForm m_licensingForm;
        private LicenseTemplate m_licenseTemplate;

        private bool m_isValidLicense = false;

        private bool m_isTrialLicense = false;
        private bool m_allowTrial = false;
        private bool m_isLicenseExpired = false;
        DateTime m_licenseExpirationDate;
        public event LicensingEventDelegate LicenseValidation;

        string m_licenseKey, 
               m_activationKey, 
               m_hardwareId;
        byte[] m_licenseKeyValidationData;
        
        string m_pendingLicenseKey,
               m_pendingHardwareId;
        byte[] m_pendingLicenseKeyValidationData;
        
        string m_registrationName, 
               m_registrationEmail;
        
        bool m_validPendingLicenseKey = false;
        string m_name;
        string m_email;

        SynchronizationContext m_uiSyncContext;

        bool m_onlineRegistrationEnabled = true;
        bool m_offlineRegistrationEnabled = false;

        string m_offlineRegistrationEmail;
        string m_offlineRegistrationPhone;

        string m_licenseFilePath;
    }
}
