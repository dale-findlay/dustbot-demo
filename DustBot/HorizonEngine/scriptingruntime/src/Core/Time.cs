using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace HorizonEngine.Core
{
    public static class Time
    {
        public static float DeltaTime
        {
            get
            {
                return _CalculateDeltaTime();
            }
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern float _CalculateDeltaTime();
    }
}
