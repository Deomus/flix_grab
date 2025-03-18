// Decompiled with JetBrains decompiler
// Type: PaddleSDK.PaddleDataStore
// Assembly: PaddleSDK, Version=1.0.0.0, Culture=neutral, PublicKeyToken=b2cf3dc44105319d
// MVID: C7382385-AD81-415C-84F8-E0CBBE666E17
// Assembly location: D:\Projects\fga_source\paddle\Windows-Framework-master\PaddleSDK.dll

using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.Web.Script.Serialization;

namespace PaddleSDK
{
  internal class PaddleDataStore
  {
    /// <summary>The product Id this store is writing for.</summary>
    private const string PaddleRegistryRoot = "Software\\Paddle";
    /// <summary>The product Id this store is writing for.</summary>
    private string ProductId;

    /// <summary>The dictionary we store all our data in.</summary>
    private Dictionary<string, object> Data;
    /// <summary>The encryption key we use.</summary>
    private PaddleDataStoreCryptography StoreCryptography;
    /// <summary>The registry key we hold everything under.</summary>
    private string RegistryKey;
    /// <summary>Serializer we use for writing data.</summary>
    private JavaScriptSerializer Serializer;
    /// <summary>Gets if the dictionary has been populated yet or not.</summary>
    private bool IsLoaded;

    /// <summary>The activation Id for this product if activated.</summary>
    public string ActivationId
    {
      get
      {
        return (string) this.GetValue("activation_id");
      }
      set
      {
        this.StoreValue("activation_id", (object) value);
      }
    }

    /// <summary>
    /// When the product was first run (called Start in other SDKs).
    /// </summary>
    public DateTime? FirstRun
    {
      get
      {
        return (DateTime?) this.GetValue("start");
      }
      set
      {
        this.StoreValue("start", (object) value);
      }
    }

    /// <summary>The license key for this product if activated.</summary>
    public string LicenseKey
    {
      get
      {
        return (string) this.GetValue("license_key");
      }
      set
      {
        this.StoreValue("license_key", (object) value);
      }
    }

    /// <summary>The email that was used to licence this product.</summary>
    public string LicenseEmail
    {
      get
      {
        return (string) this.GetValue("license_email");
      }
      set
      {
        this.StoreValue("license_email", (object) value);
      }
    }

    /// <summary>
    /// The last time the product information was refreshed from the Paddle API.
    /// </summary>
    public DateTime? LastProductInformationRefresh
    {
      get
      {
        return (DateTime?) this.GetValue("last_refresh");
      }
      set
      {
        this.StoreValue("last_refresh", (object) value);
      }
    }

    /// <summary>
    /// Creates a new instace of the PaddleDataStore for storing data about the app.
    /// </summary>
    /// <param name="productId"></param>
    public PaddleDataStore(string productId)
    {
      this.ProductId = productId;
      this.RegistryKey = "HKEY_CURRENT_USER\\Software\\Paddle\\" + this.ProductId;
      this.Data = new Dictionary<string, object>();
      this.Serializer = new JavaScriptSerializer();
      this.StoreCryptography = new PaddleDataStoreCryptography(productId);
    }

    /// <summary>
    /// Stores a value in our data store. Will overwrite if a value already exists.
    /// </summary>
    /// <param name="key">The key for the data being stored.</param>
    /// <param name="value">The value of any data being stored.</param>
    private void StoreValue(string key, object value)
    {
      this.Data[key] = value;
      this.Save();
    }

    /// <summary>
    /// Gets a value from our store. Will return null if value was not found.
    /// </summary>
    /// <param name="key">The key for the data being stored.</param>
    private object GetValue(string key)
    {
      if (!this.IsLoaded)
        this.Load();
      object obj = (object) null;
      if (this.Data.TryGetValue(key, out obj))
        return obj;
      return (object) null;
    }

    /// <summary>
    /// Saves the current data store values to persistent storage.
    /// </summary>
    private void Save()
    {
      Registry.SetValue(this.RegistryKey, "license", (object) this.StoreCryptography.EncryptForStore(this.Serializer.Serialize((object) this.Data)), RegistryValueKind.Binary);
    }

    /// <summary>Loads data for the store.</summary>
    private void Load()
    {
      byte[] data = Registry.GetValue(this.RegistryKey, "license", (object) null) as byte[];
      if (data != null)
      {
        try
        {
          this.Data = this.Serializer.Deserialize<Dictionary<string, object>>(this.StoreCryptography.DecryptFromStore(data));
        }
        catch (Exception ex)
        {
        }
      }
      this.IsLoaded = true;
    }

    /// <summary>
    /// Saves the given product data. Will overwrite all keys.
    /// </summary>
    /// <param name="productData"></param>
    public void SaveProductData(PaddleProductInformation productData)
    {
      this.StoreValue("product", (object) productData);
    }

    /// <summary>
    /// Saves the given product data. Will overwrite all keys.
    /// </summary>
    public PaddleProductInformation GetProductData()
    {
      object obj = this.GetValue("product");
      if (obj != null)
        return this.Serializer.ConvertToType<PaddleProductInformation>(obj);
      return (PaddleProductInformation) null;
    }
  }
}
