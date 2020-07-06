using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HorizonEngine.Utilities
{
    public static class MathUtil
    {
        public static float Clamp(float f, float min, float max)
        {
            if(f > max)
            {
                f = max;
            }

            if(f < min)
            {
                f = min;
            }

            return f;
        }
    }
}
