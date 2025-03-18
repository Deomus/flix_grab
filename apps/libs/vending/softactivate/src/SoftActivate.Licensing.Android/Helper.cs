using Android.OS;
using System; 

namespace SoftActivate.Licensing
{
    public class KeyHelper
    {
        public static string GetCurrentHardwareId()
        {
            return Build.Serial;
        }

        public static bool MatchCurrentHardwareId(string hwid)
        {
            return Build.Serial == hwid;
        }

        public static bool DetectClockManipulation(DateTime thresholdTime)
        {
            return false;
        }
    }
}
