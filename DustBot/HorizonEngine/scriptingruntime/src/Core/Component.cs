using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using HorizonEngine.Structures;

namespace HorizonEngine.Core
{
    public class Component : IDisposable
    {
        public bool m_dispose = false;
        protected List<ComponentAttribute> m_attributes = new List<ComponentAttribute>();
        public List<ComponentAttribute> Attributes
        {
            get
            {
                return m_attributes;
            }
        }

        public void Dispose()
        {
            if (!m_dispose)
            {
                foreach (ComponentAttribute attribute in m_attributes)
                {
                    Marshal.FreeHGlobal(attribute.name);
                    Marshal.FreeHGlobal(attribute.value);
                }

                m_dispose = true;

                GC.SuppressFinalize(this);
            }
        }
    }
}