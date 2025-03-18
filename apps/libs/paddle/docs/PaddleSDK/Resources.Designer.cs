// Decompiled with JetBrains decompiler
// Type: PaddleSDK.Resources
// Assembly: PaddleSDK, Version=1.0.0.0, Culture=neutral, PublicKeyToken=b2cf3dc44105319d
// MVID: C7382385-AD81-415C-84F8-E0CBBE666E17
// Assembly location: D:\Projects\fga_source\paddle\Windows-Framework-master\PaddleSDK.dll

using System.CodeDom.Compiler;
using System.ComponentModel;
using System.Diagnostics;
using System.Globalization;
using System.Resources;
using System.Runtime.CompilerServices;

namespace PaddleSDK
{
  /// <summary>
  ///   A strongly-typed resource class, for looking up localized strings, etc.
  /// </summary>
  [DebuggerNonUserCode]
  [GeneratedCode("System.Resources.Tools.StronglyTypedResourceBuilder", "4.0.0.0")]
  [CompilerGenerated]
  internal class Resources
  {
    private static ResourceManager resourceMan;
    private static CultureInfo resourceCulture;

    /// <summary>
    ///   Returns the cached ResourceManager instance used by this class.
    /// </summary>
    [EditorBrowsable(EditorBrowsableState.Advanced)]
    internal static ResourceManager ResourceManager
    {
      get
      {
        if (object.ReferenceEquals((object) PaddleSDK.Resources.resourceMan, (object) null))
          PaddleSDK.Resources.resourceMan = new ResourceManager("PaddleSDK.Resources", typeof (PaddleSDK.Resources).Assembly);
        return PaddleSDK.Resources.resourceMan;
      }
    }

    /// <summary>
    ///   Overrides the current thread's CurrentUICulture property for all
    ///   resource lookups using this strongly typed resource class.
    /// </summary>
    [EditorBrowsable(EditorBrowsableState.Advanced)]
    internal static CultureInfo Culture
    {
      get
      {
        return PaddleSDK.Resources.resourceCulture;
      }
      set
      {
        PaddleSDK.Resources.resourceCulture = value;
      }
    }

    /// <summary>
    ///    Looks up a localized string similar to Sorry. We were unable to verify your license information.
    /// 
    /// {0}.
    ///  </summary>
    internal static string VerifyFailDialogBody
    {
      get
      {
        return PaddleSDK.Resources.ResourceManager.GetString("VerifyFailDialogBody", PaddleSDK.Resources.resourceCulture);
      }
    }

    /// <summary>Looks up a localized string similar to Uh oh!.</summary>
    internal static string VerifyFailDialogTitle
    {
      get
      {
        return PaddleSDK.Resources.ResourceManager.GetString("VerifyFailDialogTitle", PaddleSDK.Resources.resourceCulture);
      }
    }

    /// <summary>
    ///   Looks up a localized string similar to Thank you. We have activated your license..
    /// </summary>
    internal static string VerifySucceedDialogBody
    {
      get
      {
        return PaddleSDK.Resources.ResourceManager.GetString("VerifySucceedDialogBody", PaddleSDK.Resources.resourceCulture);
      }
    }

    /// <summary>
    ///   Looks up a localized string similar to License Activated.
    /// </summary>
    internal static string VerifySucceedDialogTitle
    {
      get
      {
        return PaddleSDK.Resources.ResourceManager.GetString("VerifySucceedDialogTitle", PaddleSDK.Resources.resourceCulture);
      }
    }

    internal Resources()
    {
    }
  }
}
