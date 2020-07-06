using GlmNet;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Aim
{
    public static class AimMath
    {
        public static float Lerp(float a, float b, float t)
        {
            return (1 - t) * a + t * b;
        }

        public static vec3 Lerp(vec3 a, vec3 b, float t)
        {
            float x = Lerp(a.x, b.x, t);
            float y = Lerp(a.y, b.y, t);
            float z = Lerp(a.z, b.z, t);

            return new vec3(x, y, z);
        }
    }
}
