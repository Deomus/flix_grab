// Decompiled with JetBrains decompiler
// Type: PaddleSDK.PaddleProduct
// Assembly: PaddleSDK, Version=1.0.0.0, Culture=neutral, PublicKeyToken=b2cf3dc44105319d
// MVID: C7382385-AD81-415C-84F8-E0CBBE666E17
// Assembly location: D:\Projects\fga_source\paddle\Windows-Framework-master\PaddleSDK.dll

using Microsoft.Win32;
using PaddleSDK.Util;
using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Globalization;
using System.Linq;
using System.Net;
using System.Net.NetworkInformation;
using System.Text;
using System.Threading;
using System.Web.Script.Serialization;

namespace PaddleSDK
{
  internal class PaddleProduct
  {
    /// <summary>
    /// Array of countries ISO codes that we say are "in Europe" (enough to display EUR currencies by default).
    /// </summary>
    public static readonly string[] EuropeCounrties = new string[26]
    {
      "AT",
      "BE",
      "BG",
      "CR",
      "CY",
      "CZ",
      "DK",
      "EE",
      "FI",
      "FR",
      "DE",
      "GR",
      "HU",
      "IE",
      "IT",
      "LV",
      "LT",
      "MT",
      "NL",
      "PL",
      "PT",
      "RO",
      "SK",
      "SI",
      "ES",
      "SE"
    };
    /// <summary>The base URL for calls to API server.</summary>
    public const string ApiServerBaseUrl = "v3.paddleapi.com";
    /// <summary>The credentials needed to talk to the API server.</summary>
    private PaddleCredentials Credentials;
    /// <summary>The store we use to save data.</summary>
    private PaddleDataStore Store;

    /// <summary>The information we have on this product.</summary>
    public PaddleProductInformation ProductData { get; private set; }

    /// <summary>The last error message set by the API.</summary>
    public string LastErrorMessage { get; private set; }

    /// <summary>
    /// Gets the days remaining on this product's trial. Will return negative days if trial has expired. Will return 0 if trial
    /// was not started, or this product has no trial.
    /// </summary>
    public int DaysRemainingOnTrial
    {
      get
      {
        DateTime? firstRun = this.Store.FirstRun;
        if ((this.ProductData != null || this.GetProductInfo()) && (firstRun.HasValue && this.ProductData.TrialLength > 0U))
          return (int) this.ProductData.TrialLength - (int) Math.Floor((DateTime.UtcNow - firstRun.Value).TotalDays);
        return 0;
      }
    }

    /// <summary>
    /// Creates a new PaddleProduct instance for managing the product.
    /// </summary>
    /// <param name="credentials">Credentials needed to communicate with the Paddle API for this product.</param>
    /// <param name="store">The data store to persist data back to.</param>
    public PaddleProduct(PaddleCredentials credentials, PaddleDataStore store)
    {
      this.Credentials = credentials;
      this.Store = store;
    }

    /// <summary>Fetches product information from the API if needed.</summary>
    public bool GetProductInfo()
    {
      PaddleProductInformation productData1 = this.Store.GetProductData();
      DateTime? informationRefresh = this.Store.LastProductInformationRefresh;
      if (productData1 == null || !informationRefresh.HasValue || informationRefresh.Value > DateTime.UtcNow.AddHours(-24.0))
      {
        Dictionary<string, object> dict = this.MakeApiRequest("product/data", new NameValueCollection()
        {
          {
            "api_key",
            this.Credentials.ApiKey
          },
          {
            "vendor_id",
            this.Credentials.VendorId.ToString()
          },
          {
            "product_id",
            this.Credentials.ProductId
          }
        });
        if (dict == null)
        {
          if (productData1 == null)
            return false;
          this.ProductData = productData1;
          return true;
        }
        PaddleProductInformation productData2 = new PaddleProductInformation();
        productData2.ProductName = (string) dict.GetValueOrNull<string, object>("name", (object) null);
        productData2.DeveloperName = (string) dict.GetValueOrNull<string, object>("vendor_name", (object) null);
        productData2.PaymentUrl = (string) dict.GetValueOrNull<string, object>("payment_url", (object) null);
        productData2.ImageUrl = (string) dict.GetValueOrNull<string, object>("image", (object) null);
        Dictionary<string, object> valueOrNull1 = dict.GetValueOrNull<string, object>("current_prices", (object) null) as Dictionary<string, object>;
        if (valueOrNull1 != null && valueOrNull1.Count > 0)
        {
          string key = this.GetCurrencyForCountry();
          if (!valueOrNull1.ContainsKey(key))
            key = valueOrNull1.Keys.First<string>();
          productData2.Currency = key;
          productData2.Price = Decimal.Parse(valueOrNull1[key].ToString());
          Dictionary<string, object> valueOrNull2 = dict.GetValueOrNull<string, object>("discount_value", (object) null) as Dictionary<string, object>;
          if (valueOrNull2 != null && valueOrNull2.ContainsKey(key))
            Decimal.Parse(valueOrNull2[key].ToString());
          Dictionary<string, object> valueOrNull3 = dict.GetValueOrNull<string, object>("discount_percentage", (object) null) as Dictionary<string, object>;
          if (valueOrNull3 != null && valueOrNull3.ContainsKey(key))
            Decimal.Parse(valueOrNull3[key].ToString());
        }
        Dictionary<string, object> valueOrNull4 = dict.GetValueOrNull<string, object>("trial", (object) null) as Dictionary<string, object>;
        if (valueOrNull4 != null && valueOrNull4.Count > 0)
        {
          productData2.TrialLength = (uint) (int) valueOrNull4.GetValueOrNull<string, object>("duration", (object) 0);
          productData2.TrialText = (string) valueOrNull4.GetValueOrNull<string, object>("text", (object) null);
        }
        this.ProductData = productData2;
        this.Store.SaveProductData(productData2);
        this.Store.LastProductInformationRefresh = new DateTime?(DateTime.UtcNow);
        return true;
      }
      this.ProductData = productData1;
      return true;
    }

    /// <summary>Activates this product licence.</summary>
    /// <param name="email">The email to associate the activation with</param>
    /// <param name="licenseKey">The liscence key to authorise the activation.</param>
    public bool ActivateLicence(string email, string licenseKey)
    {
      Dictionary<string, object> dict = this.MakeApiRequest("license/activate", new NameValueCollection()
      {
        {
          "api_key",
          this.Credentials.ApiKey
        },
        {
          "product_id",
          this.Credentials.ProductId
        },
        {
          "email",
          email
        },
        {
          "license_key",
          licenseKey
        },
        {
          "uuid",
          this.GetMachineUUID()
        }
      });
      if (dict != null)
      {
        string valueOrNull = (string) dict.GetValueOrNull<string, object>("activation_id", (object) null);
        if (valueOrNull != null)
        {
          this.Store.ActivationId = valueOrNull;
          this.Store.LicenseKey = licenseKey;
          this.Store.LicenseEmail = email;
          return true;
        }
      }
      return false;
    }

    /// <summary>Calls the API to deactive this product's licence.</summary>
    /// <returns></returns>
    public bool DeactivateLicence()
    {
      string activationId = this.Store.ActivationId;
      if (activationId != null)
      {
        if (this.MakeApiRequest("license/deactivate", new NameValueCollection()
        {
          {
            "api_key",
            this.Credentials.ApiKey
          },
          {
            "product_id",
            this.Credentials.ProductId
          },
          {
            "activation_id",
            activationId
          }
        }) != null || this.LastErrorMessage.Contains("Unable to find requested activation"))
        {
          this.Store.ActivationId = (string) null;
          return true;
        }
      }
      return false;
    }

    /// <summary>Issues the given request batch to Calq's API servers.</summary>
    /// <param name="endpoint">The API endpoint to call (e.g. product/data)</param>
    /// <param name="apiParameters">The parameters needed to make this API request.</param>
    private Dictionary<string, object> MakeApiRequest(string endpoint, NameValueCollection apiParameters)
    {
      apiParameters.Add("language", Thread.CurrentThread.CurrentUICulture.Name);
      string input;
      try
      {
        input = Encoding.UTF8.GetString(new WebClient().UploadValues(new Uri(string.Format("{0}://{1}/3.0/{2}", (object) "https", (object) "v3.paddleapi.com", (object) endpoint)), apiParameters));
      }
      catch (WebException ex)
      {
        return (Dictionary<string, object>) null;
      }
      JavaScriptSerializer scriptSerializer = new JavaScriptSerializer();
      Dictionary<string, object> dictionary;
      try
      {
        dictionary = scriptSerializer.Deserialize<Dictionary<string, object>>(input);
      }
      catch (Exception ex)
      {
        return (Dictionary<string, object>) null;
      }
      object obj1;
      if (!dictionary.TryGetValue("success", out obj1) || !(bool) obj1)
      {
        object obj2;
        if (dictionary.TryGetValue("error", out obj2))
          this.LastErrorMessage = (string) (obj2 as Dictionary<string, object>)["message"];
        return (Dictionary<string, object>) null;
      }
      object obj3 = dictionary["response"];
      if (obj3 is string && string.IsNullOrEmpty((string) obj3))
        return new Dictionary<string, object>();
      return obj3 as Dictionary<string, object>;
    }

    /// <summary>
    /// Gets an ID we can use to identify this machine for our licenses later on.
    /// </summary>
    private string GetMachineUUID()
    {
      return RegistryKey.OpenBaseKey(RegistryHive.LocalMachine, Environment.Is64BitOperatingSystem ? RegistryView.Registry64 : RegistryView.Registry32).OpenSubKey("SOFTWARE\\Microsoft\\Cryptography", false).GetValue("MachineGuid") as string ?? this.GetMacAddress();
    }

    /// <summary>Finds the MAC address of the NIC with maximum speed.</summary>
    private string GetMacAddress()
    {
      string str1 = (string) null;
      long num = -1;
      foreach (NetworkInterface networkInterface in NetworkInterface.GetAllNetworkInterfaces())
      {
        string str2 = networkInterface.GetPhysicalAddress().ToString();
        if (networkInterface.Speed > num && !string.IsNullOrEmpty(str2) && str2.Length >= 12)
        {
          num = networkInterface.Speed;
          str1 = str2;
        }
      }
      return str1;
    }

    /// <summary>
    /// Gets the currency used by the given country (that Paddle supports).
    /// </summary>
    /// <returns></returns>
    private string GetCurrencyForCountry()
    {
      string upperInvariant = RegionInfo.CurrentRegion.TwoLetterISORegionName.ToUpperInvariant();
      if (upperInvariant == "GB")
        return "GBP";
      return ((IEnumerable<string>) PaddleProduct.EuropeCounrties).Contains<string>(upperInvariant) ? "EUR" : "USD";
    }
  }
}
