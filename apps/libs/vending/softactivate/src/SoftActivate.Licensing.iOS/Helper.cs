using System;
using UIKit; 

namespace SoftActivate.Licensing
{
    public class KeyHelper
    {
        public static string GetCurrentHardwareId()
        {
            return UIDevice.CurrentDevice.IdentifierForVendor.ToString();
        }

        public static bool MatchCurrentHardwareId(string hwid)
        {
            return UIDevice.CurrentDevice.IdentifierForVendor.ToString() == hwid;
        }

        public static bool DetectClockManipulation(DateTime thresholdTime)
        {
            return false;
        }
    }
}
