using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using System.Web;
using System.Net;
using System.IO;

namespace SoftActivate.Licensing
{
    public enum LICENSE_STATUS
    {
        Valid = 0,
        Invalid = 1,
        InvalidHardwareId = 12,
        Expired = 13,
        InvalidActivationKey = 14
    }

    public enum TIME_VALIDATION_METHOD
    {
        PreferInternetTime = 0,
        UseInternetTime,
        UseLocalTime
    }

    public delegate void LicensingCompleteEventHandler();
    public delegate void LicensingExceptionEventHandler(Exception ex);

    public class LicensingClient
    {
        public event LicensingCompleteEventHandler OnLicensingComplete;
        public event LicensingExceptionEventHandler OnLicensingException;

        public LicensingClient(string licensingServiceUrl, LicenseTemplate licenseKeyTemplate, string licenseKey, byte[] licenseKeyValidationData, string hardwareId, int productId)
        {
            this.m_licenseTemplate = licenseKeyTemplate;
            this.m_licenseKey = licenseKey;
            this.m_licenseKeyValidationData = licenseKeyValidationData;
            this.m_productId = productId;
            this.m_hardwareId = hardwareId;
            this.m_licensingServiceUrl = licensingServiceUrl;
        }

        public LicensingClient(string licensingServiceUrl, LicenseTemplate licenseKeyTemplate, string licenseKey, byte[] licenseKeyValidationData, int productId):
            this(licensingServiceUrl, licenseKeyTemplate, licenseKey, licenseKeyValidationData, null, productId)
        {

        }

        public LicensingClient(string licensingServiceUrl, LicenseTemplate licenseKeyTemplate, string licenseKey, byte[] licenseKeyValidationData) :
            this(licensingServiceUrl, licenseKeyTemplate, licenseKey, licenseKeyValidationData, null, -1)
        {

        }

        public LicensingClient(LicenseTemplate licenseKeyTemplate, string licenseKey, byte[] licenseKeyValidationData, string hardwareId, string activationKey) :
            this(null, licenseKeyTemplate, licenseKey, licenseKeyValidationData, null, -1)
        {
            this.m_hardwareId = hardwareId;
            this.m_activationKey = activationKey;
        }

        private LicenseTemplate CreateActivationTemplate()
        {
            LicenseTemplate template = new LicenseTemplate();

            template.CharactersPerGroup = m_licenseTemplate.CharactersPerGroup;
            template.NumberOfGroups = m_licenseTemplate.NumberOfGroups;
            template.Encoding = m_licenseTemplate.Encoding;
            template.DataSize = m_licenseTemplate.DataSize;
            template.SignatureSize = m_licenseTemplate.SignatureSize;
            template.SetPublicKeyCertificate(m_licenseTemplate.GetPublicKeyCertificate());

            int keyLen = template.NumberOfGroups * template.CharactersPerGroup +
                    (template.NumberOfGroups - 1) * template.GroupSeparator.Length;

            int hwidLen = 29;

            if (!String.IsNullOrEmpty(m_hardwareId))
                hwidLen = m_hardwareId.Length;

            template.DataSize = template.NumberOfGroups * template.CharactersPerGroup * (int)template.Encoding - template.SignatureSize;
            template.AddDataField("ExpirationDate", LicenseKeyFieldType.Integer, 16);

            template.ValidationDataSize = (keyLen + hwidLen) * 8 + ((m_licenseKeyValidationData == null) ? 0 : (m_licenseKeyValidationData.Length * 8));

            template.AddValidationField("LicenseKey", LicenseKeyFieldType.String, keyLen * 8, 0);
            template.AddValidationField("HardwareId", LicenseKeyFieldType.String, hwidLen * 8, keyLen * 8);

            if (m_licenseKeyValidationData != null)
                template.AddValidationField("LicenseKeyValidationData", LicenseKeyFieldType.Raw, m_licenseKeyValidationData.Length * 8, (hwidLen + keyLen) * 8);

            if (!String.IsNullOrEmpty(m_licensingServiceUrl))
                template.SetSigningServiceUrl(m_licensingServiceUrl);

            if (m_productId != -1)
            {
                //activationKeyTemplate.SetSigningServiceParameters("ProductId=" + productId.ToString());
                template.SigningServiceTemplateId = m_productId;
            }

            return template;
        }

        private bool ValidateHardwareId()
        {
            if (string.IsNullOrEmpty(m_hardwareId))
            {
                m_activationStatus = LICENSE_STATUS.InvalidHardwareId;
                return false;
            }

            if (!MatchCurrentHardwareId(m_hardwareId))
            {
                m_activationStatus = LICENSE_STATUS.InvalidHardwareId;
                return false;
            }

            return true;
        }

        private bool ValidateActivationKey(LicenseTemplate activationTemplate = null, bool validateExpirationDate = true)
        {
            if (activationTemplate == null)
                activationTemplate = CreateActivationTemplate();

            KeyValidator validator = new KeyValidator(activationTemplate);

            try
            {
                validator.SetKey(m_activationKey);
            }
            catch (Exception)
            {
                // activation key is invalid - need activation
                m_activationStatus = LICENSE_STATUS.InvalidActivationKey;
                return false;
            }

            if (m_licenseKey != null)
                validator.SetValidationData("LicenseKey", m_licenseKey);

            if (m_hardwareId != null)
                validator.SetValidationData("HardwareId", m_hardwareId);

            if (m_licenseKeyValidationData != null)
                validator.SetValidationData("LicenseKeyValidationData", m_licenseKeyValidationData);

            if (!validator.IsKeyValid())
            {
                m_activationStatus = LICENSE_STATUS.InvalidActivationKey;
                return false;
            }
           
            byte[] rawKeyData = validator.QueryKeyData("ExpirationDate");
            ushort keyData = (ushort)(((ushort)rawKeyData[0]) << 8 | rawKeyData[1]);

            if (keyData != 0)
            {
                m_licenseExpirationDate = new DateTime(2000 + (keyData >> 9), (keyData & 0x01FF) >> 5, keyData & 0x001F);

                if (!validateExpirationDate)
                    return true;

                DateTime now;

                if (m_timeValidationMethod != TIME_VALIDATION_METHOD.UseLocalTime)
                {
                    try
                    {
                        now = NTPClient.GetCurrentTimeUTC(20000);
                        //System.Diagnostics.Trace.WriteLine("[LicensingSDK] The reported Internet time is " + now.ToString("D"));
                    }
                    catch (Exception)
                    {
                        if (m_timeValidationMethod == TIME_VALIDATION_METHOD.UseInternetTime)
                        {
                            throw;
                            // activationStatus = LICENSE_STATUS.Expired;
                            // return false;
                        }

                        now = DateTime.UtcNow;
                    }
                }
                else
                    now = DateTime.UtcNow;

                if (DateTime.Compare(m_licenseExpirationDate, now) < 0)
                {
                    m_activationStatus = LICENSE_STATUS.Expired;
                    return false;
                }
            }
            else
                m_licenseExpirationDate = DateTime.MinValue;

            return true;
        }

        public void BeginAcquireLicense()
        {
            Thread licensingThread = new Thread(new ParameterizedThreadStart(LicensingThread));
            licensingThread.IsBackground = true;
            licensingThread.Start(null);
        }

        public void AcquireLicense()
        {
            if (String.IsNullOrEmpty(m_hardwareId))
                m_hardwareId = GetCurrentHardwareId();

            LicenseTemplate activationTemplate = CreateActivationTemplate();

            KeyGenerator generator = new KeyGenerator(activationTemplate);

            if (!string.IsNullOrEmpty(m_licenseKey))
            {
                generator.SetValidationData("LicenseKey", m_licenseKey);
            }

            generator.SetValidationData("HardwareId", m_hardwareId);

            if (m_licenseKeyValidationData != null)
            {
                generator.SetValidationData("LicenseKeyValidationData", m_licenseKeyValidationData);
            }

            m_activationKey = generator.GenerateKey();

            if (!ValidateActivationKey(activationTemplate, false))
                throw new Exception("internal error - returned activation key is invalid");
        }

        private void LicensingThread(object param)
        {
            try
            {
                AcquireLicense();
            }
            catch (WebException ex)
            {
                if (ex.Response != null)
                {
                    HttpWebResponse httpResponse = ex.Response as HttpWebResponse;

                    if (httpResponse != null)
                    {
                        OnLicensingException(new Exception(ex.Message + " " + httpResponse.StatusDescription));
                        return;
                    }
                }

                OnLicensingException(ex);
                return;
            }
            catch (Exception ex)
            {
                OnLicensingException(ex);
                return;
            }

            OnLicensingComplete();
        }

        public bool IsLicenseValid()
        {
            return ValidateHardwareId() && ValidateActivationKey();
        }

        public virtual string GetCurrentHardwareId()
        {
            return KeyHelper.GetCurrentHardwareId();
        }

        public virtual bool MatchCurrentHardwareId(string hardwareId)
        {
            return KeyHelper.MatchCurrentHardwareId(hardwareId);
        }

        public string ActivationKey
        {
            get
            {
                return m_activationKey;
            }
        }

        public string HardwareId
        {
            get
            {
                return m_hardwareId;
            }
        }

        public LICENSE_STATUS LicenseStatus
        {
            get
            {
                return m_activationStatus;
            }
        }

        public DateTime LicenseExpirationDate
        {
            get
            {
                return m_licenseExpirationDate;
            }
        }

        public TIME_VALIDATION_METHOD TimeValidationMethod
        {
            get
            {
                return m_timeValidationMethod;
            }
            set
            {
                m_timeValidationMethod = value;
            }
        }

        string m_licensingServiceUrl;
        LicenseTemplate m_licenseTemplate;
        int m_productId = -1;
        string m_licenseKey;
        string m_hardwareId;
        string m_activationKey;
        byte[] m_licenseKeyValidationData;
        LICENSE_STATUS m_activationStatus;
        DateTime m_licenseExpirationDate;
        TIME_VALIDATION_METHOD m_timeValidationMethod = TIME_VALIDATION_METHOD.PreferInternetTime;
    }
}
