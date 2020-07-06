using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Aim.Exceptions
{
    /// <summary>
    /// Wraps the traditional .NET IOException for logging purposes.
    /// </summary>
    public class AimFileIOException : Exception
    {
        public AimFileIOException(string filePath, Exception innerException) 
            : base($"AimFileIOException file:{filePath} - {innerException.Message}", innerException)
        {}
    }
}
