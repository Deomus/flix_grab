using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;



namespace EmailOrder
{
    class EmailOrderConsole
    {
        
        public static void Main(string[] args)
        {
            LogHelper.logger += ConsoleLog;
            //EventHandler<string>
            var listener = new ImapListener();
            
            Console.WriteLine("Hit any key to end the IDLE loop.");
            listener.Start();
            Console.ReadKey();
            listener.Stop();
            listener.AwaitStop();
                        
        }

        private static void ConsoleLog(LogType t, string m)
        {
            Console.WriteLine(@"[{0}] {1}", t.ToString(), m);
        }

        
    }
}
