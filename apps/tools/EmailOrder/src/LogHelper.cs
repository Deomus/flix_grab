using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace EmailOrder
{
    public enum LogType
    {
        Error = 1,
        Warning = 2,
        Information = 4,
    }

    public delegate void Logger(LogType t, string m);

    public static class LogHelper
    {
        static public event Logger logger;

        public static void WriteLog(LogType type, string message)
        {
            if (logger != null)
                logger(type, message);
        }

        public static void WriteLog(LogType type, string format, object arg0)
        {
            WriteLog(type, string.Format(format, arg0));
        }
        public static void WriteLog(LogType type, string format, object arg0, object arg1)
        {
            WriteLog(type, string.Format(format, arg0, arg1));
        }

        public static void WriteLog(LogType type, string format, object arg0, object arg1, object arg2)
        {
            WriteLog(type, string.Format(format, arg0, arg1, arg2));
        }
       
               
    }
}
