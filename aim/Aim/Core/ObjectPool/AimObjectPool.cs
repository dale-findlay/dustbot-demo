using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Aim.Core
{
    /// <summary>
    /// A typical implementation of an object pool. Typically used for agents and other largely 
    /// </summary>
    /// <typeparam name="T">Must Inherit from AimPoolableObject</typeparam>
    public class AimObjectPool
    {
        public List<object> objectPool = new List<object>();

        public void Init<T>(uint size)
            where T: AimPoolableObject, new()
        {
            objectPool = new List<object>((int)size);

            for(int i = 0; i <(int)size; ++i)
            {
                T poolObject = new T();
                
                objectPool.Add(poolObject);
            }
        }

        /// <summary>
        /// Retrieve the first inactive object in the pool. returns null
        /// </summary>
        /// <returns></returns>
        public object NewObject<T>()
            where T : AimPoolableObject, new()
        {
            for (int i = 0; i < objectPool.Count; ++i)
            {
                if (!((T)objectPool[i]).ActiveInPool)
                {
                    ((T)objectPool[i]).ActiveInPool = true;
                    return objectPool[i];
                }
            }
            return default(T);
        }

    }
}
