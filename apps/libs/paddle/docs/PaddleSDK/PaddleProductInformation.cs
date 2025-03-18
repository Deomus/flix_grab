// Decompiled with JetBrains decompiler
// Type: PaddleSDK.PaddleProductInformation
// Assembly: PaddleSDK, Version=1.0.0.0, Culture=neutral, PublicKeyToken=b2cf3dc44105319d
// MVID: C7382385-AD81-415C-84F8-E0CBBE666E17
// Assembly location: D:\Projects\fga_source\paddle\Windows-Framework-master\PaddleSDK.dll

using System;

namespace PaddleSDK
{
  /// <summary>Information about the product being sold.</summary>
  public class PaddleProductInformation
  {
    /// <summary>The name of this product (e.g. My Awesome App).</summary>
    public string ProductName;
    /// <summary>
    /// The name of the developer for this product (e.g. My Corp).
    /// </summary>
    public string DeveloperName;
    /// <summary>The currency this product is in (e.g. USD).</summary>
    public string Currency;
    /// <summary>
    /// The default price of this product in the provided currency (e.g. 10.99).
    /// </summary>
    public Decimal Price;
    /// <summary>
    /// URL to a publically available image of the product to show on previews (e.g. https://vendors.paddle.com/assets/images/checkout/default_product_icon.png).
    /// </summary>
    public string ImageUrl;
    /// <summary>
    /// The default trial length for this product in days (e.g. 7).
    /// </summary>
    public uint TrialLength;
    /// <summary>
    /// The short line of text shown to customers on the licensing window (e.g. Thanks for downloading a trial of our My Awesome App).
    /// </summary>
    public string TrialText;
    /// <summary>
    /// The URL used for checkouts. Normally you should let the SDK manage this internally.
    /// </summary>
    public string PaymentUrl;
  }
}
