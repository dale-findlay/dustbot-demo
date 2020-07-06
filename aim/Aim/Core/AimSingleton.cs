using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Aim.Core
{
    /// <summary>
    /// This class is used to create a singleton within the aim library.
    /// </summary>
    public abstract class AimSingleton<T>
        where T : class, new()
    {
        private static T m_instance = null;

        public static void CreateInstance()
        {
            m_instance = new T();
        }

        public static T Instance
        {
            get
            {
                if(m_instance == null)
                {
                    Console.WriteLine($"{typeof(T).Name} was not initialized.");
                    return null;
                }

                return m_instance;
            }
        }
    }
}
