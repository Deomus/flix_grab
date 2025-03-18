using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using EmailOrder.Properties;

using MailKit.Net.Imap;
using MailKit;
using MailKit.Search;
using System.Threading;
using System.IO;
using System.Net.Sockets;
using MailKit.Security;

namespace EmailOrder
{
    class ImapListener
    {        
        const string OrderCompleted = @"Order Completed";
        const string OrderRejected = @"Order Rejected";
        const string OrderNotFound = @"Order Not Found";

        private ImapClient client;
        private Thread thread;
        
        private DateTime check_date = Settings.Default.DeliveredAfter;

        readonly object mutex = new object();
        private CancellationTokenSource timeout_token;
        private CancellationTokenSource cancel_token;

        private const int TRUE_VALUE = 1;
        private const int FALSE_VALUE = 0;
        private volatile int is_process_messages = TRUE_VALUE;

        //System.IO.StreamWriter file;

        public bool Start()
        {
            LogHelper.WriteLog(LogType.Information, "Starting service...");
            lock (mutex)
            {
                try
                {
                    //file = new System.IO.StreamWriter(@"../../test/processed_mails.csv");
                    //file.WriteLine(Order.ToColumnNames());

                    cancel_token = new CancellationTokenSource();
                    
                    thread = new Thread(Run);
                    //Start threading;
                    thread.Start();
                }
                catch (Exception e)
                {
                    LogHelper.WriteLog(LogType.Error, "Exception {0}", e.ToString());
                    return false;
                }
            }
            

            return true;

        }

        public void Stop()
        {
            LogHelper.WriteLog(LogType.Information, "Stopping service...");
            lock (mutex)
            {
                //if (file != null)
                //    file.Close();

                if (cancel_token != null)
                    cancel_token.Cancel();

                if (timeout_token != null)
                    timeout_token.Cancel();
            }

           

        }

        public void AwaitStop()
        {
            if (thread != null)
                thread.Join();

            cancel_token.Dispose();
            cancel_token = null;
            thread = null;
        }

        private void RemoveLabels()
        {
            var search_label = SearchQuery.HasGMailLabel(OrderCompleted).Or(SearchQuery.HasGMailLabel(OrderRejected)).Or(SearchQuery.HasGMailLabel(OrderNotFound));
            //client.SyncRoot;
            
            var uids = client.Inbox.Search(search_label);
            client.Inbox.RemoveLabels(uids, new List<string> { OrderCompleted, OrderRejected, OrderNotFound }, true);
        }
        
        private void ProcessMessages()
        {
            var search_label = SearchQuery.HasGMailLabel(OrderCompleted).Or(SearchQuery.HasGMailLabel(OrderRejected)).Or(SearchQuery.HasGMailLabel(OrderNotFound));
            var search_query = SearchQuery.Not(search_label).And(SearchQuery.FromContains(Settings.Default.FromAddress)).And(SearchQuery.DeliveredAfter(check_date));
            //client.SyncRoot;
            var uids = client.Inbox.Search(search_query);

            foreach (var uid in uids)
            {
                var message = client.Inbox.GetMessage(uid);
                string text = message.HtmlBody;

                var order = new PaypalOrder(Settings.Default.ProductName, Settings.Default.ProductId);

                if (order.ParseEmail(text, message.Date.LocalDateTime))
                {
                    LogHelper.WriteLog(LogType.Information, order.ToValues());

                    if (Settings.Default.TestMode && Settings.Default.FallbackEmail.Count() > 0)
                    {
                        order.Email = Settings.Default.FallbackEmail;
                    }

                    if (order.Place(Settings.Default.OrderUrl, Settings.Default.SignatureKey))
                    {
                        LogHelper.WriteLog(LogType.Information, "Order completed for email '{0}'", order.Email);
                        client.Inbox.AddLabels(uid, new List<string> { OrderCompleted }, true);
                    }
                    else
                    {
                        LogHelper.WriteLog(LogType.Information, "Order rejected for email '{0}'", order.Email);
                        client.Inbox.AddLabels(uid, new List<string> { OrderRejected }, true);
                    }
              
                    //TODO: remove
                    //if (Settings.Default.TestMode)
                    //{
                    //    try
                    //    {
                    //        file.WriteLine(order.ToValues());
                    //        file.Flush();
                    //    }
                    //    catch (Exception e)
                    //    {
                    //        LogHelper.WriteLog(LogType.Error, "Cant write line to {0}: {1}", file.ToString(), e.ToString());
                    //    }
                    //}
                    
                }
                else
                {
                    LogHelper.WriteLog(LogType.Information, "Order not found");
                    client.Inbox.AddLabels(uid, new List<string> { OrderNotFound }, true);

                    //if (Settings.Default.TestMode)
                    //{
                    //    try
                    //    {
                    //        string file_name = string.Join("_", message.Subject.Split(Path.GetInvalidFileNameChars())) + "_" + uid;
                    //        System.IO.Directory.CreateDirectory(@"../../test/wrong_mail");
                    //        System.IO.File.WriteAllText(@"../../test/wrong_mail/" + file_name + @".eml", text);
                    //    }
                    //    catch (Exception e)
                    //    {
                    //        LogHelper.WriteLog(LogType.Error, "Wrong Filename: {0}, {1}", message.Subject, e.ToString());
                    //    }
                    //}

                    
                }
               

                //if (message.Date.LocalDateTime > check_date)
                //    check_date = message.Date.LocalDateTime;
                
            }
         
        }

        private void Initialize()
        {
            client = new ImapClient(new ProtocolLogger(Console.OpenStandardError()));

            client.Connect(Settings.Default.ImapHost, Settings.Default.ImapPort, true, cancel_token.Token);

            if (client.Capabilities.HasFlag(ImapCapabilities.Id))
            {
                var clientImplementation = new ImapImplementation { Name = "MailKit", Version = "1.0" };
                var serverImplementation = client.Identify(clientImplementation);

                LogHelper.WriteLog(LogType.Information, "Server implementation details:");

                foreach (var property in serverImplementation.Properties)
                    LogHelper.WriteLog(LogType.Information, "  {0} = {1}", property.Key, property.Value);
            }
            client.Authenticate(Settings.Default.EmailName, Settings.Default.EmailPassword);

            client.Inbox.Open(FolderAccess.ReadWrite);
            //client.Inbox.Open(FolderAccess.ReadOnly);
                        
            // Keep track of changes to the number of messages in the folder (this is how we'll tell if new messages have arrived).
            client.Inbox.CountChanged += (sender, e) =>
            {
                // Note: the CountChanged event will fire when new messages arrive in the folder and/or when messages are expunged.
                var folder = (ImapFolder)sender;

                //LogHelper.WriteLog(LogType.Information, "The number of messages in {0} has changed.", folder);
                is_process_messages = TRUE_VALUE;
                lock (mutex)
                {
                    if (timeout_token != null)
                        timeout_token.Cancel();
                }

            };
            is_process_messages = TRUE_VALUE;
        }

        private void Deinitialize()
        {
            LogHelper.WriteLog(LogType.Information, "Disconnect IMAP");
            client.Disconnect(true);
            //client.Dispose();
            //client = null;
        }

        private void RunProcess()
        {
            // Note: since the IMAP server will drop the connection after 30 minutes, we must loop sending IDLE commands that
            // last ~29 minutes or until the user has requested that they do not want to IDLE anymore.
            //
            // For GMail, we use a 9 minute interval because they do not seem to keep the connection alive for more than ~10 minutes.
            while (!cancel_token.IsCancellationRequested)
            {
                lock (mutex)
                {
                    timeout_token = new CancellationTokenSource();
                }

                if (Interlocked.CompareExchange(ref is_process_messages, FALSE_VALUE, TRUE_VALUE) == TRUE_VALUE)
                {
                    ProcessMessages();
                }


                using (var timer = new System.Timers.Timer(9 * 60 * 1000))
                {
                    // End the IDLE command when the timer expires.
                    timer.Elapsed += (sender, e) => timeout_token.Cancel();
                    timer.AutoReset = false;
                    timer.Enabled = true;

                    if (client.Capabilities.HasFlag(ImapCapabilities.Idle))
                    {
                        // The Idle() method will not return until the timeout has elapsed or idle.CancellationToken is cancelled
                        //client.Idle(timeout_token.Token, cancel_token.Token);
                        client.Idle(timeout_token.Token);
                    }
                    else
                    {
                        // The IMAP server does not support IDLE, so send a NOOP command instead
                        client.NoOp(cancel_token.Token);

                        // Wait for the timeout to elapse or the cancellation token to be cancelled
                        WaitHandle.WaitAny(new[] { timeout_token.Token.WaitHandle, cancel_token.Token.WaitHandle });
                    }
            
                }

                lock (mutex)
                {
                    timeout_token.Dispose();
                    timeout_token = null;
                }
            }
        }

        private void Run()
        {
            while (!cancel_token.IsCancellationRequested)
            {
                try
                {
                    Initialize();
                    RunProcess();
                    if (Settings.Default.TestMode)
                        RemoveLabels();

                    Deinitialize();

                }
                catch (ArgumentException e)
                {
                    LogHelper.WriteLog(LogType.Error, "General Error Argument Exception: {0}", e.ToString());
                }
                catch (ObjectDisposedException e)
                {
                    LogHelper.WriteLog(LogType.Error, "General Error Object Disposed: {0}", e.ToString());
                }
                catch (AuthenticationException e)
                {
                    LogHelper.WriteLog(LogType.Error, "Authentication Exception: {0}", e.ToString());

                }
                catch (ServiceNotAuthenticatedException e)
                {
                    LogHelper.WriteLog(LogType.Error, "Service not Authenticated: {0}", e.ToString());
                }
                catch (ServiceNotConnectedException e)
                {
                    LogHelper.WriteLog(LogType.Error, "Service not Connected or connection Lost: {0}", e.ToString());
                }
                catch (InvalidOperationException e)
                {
                    LogHelper.WriteLog(LogType.Error, "Invalid Operation Exception: {0}", e.ToString());

                }
                catch (NotSupportedException e)
                {
                    LogHelper.WriteLog(LogType.Error, "Operation Not Supported Exception: {0}", e.ToString());

                }
                catch (OperationCanceledException e)
                {
                    // This means that idle.CancellationToken was cancelled, not the DoneToken nor the timeout.
                    LogHelper.WriteLog(LogType.Error, "Operation Cancelled: {0}", e.ToString());

                }
                catch (ImapProtocolException e)
                {
                    // The IMAP server sent garbage in a response and the ImapClient was unable to deal with it.
                    // This should never happen in practice, but it's probably still a good idea to handle it.
                    //
                    // Note: an ImapProtocolException almost always results in the ImapClient getting disconnected.
                    LogHelper.WriteLog(LogType.Error, "IMAP Protocol Exception: {0}", e.ToString());
                    //break;
                }
                catch (ImapCommandException e)
                {
                    // The IMAP server responded with "NO" or "BAD" to either the IDLE command or the NOOP command.
                    // This should never happen... but again, we're catching it for the sake of completeness.
                    LogHelper.WriteLog(LogType.Error, "IMAP Command Exception: {0}", e.ToString());
                    //break;
                }
                catch (IOException e)
                {
                    LogHelper.WriteLog(LogType.Error, "IO Exception: {0}", e.ToString());
                }
                catch (SocketException e)
                {
                    LogHelper.WriteLog(LogType.Error, "Socket Exception: {0}", e.ToString());
                }
                catch (Exception e)
                {
                    LogHelper.WriteLog(LogType.Error, "Unknown Exception: {0}", e.ToString());
                }
                finally
                {
                    // We're about to Dispose() the timeout source, so set it to null.
                    LogHelper.WriteLog(LogType.Information, "Finalizing IMAP");

                    //if (client != null)
                    //    client.Dispose();

                    if (!cancel_token.IsCancellationRequested)
                    {
                        LogHelper.WriteLog(LogType.Information, "Sleeping before reconnection...");
                        //Reconnection, Sleep some time;
                        cancel_token.Token.WaitHandle.WaitOne(60 * 1000);

                    }

                }
            }
        }
    }
}
