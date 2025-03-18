using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Linq;
using System.ServiceProcess;
using System.Text;
using System.Threading.Tasks;

namespace EmailOrder
{
    public partial class EmailOrderServiceFreeYouTubeDownload : ServiceBase
    {
        private ImapListener listener;

        public EmailOrderServiceFreeYouTubeDownload()
        {
            InitializeComponent();

            //EventLog.Delete("EmailOrder.Log");


            if (!System.Diagnostics.EventLog.SourceExists("EmailOrderSource"))
            {
                System.Diagnostics.EventLog.CreateEventSource(
                    "EmailOrderSource", "EmailOrder Logs");
            }
            eventLog.Source = "EmailOrderSource";
            eventLog.Log = "EmailOrder Logs";

            LogHelper.logger += WriteEventLog;
            listener = new ImapListener();
        }

        protected override void OnStart(string[] args)
        {
            LogHelper.WriteLog(LogType.Information, "Email Order Service started.");
            listener.Start();
        }

        protected override void OnStop()
        {
            listener.Stop();
            LogHelper.WriteLog(LogType.Information, "Email Order Service stopped.");
        }

        protected void WriteEventLog(LogType t, string m)
        {
            EventLogEntryType type = EventLogEntryType.Information;

            switch(t)
            {
                case LogType.Error:
                    type = EventLogEntryType.Error;
                    break;
                case LogType.Warning:
                    type = EventLogEntryType.Warning;
                    break;
                case LogType.Information:
                    type = EventLogEntryType.Information;
                    break;
            }
            
            eventLog.WriteEntry(m, type);
        }
    }
}
