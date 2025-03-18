//
// Copyright (c) 2014 SoftActivate. All rights reserved.
//

using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using System.IO;
using System.Collections;

namespace SoftActivate.Licensing
{
    public enum LicenseKeyFieldType
    {
        Raw,     // Raw buffer of bytes
        Integer, // A 32-bit integer
        String,   // A string (internally, strings are UTF8-encoded)
        Date14, 
        Date16,
        Date13
    }

    public enum LicenseKeyStatus
    {
        Success,
        GenericError,
        OutOfMemory,
        FieldNotFound,
        BufferTooSmall,
        InvalidXml,
        InvalidLicenseKey,
        InvalidKeyEncoding,
        InvalidParameter,
        InvalidSignatureSize,
        UnsupportedVersion
    }

    public enum LicenseKeyEncoding
    {
        None = 0,
        Base32X = 5,
        Base64 = 6
    }

    public class LicenseTemplate
    {
        public LicenseTemplate()
        {
            // default key encoding
            KeyEncoding = LicenseKeyEncoding.Base32X;

            // default group separator
            GroupSeparator = "-";
            Header = "";
            Footer = "";
        }

        public LicenseTemplate(int numberOfGroups, int charsPerGroup, string publicKeyCertificate, string privateKey, int signatureSize, int dataSize, string dataFieldName) : this()
        {
            this._NumGroups = numberOfGroups;
            this._CharsPerGroup = charsPerGroup;
            this.SignatureSize = signatureSize;
            this.DataSize = dataSize;

            if (!string.IsNullOrEmpty(privateKey))
                SetPrivateKey(privateKey);

            if (!string.IsNullOrEmpty(publicKeyCertificate))
                SetPublicKeyCertificate(publicKeyCertificate);
            
            if (!String.IsNullOrEmpty(dataFieldName))
                AddDataField(dataFieldName, LicenseKeyFieldType.Integer, dataSize, 0);
        }

        public LicenseTemplate(string xmlTemplate) : this()
        {
            LoadXml(xmlTemplate);
        }

        ~LicenseTemplate()
        {

        }

        public int Version
        {
            get
            {
                return _Version;
            }
            set
            {
                if (value != 2)
                    throw new Exception("unsupported version");

                _Version = value;
            }
        }

        public int NumberOfGroups
        {
            get
            {
                return _NumGroups;
            }
            set
            {
                if (value <= 0)
                    throw new Exception("invalid parameter");

                _NumGroups = value;
            }
        }

        public int CharactersPerGroup
        {
            get
            {
                return _CharsPerGroup;
            }
            set
            {
                if (value <= 0)
                    throw new Exception("invalid parameter");

                _CharsPerGroup = value;
            }
        }

        public LicenseKeyEncoding KeyEncoding
        {
            get
            {
                return _KeyEncoding;
            }
            set
            {
                _KeyEncoding = value;
            }
        }

        public string GroupSeparator
        {
            get
            {
                return _GroupSeparator;
            }
            set
            {
                _GroupSeparator = value;
            }
        }

        // Sets licese key encoding (BASE32 or BASE64 for now)
        public LicenseKeyEncoding Encoding
        {
            get
            {
                return _KeyEncoding;
            }
            set
            {
                _KeyEncoding = value;
            }
        }

        public string Header
        {
            get
            {
                return _Header;
            }
            set
            {
                _Header = value;
            }
        }

        public string Footer
        {
            get
            {
                return _Footer;
            }
            set
            {
                _Footer = value;
            }
        }

        public int DataSize
        {
            get
            {
                return _DataSize;
            }
            set
            {
                _DataSize = value;
            }
        }

        public bool HasPrivateKey
        {
            get
            {
                return (privateKey != null);
            }
        }

        public string SigningServiceUrl
        {
            get
            {
                return _SigningServiceUrl;
            }
            set
            {
                _SigningServiceUrl = value;
            }
        }

        public int SigningServiceTemplateId
        {
            get
            {
                return _SigningServiceTemplateId;
            }
            set
            {
                _SigningServiceTemplateId = value;
            }
        }

        public void AddDataField(string name, LicenseKeyFieldType type, int size)
        {
            AddDataField(name, type, size, 0);
        }

        public void AddDataField(string name, LicenseKeyFieldType type, int size, int offset)
        {
            keyDataMap.AddField(name, (BitStruct.FieldType)type, size, offset);
        }

        public void DeleteDataField(string name)
        {
            keyDataMap.DeleteField(name);
        }

        public bool EnumDataFields(ref object enumHandle, out string fieldName, out LicenseKeyFieldType fieldType, out int fieldSize, out int fieldOffset)
        {
            BitStruct.FieldType type;
            bool result = keyDataMap.EnumFields(ref enumHandle, out fieldName, out type, out fieldSize, out fieldOffset);
            fieldType = (LicenseKeyFieldType)type;
            return result;
        }

        public bool GetDataField(string fieldName, out LicenseKeyFieldType fieldType, out int fieldSize, out int startPos)
        {
            BitStruct.FieldType type;
            bool result = keyDataMap.GetFieldInfo(fieldName, out type, out fieldSize, out startPos);
            fieldType = (LicenseKeyFieldType)type;
            return result;
        }

        // Sets the total size of the validation data in bits
        // Validation data is similar to license key data above, 
        // But it is not included in the license key and is used only for key validation
        public int ValidationDataSize
        {
            get
            {
                return _ValidationDataSize;
            }
            set
            {
                _ValidationDataSize = value;
            }
        }

        public void AddValidationField(string name, LicenseKeyFieldType type, int size)
        {
            AddValidationField(name, type, size, -1);
        }

        // Add a validation field (a named sequence of bits which will be required at validation time)
        public void AddValidationField(string name, LicenseKeyFieldType type, int size, int offset)
        {
            validationDataMap.AddField(name, (BitStruct.FieldType)type, size, offset);

            if (offset + size > _ValidationDataSize)
            {
                _ValidationDataSize = offset + size;
            }
        }

        public void DeleteValidationField(string fieldName)
        {
            validationDataMap.DeleteField(fieldName);

            _ValidationDataSize = validationDataMap.MappedSize;
        }

        public bool EnumValidationFields(ref object enumHandle, out string fieldName, out LicenseKeyFieldType fieldType, out int fieldSize, out int fieldOffset)
        {
            BitStruct.FieldType type;
            bool result = validationDataMap.EnumFields(ref enumHandle, out fieldName, out type, out fieldSize, out fieldOffset);
            fieldType = (LicenseKeyFieldType)type;
            return result;
        }

        public bool GetValidationDataField(string fieldName, out LicenseKeyFieldType fieldType, out int fieldSize, out int fieldOffset)
        {
            BitStruct.FieldType type;
            bool result = validationDataMap.GetFieldInfo(fieldName, out type, out fieldSize, out fieldOffset);
            fieldType = (LicenseKeyFieldType)type;
            return result;
        }

        public bool RandomizeUnusedSpace
        {
            get
            {
                return _RandomizeUnusedSpace;
            }
            set
            {
                _RandomizeUnusedSpace = value;
            }
        }

        public int SignatureSize
        {
            get
            {
                return _SignatureSize;
            }
            set
            {
                if (value < 76 || value > 322)
                    throw new Exception("invalid signature size");

                _SignatureSize = value;

                if (_SignatureSize <= 108)
                {
                    signatureKeyType = Ecc.KeyType.Ecc54;
                    signatureKeySize = 54;
                }
                else
                    if (_SignatureSize <= 146)
                    {
                        signatureKeyType = Ecc.KeyType.Ecc73;
                        signatureKeySize = 73;
                    }
                    else
                        if (_SignatureSize <= 158)
                        {
                            signatureKeyType = Ecc.KeyType.Ecc79;
                            signatureKeySize = 79;
                        }
                        else
                            if (_SignatureSize <= 182)
                            {
                                signatureKeyType = Ecc.KeyType.Ecc91;
                                signatureKeySize = 91;
                            }
                            else
                                if (_SignatureSize <= 200)
                                {
                                    signatureKeyType = Ecc.KeyType.Ecc100;
                                    signatureKeySize = 100;
                                }
                                else
                                    if (_SignatureSize <= 240)
                                    {
                                        signatureKeyType = Ecc.KeyType.Ecc120;
                                        signatureKeySize = 120;
                                    }
                                    else
                                        if (_SignatureSize <= 262)
                                        {
                                            signatureKeyType = Ecc.KeyType.Ecc131;
                                            signatureKeySize = 131;
                                        }
                                        else
                                            if (_SignatureSize <= 282)
                                            {
                                                signatureKeyType = Ecc.KeyType.Ecc141;
                                                signatureKeySize = 141;
                                            }
                                            else
                                            {
                                                signatureKeyType = Ecc.KeyType.Ecc161;
                                                signatureKeySize = 161;
                                            }
            }
        }

        public void LoadXml(string xmlTemplate)
        {
            XmlDocument x = new XmlDocument();
            x.LoadXml(xmlTemplate);

            string rootElementName = "LicenseKeyTemplate";

            XmlElement e = (XmlElement)(x.GetElementsByTagName("LicenseKeyTemplate")[0]);
            if (e == null)
            {
                e = (XmlElement)(x.GetElementsByTagName("LicenseTemplate")[0]);
                rootElementName = "LicenseTemplate";
            }

            string templateVersionStr = e.GetAttribute("version");
            int templateVersion = String.IsNullOrEmpty(templateVersionStr) ? 1 : Int32.Parse(templateVersionStr);

            e = (XmlElement)(x.GetElementsByTagName("LicenseKey")[0]);

            string encoding = e.GetAttribute("encoding");

            if (!String.IsNullOrEmpty(encoding))
            {
                switch (encoding.ToLower())
                {
                    case "base32x":
                        _KeyEncoding = LicenseKeyEncoding.Base32X;
                        break;

                    case "base64":
                        _KeyEncoding = LicenseKeyEncoding.Base64;
                        break;

                    default:
                        _KeyEncoding = LicenseKeyEncoding.Base32X;
                        break;
                }
            }
            else
                _KeyEncoding = LicenseKeyEncoding.Base32X;

           _NumGroups = int.Parse(e.GetAttribute("characterGroups"));
           _CharsPerGroup = int.Parse(e.GetAttribute("charactersPerGroup"));
           _GroupSeparator = (e.HasAttribute("groupSeparator")) ? e.GetAttribute("groupSeparator") : "-";
           _Header = (e.HasAttribute("header")) ? e.GetAttribute("header") : "";
           _Footer = (e.HasAttribute("footer")) ? e.GetAttribute("footer") : "";

           XmlNodeList l = x.SelectNodes("/" + rootElementName + "/LicenseKey/Data/DataFields/Field");

           LicenseKeyFieldType type;
           string typeStr;

           foreach (XmlElement n in l)
           {
               typeStr = n.GetAttribute("type");
               if (!String.IsNullOrEmpty(typeStr))
               {
                   switch (typeStr.ToLower())
                   {
                       case "integer":
                           type = LicenseKeyFieldType.Integer;
                           break;

                       case "string":
                           type = LicenseKeyFieldType.String;
                           break;
                       
                       case "raw":
                           type = LicenseKeyFieldType.Raw;
                           break;

                       case "date13":
                           type = LicenseKeyFieldType.Date13;
                           break;

                       case "date14":
                           type = LicenseKeyFieldType.Date14;
                           break;

                       case "date16":
                           type = LicenseKeyFieldType.Date16;
                           break;

                       default:
                           type = LicenseKeyFieldType.Raw;
                           break;
                   }
               }
               else
                   type = LicenseKeyFieldType.Raw;

               keyDataMap.AddField(n.GetAttribute("name"), (BitStruct.FieldType)type, int.Parse(n.GetAttribute("size")), int.Parse(n.GetAttribute("offset")));
           }

           l = x.SelectNodes("/" + rootElementName + "/LicenseKey/Data/ValidationFields/Field");

           _ValidationDataSize = 0;

           foreach (XmlElement n in l)
           {
               typeStr = n.GetAttribute("type");
               if (!String.IsNullOrEmpty(typeStr))
               {
                   switch (typeStr.ToLower())
                   {
                       case "integer":
                           type = LicenseKeyFieldType.Integer;
                           break;

                       case "string":
                           type = LicenseKeyFieldType.String;
                           break;

                       case "raw":
                           type = LicenseKeyFieldType.Raw;
                           break;

                       case "date13":
                           type = LicenseKeyFieldType.Date13;
                           break;
                       
                       case "date14":
                           type = LicenseKeyFieldType.Date14;
                           break;

                       case "date16":
                           type = LicenseKeyFieldType.Date16;
                           break;

                       default:
                           type = LicenseKeyFieldType.Raw;
                           break;
                   }
               }
               else
                   type = LicenseKeyFieldType.Raw;

               int offset = int.Parse(n.GetAttribute("offset"));
               int size = int.Parse(n.GetAttribute("size"));

               validationDataMap.AddField(n.GetAttribute("name"), (BitStruct.FieldType)type, size, offset);

               if (_ValidationDataSize < offset + size)
                   _ValidationDataSize = offset + size;
           }

           e = (XmlElement)x.SelectSingleNode("/" + rootElementName + "/LicenseKey/Signature");
           SignatureSize = int.Parse(e.GetAttribute("size"));
           _DataSize = (_NumGroups * _CharsPerGroup) * (int)_KeyEncoding - _SignatureSize;

           if ((e = (XmlElement)x.SelectSingleNode("/" + rootElementName + "/LicenseKey/Signature/SignaturePrivateKey")) != null)
           {
               if (templateVersion == 1)
               {
                   Key key = new Key(e.InnerText, signatureKeyType);
                   SetPrivateKey(Convert.ToBase64String(key.Export()));
               }
               else
                   if (templateVersion <= 3)
                   {
                       SetPrivateKey(e.InnerText);
                   }
                   else
                       throw new Exception("unsupported template version");
           }

           if ((e = (XmlElement)x.SelectSingleNode("/" + rootElementName + "/LicenseKey/Signature/SignaturePublicKey")) != null)
           {
               if (templateVersion == 1)
               {
                   // convert key to new format
                   Key key = new Key(e.InnerText, signatureKeyType);
                   SetPublicKeyCertificate(Certificate.Generate(SignatureSize, DateTime.UtcNow.AddYears(10), null, Convert.ToBase64String(key.Export()), /*GetPrivateKey()*/null, null, certificateAuthorityUrl).ToString());
               }
               else
                   if (templateVersion <= 3)
                   {
                       SetPublicKeyCertificate(e.InnerText);
                   }
                   else
                       throw new Exception("unsupported template version");
           }
           else
           {
               throw new Exception("public key missing from template");
           }

           if ((e = (XmlElement)x.SelectSingleNode("/" + rootElementName + "/LicenseKey/Signature/SigningServiceUrl")) != null)
           {
               SigningServiceUrl = e.InnerText;
           }

           if ((e = (XmlElement)x.SelectSingleNode("/" + rootElementName + "/LicenseKey/Signature/SigningServiceTemplateId")) != null)
           {
               if (!Int32.TryParse(e.InnerText, out _SigningServiceTemplateId))
                   _SigningServiceTemplateId = 0;
           }

           if (templateVersion < 3)
               return;
          
           // V3 template fields
           if ((e = (XmlElement)x.SelectSingleNode("/LicenseTemplate/Properties")) != null)
           {
               properties.LoadXml(e);
           }
        }

        public string SaveXml(bool savePrivateKey)
        {
            TextWriter s = new Utf8StringWriter();
            // XmlWriter x = XmlWriter.Create(s, new XmlWriterSettings{ Indent = true });
            XmlTextWriter x = new XmlTextWriter(s);
            x.Formatting = Formatting.Indented;
            x.QuoteChar = '\'';

            x.WriteStartDocument();
                x.WriteStartElement("LicenseTemplate");
                x.WriteAttributeString("version", Version.ToString());
                    
                    x.WriteStartElement("LicenseKey");
                    x.WriteAttributeString("encoding", (_KeyEncoding == LicenseKeyEncoding.Base64) ? "BASE64" : "BASE32X");
                    x.WriteAttributeString("characterGroups", _NumGroups.ToString());
                    x.WriteAttributeString("charactersPerGroup", _CharsPerGroup.ToString());
                    x.WriteAttributeString("groupSeparator", _GroupSeparator);
                    x.WriteAttributeString("header", _Header);
                    x.WriteAttributeString("footer", _Footer);
                        x.WriteStartElement("Data");
                            x.WriteStartElement("DataFields");
                
                            string name; int size, offset; BitStruct.FieldType type;
                            object enumHandle = null;
                            
                            while (keyDataMap.EnumFields(ref enumHandle, out name, out type, out size, out offset))
                            {
                                x.WriteStartElement("Field");
                                x.WriteAttributeString("name", name);
                                x.WriteAttributeString("type", ((LicenseKeyFieldType)type).ToString());
                                x.WriteAttributeString("size", size.ToString());
                                x.WriteAttributeString("offset", offset.ToString());
                                x.WriteEndElement();
                            }
                            
                            x.WriteEndElement();
                            x.WriteStartElement("ValidationFields");

                            enumHandle = null;

                            while (validationDataMap.EnumFields(ref enumHandle, out name, out type, out size, out offset))
                            {
                                x.WriteStartElement("Field");
                                x.WriteAttributeString("name", name);
                                x.WriteAttributeString("type", ((LicenseKeyFieldType)type).ToString());
                                x.WriteAttributeString("size", size.ToString());
                                x.WriteAttributeString("offset", offset.ToString());
                                x.WriteEndElement();
                            }

                            x.WriteEndElement();
                        x.WriteEndElement();
                        x.WriteStartElement("Signature");
                        x.WriteAttributeString("size", _SignatureSize.ToString());
                            if (certificate != null)
                            {
                                x.WriteElementString("SignaturePublicKey", GetPublicKeyCertificate());
                                if (savePrivateKey && HasPrivateKey)
                                {
                                    x.WriteElementString("SignaturePrivateKey", GetPrivateKey());
                                }
                            }
                            if (!String.IsNullOrEmpty(_SigningServiceUrl))
                            {
                                x.WriteElementString("SigningServiceUrl", _SigningServiceUrl);
                            }
                            if (_SigningServiceTemplateId != 0)
                            {
                                x.WriteElementString("SigningServiceTemplateId", _SigningServiceTemplateId.ToString());
                            }
                        x.WriteEndElement();
                    x.WriteEndElement();

                    x.WriteStartElement("Properties");
                        properties.SaveXml(x);
                    x.WriteEndElement();

            x.WriteEndElement();
            x.WriteEndDocument();

            x.Close();

            return s.ToString();
        }

        public void SetSigningServiceUrl(string url)
        {
            _SigningServiceUrl = url;
        }

        public void SetSigningServiceTemplateId(int templateId)
        {
            _SigningServiceTemplateId = templateId;
        }

        public void SetSigningServiceParameters(string parameters)
        {
            signingServiceParameters = parameters;
        }

        internal Key SigningKey
        {
            get
            {
                return privateKey;
            }
        }

        internal string SigningServiceParameters
        {
            get
            {
                return signingServiceParameters;
            }
        }

        internal Key VerificationKey
        {
            get
            {
                return publicKey;
            }
        }

        internal int SignatureKeySize
        {
            get
            {
                return signatureKeySize;
            }
        }

        public void SetPublicKeyCertificate(string base64Certificate)
        {
            certificate = new Certificate(base64Certificate);

            if (!Certificate.VerifySignature(certificate, new Certificate("EClDDQELjMGgCJew5QSVMBWSAMvHap2vgu31kc43/qsrco6CqAAL")))
                throw new Exception("certificate signature is either invalid or certificate is signed by un untrusted party");
            
            publicKey = new Key(certificate.PublicKey);
        }

        public void SetPrivateKey(string base64PrivateKey)
        {
            byte[] rawPrivateKey = Convert.FromBase64String(base64PrivateKey);
            privateKey = new Key(rawPrivateKey);

            if (!SDKRegistration.IsRegistered)
            {
                Ecc.KeyType keyType = privateKey.KeyType;

                if (rawPrivateKey.Length != 2 + m_demoPrivateKeys[(int)keyType, 0])
                    throw new Exception("SDK is in DEMO mode. This certificate is not allowed");

                for (int i = 2; i < rawPrivateKey.Length; i++)
                    if (m_demoPrivateKeys[(int)keyType, i - 1] != rawPrivateKey[i])
                        throw new Exception("SDK is in DEMO mode. This certificate is not allowed");
            }
        }

        public void GenerateSigningKeyPair()
        {
            privateKey = null;
            publicKey = null;

            if (!SDKRegistration.IsRegistered)
            {
                privateKey = new Key();
                publicKey = new Key();

                int L = m_demoPrivateKeys[(int)signatureKeyType, 0];
                byte[] demoPrivateKey = new byte[L];
                System.Buffer.BlockCopy(m_demoPrivateKeys, (int)signatureKeyType * m_demoPrivateKeys.GetLength(1) + 1, demoPrivateKey, 0, L);

                L = m_demoPublicKeys[(int)signatureKeyType, 0];
                byte[] demoPublicKey = new byte[L];
                System.Buffer.BlockCopy(m_demoPublicKeys, (int)signatureKeyType * m_demoPublicKeys.GetLength(1) + 1, demoPublicKey, 0, L);

                privateKey.Load(demoPrivateKey, signatureKeyType);
                publicKey.Load(demoPublicKey, signatureKeyType);
            }
            else
            {
                Ecc.GenerateKeyPair(signatureKeyType, out privateKey, out publicKey);
            }

            Certificate certificate = Certificate.Generate(_SignatureSize, null, privateKey, publicKey, null, null, certificateAuthorityUrl);

            SetPrivateKey(Convert.ToBase64String(privateKey.Export()));
            SetPublicKeyCertificate(certificate.ToString());
        }

        public string GetPublicKeyCertificate()
        {
            return certificate.ToString();
        }

        public string GetPrivateKey()
        {
            if (privateKey == null)
                throw new Exception("the key template does not contain a private key");

            return Convert.ToBase64String(privateKey.Export());
        }

        public void SetProperty(string path, string name, string value)
        {
            properties.SetProperty(path, name, value);
        }

        public string GetProperty(string path, string name)
        {
            return properties.GetProperty(path, name);
        }

        private PropertyCollection properties = new PropertyCollection();

        private int _Version = 3;
        private int _SigningServiceTemplateId = 0;

        // key format
        private int _NumGroups;
        private int _CharsPerGroup;
        private LicenseKeyEncoding _KeyEncoding;
        private string _GroupSeparator;
        private string _Header;
        private string _Footer;

        // key data
        internal BitStruct keyDataMap = new BitStruct();
        internal BitStruct validationDataMap = new BitStruct();

        // key signature
        private Key publicKey;
        private Key privateKey;
        private Certificate certificate;
        private Ecc.KeyType signatureKeyType;
        private int signatureKeySize;
        private int _SignatureSize;
        private int _DataSize;
        private int _ValidationDataSize;
        private string _SigningServiceUrl;
        private string signingServiceParameters;
        private bool _RandomizeUnusedSpace = false;

	    private static byte[,] m_demoPublicKeys = new byte[10, 23]
            {
	            {8,  0x67, 0xC9, 0x0C, 0xB9, 0xF9, 0x7D, 0x1C, 0x9C, 0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,      0},
	            {11, 0x76, 0x8C, 0x73, 0xC8, 0xC3, 0x6E, 0xEE, 0x7E, 0x3A, 0xC2, 0x13, 0,    0,    0,    0,    0,    0,    0,    0,    0,    0,      0},
	            {12, 0xC2, 0x8F, 0x03, 0x24, 0x4B, 0xAF, 0xC7, 0x94, 0x20, 0x3D, 0x46, 0x03, 0,    0,    0,    0,    0,    0,    0,    0,    0,      0},
	            {13, 0xC8, 0xC3, 0x75, 0x6A, 0x9E, 0x74, 0x5F, 0xC9, 0x18, 0xCF, 0xA0, 0xD4, 0xA9, 0,    0,    0,    0,    0,    0,    0,    0,      0},
	            {14, 0x75, 0xD5, 0x7F, 0xDC, 0x08, 0x48, 0x93, 0x55, 0xDF, 0x63, 0xF9, 0xC4, 0x2C, 0x48, 0 ,   0,    0,    0,    0,    0,    0,      0},
	            {14, 0xE1, 0x5A, 0xA2, 0xE0, 0xB7, 0x33, 0xFF, 0x88, 0x20, 0xFF, 0x1F, 0x91, 0xC9, 0x77, 0,    0,    0,    0,    0,    0,    0,      0},
	            {17, 0xCC, 0x50, 0x01, 0x33, 0x11, 0x20, 0xFF, 0xA8, 0xED, 0x4F, 0xF2, 0x0A, 0xD7, 0xCB, 0xD0, 0xC3, 0x14, 0,    0,    0,    0,      0},
	            {18, 0x5A, 0xCC, 0xFB, 0xF5, 0x00, 0x62, 0xB5, 0x6B, 0xEE, 0x28, 0xE3, 0x92, 0x56, 0x2C, 0xAD, 0x76, 0xB8, 0xAE, 0,    0,    0,      0},
	            {20, 0x4B, 0x36, 0x14, 0x2C, 0x29, 0xF0, 0x0C, 0xCD, 0x2F, 0xC2, 0xE0, 0x24, 0x4A, 0x6E, 0xCA, 0x83, 0x7B, 0xA7, 0xB1, 0x05, 0,      0},
	            {22, 0x6B, 0x3C, 0x1D, 0xD7, 0x6F, 0xAC, 0x59, 0x37, 0xAA, 0xCC, 0x52, 0xF9, 0xBB, 0x7B, 0x25, 0x34, 0x93, 0x40, 0x7D, 0xE0, 0x90, 0x07}
            };

        private static byte[,] m_demoPrivateKeys = new byte[10, 23]
            {
	            {7,  0x5B, 0x3C, 0x97, 0xDC, 0x58, 0x4C, 0x37, 0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,   0,     0,   0},
	            {9,  0xC3, 0xD3, 0xB3, 0xE7, 0x13, 0x0E, 0xC2, 0x46, 0x2F, 0,    0,    0,    0,    0,    0,    0,    0,    0,    0,   0,     0,   0},
	            {10, 0x25, 0x72, 0x41, 0x0A, 0x22, 0x8E, 0x88, 0x25, 0x1C, 0x01, 0,    0,    0,    0,    0,    0,    0,    0,    0,   0,     0,   0},
	            {12, 0x23, 0x5F, 0xA9, 0x40, 0x61, 0x5A, 0xB6, 0xC9, 0x07, 0x86, 0x1E, 0x01, 0,    0,    0,    0,    0,    0,    0,   0,     0,   0},
	            {12, 0x24, 0x2F, 0x29, 0xA7, 0xE1, 0xAF, 0xC1, 0x42, 0x41, 0x82, 0xAF, 0x67, 0,    0,    0,    0,    0,    0,    0,   0,     0,   0},
	            {13, 0xA8, 0x89, 0x6A, 0x64, 0x85, 0x9D, 0x00, 0xBC, 0x06, 0x92, 0xF0, 0xF9, 0x08, 0,    0,    0,    0,    0,    0,   0,     0,   0},
	            {15, 0x00, 0x8F, 0x7E, 0x9F, 0x61, 0x38, 0x8B, 0xB3, 0x9D, 0x2D, 0x68, 0x17, 0x7E, 0x7D, 0x19, 0,    0,    0,    0,   0,     0,   0},
	            {17, 0xBE, 0x53, 0x27, 0xF2, 0xB4, 0xD6, 0xA0, 0xA8, 0xF9, 0x10, 0x67, 0x94, 0x61, 0xD9, 0x8B, 0x4B, 0x03, 0,    0,   0,     0,   0},
	            {18, 0x0E, 0x15, 0xCB, 0xFC, 0x8D, 0x6C, 0x93, 0xA3, 0x1F, 0x70, 0xB8, 0xFB, 0x25, 0xB4, 0xB8, 0xE1, 0xED, 0x04, 0,   0,     0,   0},
	            {20, 0x66, 0x8C, 0xE8, 0x03, 0x44, 0x4E, 0x5C, 0x81, 0x15, 0xBF, 0x01, 0x07, 0x6A, 0xA3, 0x59, 0xF6, 0xE0, 0x97, 0x76, 0x45, 0,   0}
            };

        private const string certificateAuthorityUrl = "http://www.softactivate.com/licensingservice/api/certificates/issuer";

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
    }
}
