// Decompiled with JetBrains decompiler
// Type: PaddleSDK.PaddleCredentials
// Assembly: PaddleSDK, Version=1.0.0.0, Culture=neutral, PublicKeyToken=b2cf3dc44105319d
// MVID: C7382385-AD81-415C-84F8-E0CBBE666E17
// Assembly location: D:\Projects\fga_source\paddle\Windows-Framework-master\PaddleSDK.dll

namespace PaddleSDK
{
  internal class PaddleCredentials
  {
    /// <summary>
    /// The Paddle API key used to commuinicate with Paddle.com for this product.
    /// </summary>
    public string ApiKey { get; protected set; }

    /// <summary>The Id of the Paddle vendor for this product.</summary>
    public long VendorId { get; protected set; }

    /// <summary>The Id of the product being licensed.</summary>
    public string ProductId { get; protected set; }

    /// <summary>
    /// Creates a new PaddleCredentials to wrap the authentication information that identifies this product.
    /// </summary>
    /// <param name="apiKey">The Paddle API key used to commuinicate with Paddle.com for this product.</param>
    /// <param name="vendorId">The Id of the Paddle vendor for this product.</param>
    /// <param name="productId">The Id of the product being licensed.</param>
    public PaddleCredentials(string apiKey, long vendorId, string productId)
    {
      this.ApiKey = apiKey;
      this.VendorId = vendorId;
      this.ProductId = productId;
    }
  }
}
