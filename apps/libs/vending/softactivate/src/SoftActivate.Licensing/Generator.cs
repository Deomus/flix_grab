//
// Copyright (c) 2014 SoftActivate. All rights reserved.
//

using System;
using System.Text;
using System.Diagnostics;
using System.Net;
using System.IO;
using System.Collections;
using System.Net.Http;

namespace SoftActivate.Licensing
{
    public class KeyGenerator
    {
        public KeyGenerator()
        {

        }

        public KeyGenerator(LicenseTemplate templ)
        {
            SetLicenseKeyTemplate(templ);
        }

        ~KeyGenerator()
        {

        }

        public void SetLicenseKeyTemplate(LicenseTemplate templ)
        {
            if (templ == null)
                throw new Exception();

            keyTemplate = templ;
            rawKey = new BitStream(keyTemplate.NumberOfGroups * keyTemplate.CharactersPerGroup * (int)keyTemplate.KeyEncoding, keyTemplate.RandomizeUnusedSpace);
            rawKey.Clear();
            //rawKey.Randomize();
            validationData = new BitStream(keyTemplate.ValidationDataSize);
            validationData.Clear();
        }

        public void SetKeyData(string fieldName, byte[] buf)
        {
            keyTemplate.keyDataMap.Attach(rawKey);
            keyTemplate.keyDataMap.Set(fieldName, buf);
            keyTemplate.keyDataMap.Detach();
        }

        public void SetKeyData(string fieldName, string data)
        {
            keyTemplate.keyDataMap.Attach(rawKey);
            keyTemplate.keyDataMap.Set(fieldName, data);
            keyTemplate.keyDataMap.Detach();
        }

        public void SetKeyData(string fieldName, int data)
        {
            keyTemplate.keyDataMap.Attach(rawKey);
            keyTemplate.keyDataMap.Set(fieldName, data);
            keyTemplate.keyDataMap.Detach();
        }

        public void SetKeyData(string fieldName, DateTime date)
        {
            keyTemplate.keyDataMap.Attach(rawKey);
            keyTemplate.keyDataMap.Set(fieldName, date);
            keyTemplate.keyDataMap.Detach();
        }

        public void SetValidationData(string fieldName, byte[] buf)
        {
            keyTemplate.validationDataMap.Attach(validationData);
            keyTemplate.validationDataMap.Set(fieldName, buf);
            keyTemplate.validationDataMap.Detach();
        }

        public void SetValidationData(string fieldName, string data)
        {
            keyTemplate.validationDataMap.Attach(validationData);
            keyTemplate.validationDataMap.Set(fieldName, data);
            keyTemplate.validationDataMap.Detach();
        }

        public void SetValidationData(string fieldName, int data)
        {
            keyTemplate.validationDataMap.Attach(validationData);
            keyTemplate.validationDataMap.Set(fieldName, data);
            keyTemplate.validationDataMap.Detach();
        }

        public void SetValidationData(string fieldName, DateTime date)
        {
            keyTemplate.validationDataMap.Attach(validationData);
            keyTemplate.validationDataMap.Set(fieldName, date);
            keyTemplate.validationDataMap.Detach();
        }

        public string GenerateKey()
        {
            if (!keyTemplate.HasPrivateKey)
            {
                if (string.IsNullOrEmpty(keyTemplate.SigningServiceUrl))
                    throw new Exception("no private key or signing service available so key generation is not possible");

                string requestUrl = keyTemplate.SigningServiceUrl;
                if (!requestUrl.EndsWith("/")) requestUrl += "/";
                requestUrl += "Activate.ashx?";

                BitStream bits = new BitStream();
                bits.Attach(validationData.Buffer, validationData.Length);
                byte[] val = new byte[(validationData.Length + 7) >> 3];

                string name; int size, offset; BitStruct.FieldType type;
                object enumHandle = null;
                while (keyTemplate.validationDataMap.EnumFields(ref enumHandle, out name, out type, out size, out offset))
                {
                    bits.Seek(offset);
                    bits.Read(val, size);
                    requestUrl += name + "=";

                    if ((LicenseKeyFieldType)type == LicenseKeyFieldType.String)
                        requestUrl += Encoding.UTF8.GetString(val, 0, (size + 7) >> 3) + "&";
                    else
                        if ((LicenseKeyFieldType)type == LicenseKeyFieldType.Raw)
                            requestUrl += Uri.EscapeDataString(Convert.ToBase64String(val, 0, (size + 7) >> 3)) + "&";
                }

                if (keyTemplate.SigningServiceTemplateId != 0)
                {
                    requestUrl += "ProductId=" + keyTemplate.SigningServiceTemplateId + "&";
                }

                if (!String.IsNullOrEmpty(keyTemplate.SigningServiceParameters))
                {
                    requestUrl += keyTemplate.SigningServiceParameters + "&";
                }

                requestUrl = requestUrl.Remove(requestUrl.Length - 1);

                /*
                HttpWebRequest request = (HttpWebRequest)WebRequest.Create(requestUrl);

                request.Method = "GET";
                request.ContentLength = 0;
                request.CachePolicy = new HttpRequestCachePolicy(HttpRequestCacheLevel.NoCacheNoStore);

                request.Proxy.Credentials = CredentialCache.DefaultCredentials;

                HttpWebResponse response = (HttpWebResponse)request.GetResponse();

                HttpStatusCode status = response.StatusCode;
                if (status != HttpStatusCode.OK)
                    throw new Exception("Signing server exception. Code: " + (int)status + " Message: " + response.StatusDescription);

                StreamReader reader = new StreamReader(request.GetResponse().GetResponseStream());
                string licenseKey = reader.ReadToEnd();
                */

                HttpClient client = new HttpClient();
                 
                return client.GetStringAsync(requestUrl).Result;;
            }

            BitStream signedData = new BitStream();
            byte[] signature;
            Signer signer = new Signer();
            int sigLenBits = 0;

            // signed data is license key data followed by validation data
            signedData.Create(((keyTemplate.DataSize + keyTemplate.ValidationDataSize + 7) >> 3) << 3);

            if (keyTemplate.DataSize > 0)
                signedData.Write(rawKey.Buffer, keyTemplate.DataSize);

            if (keyTemplate.ValidationDataSize > 0)
                signedData.Write(validationData.Buffer, keyTemplate.ValidationDataSize);

            signedData.ZeroPadToNextByte();

            // sign data

            // we use a different algorithm than ECDSA when the signature size must be smaller than twice the key size
            if (keyTemplate.SignatureSize < (keyTemplate.SignatureKeySize << 1))
                signer.SetHashSize(keyTemplate.SignatureSize - keyTemplate.SignatureKeySize);
            else
                signer.SetHashSize(0);

            signer.SetPrivateKey(keyTemplate.SigningKey);

            signer.Sign(signedData.Buffer, 0, signedData.Buffer.Length, out signature, out sigLenBits);

            Debug.Assert(sigLenBits == keyTemplate.SignatureSize);

            // write the signature at the end of key data
            rawKey.Seek(keyTemplate.DataSize);
            rawKey.Write(signature, keyTemplate.SignatureSize);

            // text-encode the license key
            EncodeKey();

            // success
            return key;
        }

        void EncodeKey()
        {
            // reverse last byte from the stream, 
            // so that we will not lose significant bits in the padless, truncated BAS32/64 encoding

            switch (keyTemplate.KeyEncoding)
            {
                case LicenseKeyEncoding.Base32X:
                    {
                        int lastBytePtr = ((rawKey.Size + 7) >> 3) - 1;
                        byte[] buffer = rawKey.Buffer;
                        buffer[lastBytePtr] = (byte)(((buffer[lastBytePtr] * 0x0802LU & 0x22110LU) | (buffer[lastBytePtr] * 0x8020LU & 0x88440LU)) * 0x10101LU >> 16);

                        key = Base32.Encode(rawKey.Buffer, 0, rawKey.Buffer.Length);

                        key = key.Substring(0, keyTemplate.NumberOfGroups * keyTemplate.CharactersPerGroup);
                    }
                    break;

                case LicenseKeyEncoding.Base64:
                    {
                        key = Convert.ToBase64String(rawKey.Buffer);
                        key = key.TrimEnd(new char[1] {'='});
                    }
                    break;

                default:
                    throw new Exception();
            }

            int i;
            int insertPos;
            // separate character groups
            for (i = 0, insertPos = keyTemplate.CharactersPerGroup; i < keyTemplate.NumberOfGroups - 1; i++)
            {
                key = key.Insert(insertPos, keyTemplate.GroupSeparator);
                insertPos += (keyTemplate.GroupSeparator.Length + keyTemplate.CharactersPerGroup);
            }

            // add header and footer (if any)
            if (keyTemplate.Header != null)
                if (keyTemplate.Header.Length > 0)
                    key = key.Insert(0, keyTemplate.Header + "\r\n");

            if (keyTemplate.Footer != null)
                if (keyTemplate.Footer.Length > 0)
                    key += ("\r\n" + keyTemplate.Footer);
        }

        private string key;
        private BitStream rawKey;
        private BitStream validationData;
        private LicenseTemplate keyTemplate;
    }
}
