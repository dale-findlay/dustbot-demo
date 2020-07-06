using Aim.Structures;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Aim.Core
{
    public class AimLogger : AimSingleton<AimLogger>
    {
        public AimLogger() : base()
        {}

        public void Init()
        {}

        private void Log(string message, LogSeverity logSeverity)
        {
            DateTime now = DateTime.Now;

            //Log Format: '{Date Time} [{Severity}] {Message}'
            Console.WriteLine($"{now.ToString("G")} [{Enum.GetName(typeof(LogSeverity), logSeverity)}] {message}");
        }

        public void LogInfo(string message)
        {
            Log(message, LogSeverity.Information);
        }

        public void LogWarning(string message)
        {
            Log(message, LogSeverity.Warning);
        }

        public void LogError(string message)
        {
            Log(message, LogSeverity.Error);
        }

        public void LogError(Exception e)
        {
            Log($"{e.Message} StackTrace {e.StackTrace}", LogSeverity.Error);
        }

    }
}
