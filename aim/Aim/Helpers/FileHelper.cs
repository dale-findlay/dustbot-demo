using Aim.Exceptions;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Aim.Helpers
{
    /// <summary>
    /// A utility class to encapsulate any data transfer between the disk and the library.
    /// </summary>
    public static class FileHelper
    {
        /// <summary>
        /// Returns the entire contents of a file.
        /// </summary>
        /// <param name="filePath"></param>
        /// <exception cref="Aim.Exceptions.AimFileIOException"></exception>
        /// <returns></returns>
        public static string ReadToEnd(string filePath)
        {
            try
            {
                using (StreamReader streamReader = new StreamReader(filePath))
                {
                    string fileContents = streamReader.ReadToEnd();

                    return fileContents;
                }
            }
            catch(Exception e)
            {
                throw new AimFileIOException(filePath, e);
            }
        }
    }
}
