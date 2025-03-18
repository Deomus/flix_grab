using System;

namespace SoftActivate.Licensing
{
    internal class NTPClient
    {
        internal static DateTime GetCurrentTimeUTC(int timeout = -1)
        {
            throw new NotImplementedException("Please only use TIME_VALIDATION_METHOD.UseLocalTime on this platform");
        }
    }
}
