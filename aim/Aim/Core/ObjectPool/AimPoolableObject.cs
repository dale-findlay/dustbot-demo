using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Aim.Core
{
    /// <summary>
    /// A structure to encapsulate required fields by <see cref="AimObjectPool{T}"/>.
    /// </summary>
    public abstract class AimPoolableObject
    {
        //Is the object active within its object pool.
        private bool m_activeInPool = false;
        public bool ActiveInPool
        {
            get
            {
                return m_activeInPool;
            }
            set
            {
                m_activeInPool = value;
            }
        }
    }
}
