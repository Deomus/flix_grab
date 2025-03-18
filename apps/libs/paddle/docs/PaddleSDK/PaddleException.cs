// Decompiled with JetBrains decompiler
// Type: PaddleSDK.PaddleException
// Assembly: PaddleSDK, Version=1.0.0.0, Culture=neutral, PublicKeyToken=b2cf3dc44105319d
// MVID: C7382385-AD81-415C-84F8-E0CBBE666E17
// Assembly location: D:\Projects\fga_source\paddle\Windows-Framework-master\PaddleSDK.dll

using System;

namespace PaddleSDK
{
  /// <summary>Wrapper around exceptions thrown by the Paddle SDK.</summary>
  public class PaddleException : ApplicationException
  {
    /// <summary>Creates a new generic PaddleException.</summary>
    public PaddleException()
    {
    }

    /// <summary>
    /// Creates a PaddleException with the given string message.
    /// </summary>
    public PaddleException(string message)
      : base(message)
    {
    }

    /// <summary>Creates a PaddleException wrapping another exception.</summary>
    public PaddleException(string message, Exception innerException)
      : base(message, innerException)
    {
    }
  }
}
