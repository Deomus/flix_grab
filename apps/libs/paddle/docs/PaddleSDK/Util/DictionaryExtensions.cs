// Decompiled with JetBrains decompiler
// Type: PaddleSDK.Util.DictionaryExtensions
// Assembly: PaddleSDK, Version=1.0.0.0, Culture=neutral, PublicKeyToken=b2cf3dc44105319d
// MVID: C7382385-AD81-415C-84F8-E0CBBE666E17
// Assembly location: D:\Projects\fga_source\paddle\Windows-Framework-master\PaddleSDK.dll

using System.Collections.Generic;

namespace PaddleSDK.Util
{
  internal static class DictionaryExtensions
  {
    /// <summary>
    /// Returns the value from the dictionary or null if not present (means you can't test for null values being explicity set via this method).
    /// </summary>
    public static TV GetValueOrNull<TK, TV>(this IDictionary<TK, TV> dict, TK key, TV defaultValue)
    {
      TV v;
      if (!dict.TryGetValue(key, out v))
        return defaultValue;
      return v;
    }
  }
}
