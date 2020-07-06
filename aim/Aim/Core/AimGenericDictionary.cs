using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Aim.Core
{
    public class AimGenericDictionary<TKey>
    {
        private Dictionary<TKey, object> m_dictionary = new Dictionary<TKey, object>();
        public T GetValue<T>(TKey key)
            where T : class
        {
            return m_dictionary[key] as T;
        }

        public void Add<T>(TKey key, T value)
            where T : class
        {
            m_dictionary.Add(key, value);
        }

        /// <summary>
        /// Returns an array of all of the value entries in the dictionary
        /// </summary>
        /// <returns></returns>
        public List<T> GetValueArray<T>()
            where T : class
        {
            List<T> values = new List<T>();

            foreach (TKey key in m_dictionary.Keys)
            {
                values.Add(GetValue<T>(key));
            }

            return values;
        }
    }
}
