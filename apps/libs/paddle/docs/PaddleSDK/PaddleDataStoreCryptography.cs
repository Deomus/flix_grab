// Decompiled with JetBrains decompiler
// Type: PaddleSDK.PaddleDataStoreCryptography
// Assembly: PaddleSDK, Version=1.0.0.0, Culture=neutral, PublicKeyToken=b2cf3dc44105319d
// MVID: C7382385-AD81-415C-84F8-E0CBBE666E17
// Assembly location: D:\Projects\fga_source\paddle\Windows-Framework-master\PaddleSDK.dll

using System.IO;
using System.Security.Cryptography;
using System.Text;

namespace PaddleSDK
{
  internal class PaddleDataStoreCryptography
  {
    /// <summary>
    /// Init vector for PasswordDeriveBytes. Size in bytes must be KeySize / 8. With a KeySize of 256 we need 32 bytes (16 UTF16 chars)
    /// </summary>
    private static readonly byte[] InitVector = Encoding.ASCII.GetBytes("j3M7*9yz9SsK&Dho");
    /// <summary>Key size for the encryption algorithm.</summary>
    private const int KeySize = 256;
    /// <summary>The encryption key used to write the data.</summary>
    private string EncryptionKey;

    /// <summary>
    /// Creates a new PaddleDataStoreCryptography for encrypting data written to the store.
    /// </summary>
    /// <param name="productId"></param>
    public PaddleDataStoreCryptography(string productId)
    {
      this.EncryptionKey = "XlsJ$u*Zt\"pX@hIuY~1!jmBp0u9" + productId;
    }

    /// <summary>Encrypts the given string based on our magic key.</summary>
    /// <param name="data"></param>
    /// <returns></returns>
    public byte[] EncryptForStore(string data)
    {
      byte[] bytes1 = Encoding.UTF8.GetBytes(data);
      using (PasswordDeriveBytes passwordDeriveBytes = new PasswordDeriveBytes(this.EncryptionKey, (byte[]) null))
      {
        byte[] bytes2 = passwordDeriveBytes.GetBytes(32);
        using (RijndaelManaged rijndaelManaged = new RijndaelManaged())
        {
          rijndaelManaged.Mode = CipherMode.CBC;
          using (ICryptoTransform encryptor = rijndaelManaged.CreateEncryptor(bytes2, PaddleDataStoreCryptography.InitVector))
          {
            using (MemoryStream memoryStream = new MemoryStream())
            {
              using (CryptoStream cryptoStream = new CryptoStream((Stream) memoryStream, encryptor, CryptoStreamMode.Write))
              {
                cryptoStream.Write(bytes1, 0, bytes1.Length);
                cryptoStream.FlushFinalBlock();
                return memoryStream.ToArray();
              }
            }
          }
        }
      }
    }

    /// <summary>Decrypts the given store data based on our magic key.</summary>
    public string DecryptFromStore(byte[] data)
    {
      using (PasswordDeriveBytes passwordDeriveBytes = new PasswordDeriveBytes(this.EncryptionKey, (byte[]) null))
      {
        byte[] bytes = passwordDeriveBytes.GetBytes(32);
        using (RijndaelManaged rijndaelManaged = new RijndaelManaged())
        {
          rijndaelManaged.Mode = CipherMode.CBC;
          using (ICryptoTransform decryptor = rijndaelManaged.CreateDecryptor(bytes, PaddleDataStoreCryptography.InitVector))
          {
            using (MemoryStream memoryStream = new MemoryStream(data))
            {
              using (CryptoStream cryptoStream = new CryptoStream((Stream) memoryStream, decryptor, CryptoStreamMode.Read))
              {
                byte[] numArray = new byte[data.Length];
                int count = cryptoStream.Read(numArray, 0, numArray.Length);
                return Encoding.UTF8.GetString(numArray, 0, count);
              }
            }
          }
        }
      }
    }
  }
}
