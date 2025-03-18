using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Http;
using System.Text;

namespace SoftActivate.Licensing
{
    public class Certificate
    {
        private Certificate()
        {

        }

        public Certificate(string certificate) : this(Convert.FromBase64String(certificate))
        {
            
        }

        public Certificate(byte[] certificate)
        {
            certBits = new BitStruct(certificate.Length * 8);

            Buffer.BlockCopy(certificate, 0, certBits.Buffer, 0, certificate.Length);

            certBits.BeginFieldOffset();

            certBits.AddField("Version", BitStruct.FieldType.Integer, 4);
            certBits.AddField("IsSelfSigned", BitStruct.FieldType.Integer, 1);
            certBits.AddField("ReservedFlags", BitStruct.FieldType.Integer, 3);
            certBits.AddField("ExpirationDate", BitStruct.FieldType.Date16, 16);
            certBits.AddField("PublicKeyLen", BitStruct.FieldType.Integer, 8);        
            certBits.AddField("PublicKey", BitStruct.FieldType.Raw, certBits.GetInt("PublicKeyLen") * 8);

            int signedPartLen = (certBits.CurrentFieldOffset + 7) >> 3;

            if (certificate.Length <= signedPartLen)
                // certificate is not signed
                return;

            certBits.AddField("SignatureSizeBits", BitStruct.FieldType.Integer, 16);

            int signatureBitLen = certBits.GetInt("SignatureSizeBits");
            int signatureLen = (signatureBitLen + 7) >> 3;

            int signatureOffset = certBits.CurrentFieldOffset >> 3;

            certBits.AddField("Signature", BitStruct.FieldType.Raw, signatureLen << 3);

            if (certificate.Length > 4 + certBits.GetInt("PublicKeyLen") + 2 + signatureLen + 1)
            {
                certBits.AddField("PrivateKeySize", BitStruct.FieldType.Integer, 8);
                certBits.AddField("PrivateKey", BitStruct.FieldType.Raw, certBits.GetInt("PrivateKeySize") * 8);
            }
        }

        public static Certificate Generate(int signatureSize, DateTime? expirationTime, string base64PrivateKey, string base64PublicKey, string base64SigningPrivateKey, string signingCertificate, string certificateAuthorityUrl, bool legacyKeyFormat = false)
        {
            return Generate(signatureSize, 
                            expirationTime, 
                            String.IsNullOrEmpty(base64PrivateKey) ? null : new Key(base64PrivateKey, (legacyKeyFormat) ? GetKeyType(signatureSize) : Ecc.KeyType.Ecc),
                            String.IsNullOrEmpty(base64PublicKey) ? null : new Key(base64PublicKey, (legacyKeyFormat) ? GetKeyType(signatureSize) : Ecc.KeyType.Ecc), 
                            String.IsNullOrEmpty(base64SigningPrivateKey) ? null : new Key(base64SigningPrivateKey, (legacyKeyFormat) ? GetKeyType(signatureSize) : Ecc.KeyType.Ecc),
                            String.IsNullOrEmpty(signingCertificate) ? null: new Certificate(signingCertificate), 
                            certificateAuthorityUrl);
        }
        
        internal static Certificate Generate(int signatureSize, DateTime? expirationDate, Key privateKey, Key publicKey, Key signingPrivateKey, Certificate signingCertificate, string certificateAuthorityUrl)
        {
            if (publicKey == null)
            {
                throw new Exception("public key for the certificate must be provided");
            }

            bool selfSigned = (signingPrivateKey == null && certificateAuthorityUrl == null);

            byte[] rawPublicKey = publicKey.Export();

            BitStruct certBits = new BitStruct((4 + rawPublicKey.Length) * 8);
            
            certBits.BeginFieldOffset();

            certBits.AddField("Version", BitStruct.FieldType.Integer, 4);
            certBits.AddField("IsSelfSigned", BitStruct.FieldType.Integer, 1);
            certBits.AddField("ReservedFlags", BitStruct.FieldType.Integer, 3);
            certBits.AddField("ExpirationDate", BitStruct.FieldType.Date16, 16);
            certBits.AddField("PublicKeyLen", BitStruct.FieldType.Integer, 8);
            certBits.AddField("PublicKey", BitStruct.FieldType.Raw, rawPublicKey.Length * 8);

            certBits.Set("Version", 0);
            certBits.Set("IsSelfSigned", (selfSigned) ? 1 : 0);
            certBits.Set("ReservedFlags", 0);

            if (expirationDate.HasValue)
                certBits.Set("ExpirationDate", expirationDate.Value);
            else
                certBits.Set("ExpirationDate", DateTime.UtcNow.AddYears(20));
            
            certBits.Set("PublicKeyLen", rawPublicKey.Length);
            certBits.Set("PublicKey", rawPublicKey);

            Key signingKey = null;
            Key verificationKey = null;
            byte[] certSignature;
            int sigSizeBits;

            if (signingPrivateKey != null)
            {
                signingKey = signingPrivateKey;
                
                if (signingCertificate != null)
                    verificationKey = new Key(signingCertificate.PublicKey);
            }
            else
            {
                if (privateKey != null && String.IsNullOrEmpty(certificateAuthorityUrl))
                {
                    signingKey = privateKey;
                    verificationKey = publicKey;
                }
            }

            if (signingKey == null)
            {
                if (!String.IsNullOrEmpty(certificateAuthorityUrl))
                {
                    HttpClient client = new HttpClient();

                    // WebClient client = new WebClient();

                    /*
                    client.Proxy = WebRequest.DefaultWebProxy;
                    if (client.Proxy != null)
                    {
                        client.Proxy.Credentials = CredentialCache.DefaultCredentials;
                    }
                    */
                    
                    Certificate issuedCert = new Certificate(client.GetStringAsync(certificateAuthorityUrl + "?csr=" + Uri.EscapeDataString(Convert.ToBase64String(certBits.Buffer)) + (String.IsNullOrEmpty(SDKRegistration.LicenseKey) ? "" : "&sdklicensekey=" + SDKRegistration.LicenseKey)).Result);
                    Buffer.BlockCopy(issuedCert.certBits.Buffer, 0, certBits.Buffer, 0, 4 + rawPublicKey.Length);
                    sigSizeBits = issuedCert.certBits.GetInt("SignatureSizeBits");
                    certSignature = issuedCert.certBits.Get("Signature");
                }
                else
                    throw new Exception("no means for certificate signing were provided");
            }
            else
            {
                Signer signer = new Signer();
                signer.SetPrivateKey(signingKey);

                signer.Sign(certBits.Buffer, 0, certBits.Buffer.Length, out certSignature, out sigSizeBits);
            }

            if (verificationKey != null)
            {
                Verifier verifier = new Verifier();
                verifier.SetPublicKey(verificationKey);

                if (!verifier.Verify(certBits.Buffer, 0, certBits.Buffer.Length, certSignature, 0, (sigSizeBits + 7) >> 3, sigSizeBits))
                    throw new Exception("internal error while validating signature");
            }
            
            int certBufLen = certBits.Buffer.Length + 2 + certSignature.Length;
            byte[] certBuf = new byte[certBufLen];

            Buffer.BlockCopy(certBits.Buffer, 0, certBuf, 0, certBits.Buffer.Length);

            certBits.Attach(certBuf, certBuf.Length * 8);
            
            certBits.AddField("SignatureSizeBits", BitStruct.FieldType.Integer, 16);
            certBits.Set("SignatureSizeBits", sigSizeBits);

            certBits.AddField("Signature", BitStruct.FieldType.Raw, certSignature.Length * 8);
            certBits.Set("Signature", certSignature);

            Certificate cert = new Certificate();
            cert.certBits = certBits; 

            return cert;
        }

        public static string Sign(string certificate, string privateKey, string signingPrivateKey, string signingCertificate, string certificateAuthorityUrl, DateTime? newExpirationDate = null)
        {
            return Sign(new Certificate(certificate), String.IsNullOrEmpty(privateKey) ? null : new Key(privateKey), String.IsNullOrEmpty(signingPrivateKey) ? null : new Key(signingPrivateKey), String.IsNullOrEmpty(signingCertificate) ? null : new Certificate(signingCertificate), certificateAuthorityUrl, newExpirationDate).ToString();
        }

        internal static Certificate Sign(Certificate cert, Key privateKey, Key signingPrivateKey, Certificate signingCertificate, string certificateAuthorityUrl, DateTime? newExpirationDate = null)
        {
            return Certificate.Generate(0, (newExpirationDate != null) ? newExpirationDate.Value : cert.ExpirationDate, privateKey, new Key(cert.PublicKey), signingPrivateKey, signingCertificate, certificateAuthorityUrl);
        }

        internal static bool VerifySignature(Certificate cert, Certificate validationCert)
        {
            Verifier verifier = new Verifier();
            verifier.SetPublicKey(new Key((validationCert != null) ? validationCert.certBits.Get("PublicKey") : cert.certBits.Get("PublicKey")));

            return verifier.Verify(cert.certBits.Buffer, 
                                    0, 
                                    4 + cert.certBits.GetInt("PublicKeyLen"), 
                                    cert.certBits.Buffer, 
                                    4 + cert.certBits.GetInt("PublicKeyLen") + 2, 
                                    (cert.certBits.GetInt("SignatureSizeBits") + 7) >> 3, 
                                    cert.certBits.GetInt("SignatureSizeBits"));
        }

        public byte[] PublicKey
        {
            get
            {
                return new Key(certBits.Get("PublicKey")).Export();
            }
        }

        public bool SelfSigned
        {
            get
            {
                return certBits.GetInt("IsSelfSigned") != 0;
            }
        }

        public DateTime ExpirationDate
        {
            get
            {
                return certBits.GetDate("ExpirationDate");
            }
        }

        public byte[] Export()
        {
            int len = certBits.Buffer.Length;

            byte[] buf = new byte[ len ];

            Buffer.BlockCopy(certBits.Buffer, 0, buf, 0, len);

            return buf;
        }

        public override string ToString()
        {
             return Convert.ToBase64String(certBits.Buffer);
        }

        private static Ecc.KeyType GetKeyType(int signatureSize)
        {
            Ecc.KeyType keyType;
            int keySize;

            if (signatureSize < 76 || signatureSize > 322)
                throw new Exception("invalid signature size");

            if (signatureSize <= 108)
            {
                keyType = Ecc.KeyType.Ecc54;
                keySize = 54;
            }
            else
                if (signatureSize <= 146)
                {
                    keyType = Ecc.KeyType.Ecc73;
                    keySize = 73;
                }
                else
                    if (signatureSize <= 158)
                    {
                        keyType = Ecc.KeyType.Ecc79;
                        keySize = 79;
                    }
                    else
                        if (signatureSize <= 182)
                        {
                            keyType = Ecc.KeyType.Ecc91;
                            keySize = 91;
                        }
                        else
                            if (signatureSize <= 200)
                            {
                                keyType = Ecc.KeyType.Ecc100;
                                keySize = 100;
                            }
                            else
                                if (signatureSize <= 240)
                                {
                                    keyType = Ecc.KeyType.Ecc120;
                                    keySize = 120;
                                }
                                else
                                    if (signatureSize <= 262)
                                    {
                                        keyType = Ecc.KeyType.Ecc131;
                                        keySize = 131;
                                    }
                                    else
                                        if (signatureSize <= 282)
                                        {
                                            keyType = Ecc.KeyType.Ecc141;
                                            keySize = 141;
                                        }
                                        else
                                        {
                                            keyType = Ecc.KeyType.Ecc161;
                                            keySize = 161;
                                        }

            return keyType;
        }

        private BitStruct certBits;
    }
}
