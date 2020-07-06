using Aim.Core;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace Aim.Blackboards
{
    public class AimBlackboardManager : AimSingleton<AimBlackboardManager>
    {
        private Dictionary<Type, AimGenericDictionary<string>> m_instanceSyncedKeys = new Dictionary<Type, AimGenericDictionary<string>>();
        
        public bool Init()
        {
            return true;
        }

        public List<AimBlackboardKey<T>> GetInstanceSyncedKeys<T>()
        {
            List<AimBlackboardKey<T>> list = new List<AimBlackboardKey<T>>();

            if(m_instanceSyncedKeys.ContainsKey(typeof(T))) 
            {
                list = m_instanceSyncedKeys[typeof(T)].GetValueArray<AimBlackboardKey<T>>();
            }

            return list;
        }

        public void Shutdown()
        { }

        public AimBlackboardKey<T> GetInstanceSyncedKey<T>(string name)
        {
            if (m_instanceSyncedKeys.ContainsKey(typeof(T)))
            {
                return m_instanceSyncedKeys[typeof(T)].GetValue<AimBlackboardKey<T>>(name);
            }

            return null;
        }

        public void AddInstanceSyncedKey<T>(AimBlackboardKey<T> key)
        {
            if (!m_instanceSyncedKeys.ContainsKey(typeof(T)))
            {
                m_instanceSyncedKeys[typeof(T)] = new AimGenericDictionary<string>();
            }

            m_instanceSyncedKeys[typeof(T)].Add<AimBlackboardKey<T>>(key.KeyName, key);
        }

        public void SetInstanceSyncedKey<T>(string keyName, object newValue)
        {
            var key = m_instanceSyncedKeys[typeof(T)].GetValue<AimBlackboardKey<T>>(keyName);

            MethodInfo dynMethod = this.GetType().GetMethod("SetLocalKeyValue", BindingFlags.NonPublic | BindingFlags.Instance);
            dynMethod.Invoke(key, new object[] { keyName, newValue });
        }
    }
}
