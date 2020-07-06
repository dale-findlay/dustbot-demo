using System;
using System.Runtime.InteropServices;

namespace HorizonEngine.Structures
{
    [StructLayout(LayoutKind.Sequential)]
    public struct ComponentAttribute
    {
        public ComponentAttribute(string name, string value)
        {
            this.name = Marshal.StringToHGlobalAnsi(name);
            this.value = Marshal.StringToHGlobalAnsi(value);
        }

        public IntPtr name;

        public IntPtr value;
    }
}