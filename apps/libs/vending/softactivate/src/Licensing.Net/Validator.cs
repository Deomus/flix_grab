//
// Copyright (c) 2014 SoftActivate. All rights reserved.
//

using System;
using System.Collections.Generic;
using System.Text; 

namespace SoftActivate.Licensing
{
    public class KeyValidator
    {
        public KeyValidator()
        {

        }

        public KeyValidator(LicenseTemplate licenseKeyTemplate)
            : this(licenseKeyTemplate, null)
        {

        }

        public KeyValidator(LicenseTemplate _keyTemplate, string _key)
        {
            SetKeyTemplate(_keyTemplate);
            if (_key != null)
                SetKey(_key);
        }

        ~KeyValidator()
        {

        }

        // Set the template used for validation (the template contains the license key format and the ECC verification key)
        public void SetKeyTemplate(LicenseTemplate templ)
        {
            if (templ == null)
                throw new Exception();

            keyTemplate = templ;
            validationData = new BitStream();
            validationData.Create(keyTemplate.ValidationDataSize);
        }

        // Set the license key used for validation and data query
        public void SetKey(string strKey)
        {
            string key = strKey;

            if (string.IsNullOrEmpty(strKey))
                throw new Exception("invalid license key");

            if (strKey.Length < keyTemplate.Header.Length +
                              keyTemplate.Footer.Length +
                              keyTemplate.CharactersPerGroup * keyTemplate.NumberOfGroups +
                              keyTemplate.GroupSeparator.Length * (keyTemplate.NumberOfGroups - 1))
            {
                throw new Exception("invalid license key (too short)");
            }

            // remove header and footer (if any)
            if (keyTemplate.Header != null)
                if (keyTemplate.Header.Length > 0)
                {
                    key = key.Remove(0, keyTemplate.Header.Length + 2);
                }

            if (keyTemplate.Footer != null)
                if (keyTemplate.Footer.Length > 0)
                {
                    key = key.Remove(key.Length - keyTemplate.Footer.Length - 2, keyTemplate.Footer.Length + 2);
                }

            // ungroup license key
            for (int i = 0, erasePos = 0; i < keyTemplate.NumberOfGroups - 1; i++)
            {
                erasePos += keyTemplate.CharactersPerGroup;
                key = key.Remove(erasePos, keyTemplate.GroupSeparator.Length);
            }

            rawKey = new BitStream();

            // decode license key
            switch (keyTemplate.KeyEncoding)
            {
                case LicenseKeyEncoding.Base32X:
                    {
                        byte[] keyBuf = Base32.Decode(key, 0, key.Length);
                        // reverse last byte
                        keyBuf[keyBuf.Length - 1] = (byte)(((keyBuf[keyBuf.Length - 1] * 0x0802LU & 0x22110LU) | (keyBuf[keyBuf.Length - 1] * 0x8020LU & 0x88440LU)) * 0x10101LU >> 16);

                        rawKey.Attach(keyBuf, (int)keyTemplate.KeyEncoding * keyTemplate.CharactersPerGroup * keyTemplate.NumberOfGroups);
                    }
                    break;

                case LicenseKeyEncoding.Base64:
                    {
                        if ((key.Length & 3) != 0)
                        {
                            key = key.PadRight(key.Length + 4 - (key.Length & 3), '=');
                        }

                        byte[] keyBuf = Convert.FromBase64String(key);
                        rawKey.Attach(keyBuf, (int)keyTemplate.KeyEncoding * keyTemplate.CharactersPerGroup * keyTemplate.NumberOfGroups);
                    }
                    break;

                default:
                    throw new Exception();
            }
        }

        public void SetValidationData(string fieldName, byte[] buf)
        {
            keyTemplate.validationDataMap.Attach(validationData);
            keyTemplate.validationDataMap.Set(fieldName, buf);
            keyTemplate.validationDataMap.Detach();
        }

        public void SetValidationData(string fieldName, int data)
        {
            keyTemplate.validationDataMap.Attach(validationData);
            keyTemplate.validationDataMap.Set(fieldName, data);
            keyTemplate.validationDataMap.Detach();
        }

        public void SetValidationData(string fieldName, string data)
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

        // Verifies the license key's signature using the public(verification) ECC key from the template
        public bool IsKeyValid()
        {
            Verifier verifier = new Verifier();
            BitStream signedData = new BitStream(),
                      signature = new BitStream();

            signedData.Create(((keyTemplate.DataSize + keyTemplate.ValidationDataSize + 7) >> 3) << 3);
            signedData.Clear();
            if (keyTemplate.DataSize > 0)
                signedData.Write(rawKey.Buffer, keyTemplate.DataSize);
            if (keyTemplate.ValidationDataSize > 0)
                signedData.Write(validationData.Buffer, keyTemplate.ValidationDataSize);

            signedData.ZeroPadToNextByte();

            byte[] sigBytes = new byte[(keyTemplate.SignatureSize + 7) >> 3];
            rawKey.Seek(keyTemplate.DataSize);
            rawKey.Read(sigBytes, keyTemplate.SignatureSize);

            signature.Attach(sigBytes, keyTemplate.SignatureSize);
            signature.Seek(keyTemplate.SignatureSize);
            signature.ZeroPadToNextByte();

            // we use a different algorithm than ECDSA when the signature size must be smaller than twice the key size
            if (keyTemplate.SignatureSize < (keyTemplate.SignatureKeySize << 1))
                verifier.SetHashSize((uint)(keyTemplate.SignatureSize - keyTemplate.SignatureKeySize));
            else
                verifier.SetHashSize(0);

            verifier.SetPublicKey(keyTemplate.VerificationKey);

            return verifier.Verify(signedData.Buffer, 0, signedData.Buffer.Length, signature.Buffer, 0, signature.Buffer.Length, keyTemplate.SignatureSize);
        }

        // Queries the license key for data. The data is returned in raw format (a byte buffer).
        public byte[] QueryKeyData(string fieldName)
        {
            keyTemplate.keyDataMap.Attach(rawKey);
            byte[] result = keyTemplate.keyDataMap.Get(fieldName);
            keyTemplate.keyDataMap.Detach();
            return result;
        }

        public int QueryIntKeyData(string fieldName)
        {
            keyTemplate.keyDataMap.Attach(rawKey);
            int result = keyTemplate.keyDataMap.GetInt(fieldName);
            keyTemplate.keyDataMap.Detach();
            return result;
        }

        public DateTime QueryDateKeyData(string fieldName)
        {
            keyTemplate.keyDataMap.Attach(rawKey);
            DateTime result = keyTemplate.keyDataMap.GetDate(fieldName);
            keyTemplate.keyDataMap.Detach();
            return result;
        }

        public byte[] QueryValidationData(string fieldName)
        {
            keyTemplate.validationDataMap.Attach(validationData);
            byte[] result = keyTemplate.validationDataMap.Get(fieldName);
            keyTemplate.validationDataMap.Detach();
            return result;
        }

        private BitStream rawKey;
        private BitStream validationData;
        private LicenseTemplate keyTemplate;
    }
}
