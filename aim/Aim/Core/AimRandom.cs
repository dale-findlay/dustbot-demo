using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;

namespace Aim.Core
{
    public static class AimRandom
    {
        private static RandomNumberGenerator r = null;

        public static RandomNumberGenerator GetInstance()
        {
            if (r == null)
            {
                r = RandomNumberGenerator.Create();
            }

            return r;
        }

        public static double NextDouble()
        {
            byte[] b = new byte[4];
            GetInstance().GetBytes(b);
            return (double)BitConverter.ToUInt32(b, 0) / UInt32.MaxValue;
        }

        public static int Next(int minValue, int maxValue)
        {
            int i = (int)Math.Round(NextDouble() * (maxValue - minValue - 1.0)) + minValue;
            return i;
        }

        /// <summary>
        /// Generates a random integer between (including) a and b.
        /// </summary>
        /// <param name="a"></param>
        /// <param name="b"></param>
        /// <returns></returns>
        public static int RandomRange(int a, int b)
        {
            int randomInt = Next(a, b);

            return randomInt;
        }

        /// <summary>
        /// Generates a float between (including) a and b.
        /// </summary>
        /// <param name="a"></param>
        /// <param name="b"></param>
        /// <returns></returns>
        public static double RandomRange(double a, double b)
        {
            double range = b - a;

            return NextDouble() * range;
        }

        /// <summary>
        /// Generates a float between (including) a and b.
        /// </summary>
        /// <param name="a"></param>
        /// <param name="b"></param>
        /// <returns></returns>
        public static float RandomRange(float a, float b)
        {
            return (float)RandomRange((double)a, (double)b);
        }
    }
}
