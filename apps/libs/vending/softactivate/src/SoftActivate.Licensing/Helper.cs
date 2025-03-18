using System;

namespace SoftActivate.Licensing
{
    public class KeyHelper
    {
        public static string GetCurrentHardwareId()
        {
            throw new NotImplementedException("HWID not implemented in portable library. Please use the platform-specific library");
        }

        public static bool MatchCurrentHardwareId(string hwid)
        {
            throw new NotImplementedException("HWID not implemented in portable library. Please use the platform-specific library");
        }

        public static bool DetectClockManipulation(DateTime thresholdTime)
        {
            return false;
        }
    }
}
