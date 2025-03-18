using System;
using System.Diagnostics;
using System.IO;
using System.Net;
using System.Net.NetworkInformation;
using System.Net.Sockets;
using System.Threading;

namespace SoftActivate.Licensing
{
    internal class NTPClient
    {
        // List of NIST time servers
        // http://tf.nist.gov/tf-cgi/servers.cgi
        private readonly static string[] NistServers = new[] {
			"nist1-nj.ustiming.org",
			"nist1-nj2.ustiming.org",
			"nist1-ny2.ustiming.org",
			"nist1-pa.ustiming.org",
			"time-a.nist.gov",
			"time-b.nist.gov",
			"time-d.nist.gov",
			"nist1.aol-va.symmetricom.com",
			"nist1-atl.ustiming.org",
			"wolfnisttime.com",
			"nist1-chi.ustiming.org",
			"nist.expertsmi.com",
			"nist.netservicesgroup.com",
			"nisttime.carsoncity.k12.mi.us",
			"wwv.nist.gov",
			"time-nist.symmetricom.com",
			"time-a.timefreq.bldrdoc.gov",
			"time-b.timefreq.bldrdoc.gov",
			"time-c.timefreq.bldrdoc.gov",
			"time.nist.gov",
			"utcnist.colorado.edu",
			"utcnist2.colorado.edu",
			"ntp-nist.ldsbc.edu",
			"nist1-lv.ustiming.org",
			"nist-time-server.eoni.com",
			"nist1.symmetricom.com"
		};

        private readonly static string[] NtpServers = new[] {
			"time.windows.com",
			"ntp.nic.cz",
			"ptbtime2.ptb.de",
			"time-a.nist.gov",
			"ntp0.as34288.net"
		};

        internal static DateTime GetCurrentTimeUTC(int timeout = -1)
        {
            if (!IsNetworkAvailable())
                throw new Exception("Unable to query Internet time (network not available)");

            var serversCount = NistServers.Length + NtpServers.Length;
            var waitHandles = new WaitHandle[serversCount];
            var timeServerQueries = new ITimeServerQuery[serversCount];

            var i = -1;

            foreach (var nistServer in NistServers)
            {
                var manualResetEvent = new ManualResetEvent(false);
                waitHandles[++i] = manualResetEvent;
                var nistTimeServerQuery = new NistTimeServerQuery(nistServer, timeout, manualResetEvent);
                timeServerQueries[i] = nistTimeServerQuery;
                ThreadPool.QueueUserWorkItem(nistTimeServerQuery.ThreadPoolCallback, i);
            }

            foreach (var ntpServer in NtpServers)
            {
                var manualResetEvent = new ManualResetEvent(false);
                waitHandles[++i] = manualResetEvent;
                var nptTimeServerQuery = new NtpTimeServerQuery(ntpServer, timeout, manualResetEvent);
                timeServerQueries[i] = nptTimeServerQuery;
                ThreadPool.QueueUserWorkItem(nptTimeServerQuery.ThreadPoolCallback, i);
            }

            // Wait any successfull response...
            if (WaitHandle.WaitAny(waitHandles, timeout) == WaitHandle.WaitTimeout)
                throw new Exception("Unable to query Internet time (timeout)");

            // Display the results...
            for (var j = 0; j < serversCount; j++)
            {
                var query = timeServerQueries[j];
                if (query.DateTimeResult == TimeServerQuery.NoDate) 
                    continue;
                
                Trace.WriteLine("And the winner is... " + query.TimeServer);
                
                return query.DateTimeResult;
            }

            throw new Exception("Unable to query Internet time");
        }

        /// <summary>
        /// Indicates whether any network connection is available
        /// Filter connections below a specified speed, as well as virtual network cards.
        /// </summary>
        /// <returns>
        ///     <c>true</c> if a network connection is available; otherwise, <c>false</c>.
        /// </returns>
        private static bool IsNetworkAvailable()
        {
            return IsNetworkAvailable(0);
        }

        /// <summary>
        /// Indicates whether any network connection is available.
        /// Filter connections below a specified speed, as well as virtual network cards.
        /// </summary>
        /// <param name="minimumSpeed">The minimum speed required. Passing 0 will not filter connection using speed.</param>
        /// <returns>
        ///     <c>true</c> if a network connection is available; otherwise, <c>false</c>.
        /// </returns>
        private static bool IsNetworkAvailable(long minimumSpeed)
        {
            if (!NetworkInterface.GetIsNetworkAvailable())
                return false;

            foreach (NetworkInterface ni in NetworkInterface.GetAllNetworkInterfaces())
            {
                // discard because of standard reasons
                if ((ni.OperationalStatus != OperationalStatus.Up) ||
                    (ni.NetworkInterfaceType == NetworkInterfaceType.Loopback) ||
                    (ni.NetworkInterfaceType == NetworkInterfaceType.Tunnel))
                    continue;

                // this allow to filter modems, serial, etc.
                // I use 10000000 as a minimum speed for most cases
                if (ni.Speed < minimumSpeed)
                    continue;

                // discard virtual cards (virtual box, virtual pc, etc.)
                if ((ni.Description.IndexOf("virtual", StringComparison.OrdinalIgnoreCase) >= 0) ||
                    (ni.Name.IndexOf("virtual", StringComparison.OrdinalIgnoreCase) >= 0))
                    continue;

                // discard "Microsoft Loopback Adapter", it will not show as NetworkInterfaceType.Loopback but as Ethernet Card.
                if (ni.Description.Equals("Microsoft Loopback Adapter", StringComparison.OrdinalIgnoreCase))
                    continue;

                return true;
            }

            return false;
        }
    }

    internal interface ITimeServerQuery
    {
        string TimeServer { get; }
        int Timeout { get; }
        ManualResetEvent DoneEvent { get; }
        DateTime DateTimeResult { get; }
        void ThreadPoolCallback(Object threadContext);
    }

    internal abstract class TimeServerQuery : ITimeServerQuery
    {
        public static readonly DateTime NoDate = DateTime.MinValue;

        public string TimeServer { get; private set; }

        public int Timeout { get; private set; }

        public ManualResetEvent DoneEvent { get; private set; }

        public DateTime DateTimeResult { get; private set; }

        protected TimeServerQuery(string timeServer, int timeout, ManualResetEvent doneEvent)
        {
            this.DoneEvent = doneEvent;
            this.Timeout = timeout;
            this.TimeServer = timeServer;
            this.DateTimeResult = DateTime.MinValue;
        }

        // Wrapper method for use with thread pool
        public void ThreadPoolCallback(Object threadContext)
        {
            this.DateTimeResult = GetTime();
            if (this.DateTimeResult == NoDate)
            {
                Thread.Sleep(this.Timeout * 2);
            }

            this.DoneEvent.Set();
        }

        protected abstract DateTime GetTime();
    }

    internal class NistTimeServerQuery : TimeServerQuery
    {
        public NistTimeServerQuery(string timeServer, int timeout, ManualResetEvent doneEvent) :
            base(timeServer, timeout, doneEvent)
        {

        }

        protected override DateTime GetTime()
        {
            try
            {
                // Connect to the server (at port 13) and get the response
                string serverResponse;
                var tcpClient = new TcpClient(this.TimeServer, 13) { ReceiveTimeout = Timeout, SendTimeout = Timeout };
                using (var reader = new StreamReader(tcpClient.GetStream()))
                {
                    serverResponse = reader.ReadToEnd();
                }

                // If a response was received
                if (string.IsNullOrEmpty(serverResponse)) return NoDate;

                // Split the response string ("55596 11-02-14 13:54:11 00 0 0 478.1 UTC(NIST) *")
                var tokens = serverResponse.Split(' ');

                // Check the number of tokens
                if (tokens.Length < 6) return NoDate;

                // Check the health status
                var health = tokens[5];
                if (health != "0") return NoDate;

                // Get date and time parts from the server response
                var dateParts = tokens[1].Split('-');
                var timeParts = tokens[2].Split(':');

                // Create a DateTime instance
                var utcDateTime = new DateTime(
                    Convert.ToInt32(dateParts[0]) + 2000,
                    Convert.ToInt32(dateParts[1]), Convert.ToInt32(dateParts[2]),
                    Convert.ToInt32(timeParts[0]), Convert.ToInt32(timeParts[1]),
                    Convert.ToInt32(timeParts[2]));

                return utcDateTime;
            }
            catch (Exception)
            {
                return NoDate;
            }
        }
    }

    internal class NtpTimeServerQuery : TimeServerQuery
    {
        public NtpTimeServerQuery(string timeServer, int timeout, ManualResetEvent doneEvent) :
            base(timeServer, timeout, doneEvent)
        {

        }

        protected override DateTime GetTime()
        {
            try
            {
                // NTP message size - 16 bytes of the digest (RFC 2030)
                var ntpData = new byte[48];

                //Setting the Leap Indicator, Version Number and Mode values
                ntpData[0] = 0x1B; //LI = 0 (no warning), VN = 3 (IPv4 only), Mode = 3 (Client Mode)

                var addresses = Dns.GetHostEntry(this.TimeServer).AddressList;

                //The UDP port number assigned to NTP is 123
                var ipEndPoint = new IPEndPoint(addresses[0], 123);
                //NTP uses UDP
                var socket = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);

                socket.Connect(ipEndPoint);

                socket.Send(ntpData);
                socket.Receive(ntpData);
                socket.Close();

                //Offset to get to the "Transmit Timestamp" field (time at which the reply 
                //departed the server for the client, in 64-bit timestamp format."
                const byte serverReplyTime = 40;

                //Get the seconds part
                ulong intPart = BitConverter.ToUInt32(ntpData, serverReplyTime);

                //Get the seconds fraction
                ulong fractPart = BitConverter.ToUInt32(ntpData, serverReplyTime + 4);

                //Convert From big-endian to little-endian
                intPart = SwapEndianness(intPart);
                fractPart = SwapEndianness(fractPart);

                var milliseconds = (intPart * 1000) + ((fractPart * 1000) / 0x100000000L);

                //**UTC** time
                var networkDateTime = (new DateTime(1900, 1, 1)).AddMilliseconds((long)milliseconds);

                return networkDateTime;
            }
            catch (Exception ex)
            {
                Trace.WriteLine("Delete this obsolete time server: " + this.TimeServer);
                Trace.WriteLine(ex.Message);
                return NoDate;
            }
        }

        // stackoverflow.com/a/3294698/162671
        private static uint SwapEndianness(ulong x)
        {
            return (uint)(((x & 0x000000ff) << 24) + ((x & 0x0000ff00) << 8) + ((x & 0x00ff0000) >> 8) + ((x & 0xff000000) >> 24));
        }
    }
}
