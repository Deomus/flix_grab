// Decompiled with JetBrains decompiler
// Type: PaddleSDK.Paddle
// Assembly: PaddleSDK, Version=1.0.0.0, Culture=neutral, PublicKeyToken=b2cf3dc44105319d
// MVID: C7382385-AD81-415C-84F8-E0CBBE666E17
// Assembly location: D:\Projects\fga_source\paddle\Windows-Framework-master\PaddleSDK.dll

using System;

namespace PaddleSDK
{
  /// <summary>Main class for interacting with the Paddle SDK.</summary>
  public class Paddle
  {
    /// <summary>Lock used for thread safety.</summary>
    private static object Lock = new object();
    /// <summary>The credentials used to identify this product.</summary>
    private PaddleCredentials Credentials;
    /// <summary>The store that holds the data we save about products.</summary>
    private PaddleDataStore Store;
    /// <summary>
    /// Provides functionality for interacting with the product (licence activation etc).
    /// </summary>
    private PaddleProduct Product;
    /// <summary>Singleton shared instance if created.</summary>
    private static Paddle _Singleton;

    /// <summary>
    /// Gets whether or not this product has an activated liscence.
    /// </summary>
    public bool ProductActivated
    {
      get
      {
        if (this.Store.ActivationId != null && this.Store.LicenseKey != null)
          return true;
        if (!this.Store.FirstRun.HasValue)
          this.Store.FirstRun = new DateTime?(DateTime.UtcNow);
        return false;
      }
    }

    /// <summary>
    /// Gets the number of days remaining on this user's trial. A negative number shows the number of days since the trial ended.
    /// </summary>
    public int DaysRemainingOnTrial
    {
      get
      {
        return this.Product.DaysRemainingOnTrial;
      }
    }

    /// <summary>Gets the duration of this product's trial.</summary>
    public uint TrialLength
    {
      get
      {
        if (this.Product.ProductData != null || this.Product.GetProductInfo())
          return this.Product.ProductData.TrialLength;
        return 0;
      }
    }

    /// <summary>
    /// Sets whether or not to show SDK windows in the task bar (default is true).
    /// </summary>
    public bool ShowInTaskbar { get; set; }

    /// <summary>
    /// The email address that was used to register this product. Will be null if not activated.
    /// </summary>
    public string LicenceEmail
    {
      get
      {
        if (!this.ProductActivated)
          return (string) null;
        return this.Store.LicenseEmail;
      }
    }

    /// <summary>
    /// The liscene key that was used to register this product. Will be null if not activated.
    /// </summary>
    public string LicenceKey
    {
      get
      {
        if (!this.ProductActivated)
          return (string) null;
        return this.Store.LicenseKey;
      }
    }

    /// <summary>
    /// The Paddle API key used to commuinicate with Paddle.com for this product.
    /// </summary>
    public string ApiKey
    {
      get
      {
        return this.Credentials.ApiKey;
      }
    }

    /// <summary>The Id of the Paddle vendor for this product.</summary>
    public long VendorId
    {
      get
      {
        return this.Credentials.VendorId;
      }
    }

    /// <summary>The Id of the product being licensed.</summary>
    public string ProductId
    {
      get
      {
        return this.Credentials.ProductId;
      }
    }

    /// <summary>
    /// Gets the shared instance (singleton) previously created with CreateSharedInstance(...).
    /// </summary>
    public static Paddle SharedInstance
    {
      get
      {
        if (Paddle._Singleton == null)
          throw new PaddleException("CreateSharedInstance(...) has not yet been called. You must call this before using the SharedInstance property");
        return Paddle._Singleton;
      }
    }

    /// <summary>
    /// Creates a new instance of the Paddle SDK for the given product information.
    /// </summary>
    /// <param name="apiKey">The Paddle API key used to commuinicate with Paddle.com for this product.</param>
    /// <param name="vendorId">The Id of the Paddle vendor for this product.</param>
    /// <param name="productId">The Id of the product being licensed.</param>
    public Paddle(string apiKey, long vendorId, string productId)
    {
      this.Credentials = new PaddleCredentials(apiKey, vendorId, productId);
      this.Store = new PaddleDataStore(productId);
      this.Product = new PaddleProduct(this.Credentials, this.Store);
      this.ShowInTaskbar = true;
    }

    /// <summary>
    /// Sets the product information for the first time the app is run. This information is used if the user is offline
    /// or the Paddle API can not be contacted to fetch current live product information. This way the user can still be
    /// prompted or given a trial. If this information is not set then the app will still call the API (but have no
    /// fallback). Live API data (and cached API data) is always favoured over the first run data.
    /// 
    /// You do not have to specify all information, though a minimum of DeveloperName, ProductName, Currency, Price,
    /// and TrialLength is required.
    /// </summary>
    public void SetFirstRunProductInformation(PaddleProductInformation productInformation)
    {
      if (productInformation.DeveloperName == null)
        throw new ArgumentNullException("DeveloperName can not be null");
      if (productInformation.ProductName == null)
        throw new ArgumentNullException("ProductName can not be null");
      if (productInformation.Currency == null)
        throw new ArgumentNullException("Currency can not be null");
      if (productInformation.Price == new Decimal(0))
        throw new ArgumentException("Price must not be 0");
      if ((int) productInformation.TrialLength == 0)
        throw new ArgumentException("TrialLength must not be 0");
      PaddleProductInformation productData = this.Store.GetProductData();
      if (productData != null && productData.PaymentUrl != null)
        return;
      this.Store.SaveProductData(productInformation);
    }

    /// <summary>
    /// Checks the liscensing state for this product. Will spawn a liscence dialog if required. Assumes a time trialed app.
    /// </summary>
    public bool StartLicensing()
    {
      return this.StartLicensing(true);
    }

    /// <summary>
    /// Checks the liscensing state for this product. Will spawn a liscence dialog if required.
    /// </summary>
    /// <param name="isTimeTrialLimited">Whether this liscene is limited by time (true) or is unlimited but locked by feature (false)</param>
    public bool StartLicensing(bool isTimeTrialLimited = true)
    {
      if (!this.ProductActivated)
        return this.ShowProductWindow(isTimeTrialLimited);
      return true;
    }

    /// <summary>Deactivates the licence for this product.</summary>
    public bool DeactivateLicence()
    {
      if (this.ProductActivated)
        return this.Product.DeactivateLicence();
      return false;
    }

    /// <summary>
    /// Triggers the license window to be shown. This will show the liscence information used to activate the product.
    /// If the product is not activated the method will be ignored.
    /// </summary>
    public void ShowLicensingWindow()
    {
      if (!this.ProductActivated || this.Product.ProductData == null && !this.Product.GetProductInfo())
        return;
      PaddleLicenceWindow paddleLicenceWindow = new PaddleLicenceWindow(this.Product);
      paddleLicenceWindow.SetExistingLicence(this.Store.LicenseEmail, this.Store.LicenseKey);
      int num = (int) paddleLicenceWindow.ShowDialog();
    }

    /// <summary>Triggers the product window to be shown.</summary>
    /// <param name="isTimeTrialLimited">Whether this liscene is limited by time (true) or is unlimited but locked by feature (false).</param>
    /// <returns>True if the app should continue, false if the app should exit.</returns>
    private bool ShowProductWindow(bool isTimeTrialLimited = true)
    {
      if (this.Product.ProductData != null || this.Product.GetProductInfo())
      {
        PaddleProductWindow paddleProductWindow = new PaddleProductWindow(this.Product, this.ProductId);
        paddleProductWindow.ShowInTaskbar = this.ShowInTaskbar;
        int num = (int) paddleProductWindow.ShowDialog();
        if (!this.ProductActivated && this.Product.DaysRemainingOnTrial <= 0 && isTimeTrialLimited)
          return false;
      }
      return true;
    }

    /// <summary>
    /// Creates a shared instance (singleton) that can be statically referenced througout and application.
    /// </summary>
    public static Paddle CreateSharedInstance(string apiKey, long vendorId, string productId)
    {
      lock (Paddle.Lock)
      {
        if (Paddle._Singleton != null)
          throw new PaddleException("CreateSharedInstance(...) has already been called");
        Paddle._Singleton = new Paddle(apiKey, vendorId, productId);
        return Paddle._Singleton;
      }
    }
  }
}
