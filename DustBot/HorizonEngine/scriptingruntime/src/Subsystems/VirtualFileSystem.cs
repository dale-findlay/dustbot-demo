using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace HorizonEngine.Subsystems
{
    public static class VirtualFileSystem
    {
        /// <summary>
        /// Resolves the path to a file via the VFS.
        /// </summary>
        /// <param name="virtualPath"></param>
        /// <returns></returns>
        public static string ResolvePath(string virtualPath)
        {
            return _ResolveFilePath(virtualPath);
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern string _ResolveFilePath(string virtualPath);
    }
}
