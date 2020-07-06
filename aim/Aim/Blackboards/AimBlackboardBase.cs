using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Aim.Blackboards
{
    /// <summary>
    /// A blackboard, containing various keys.
    /// </summary>
    public abstract class AimBlackboard<T>
    {
        protected List<AimBlackboardKey<T>> m_keys;
        protected AimBlackboard()
        {
            var instanceSyncedKeys = AimBlackboardManager.Instance.GetInstanceSyncedKeys<T>();
            m_keys = instanceSyncedKeys;
        }
        
        public List<AimBlackboardKey<T>> Keys
        {
            get
            {
                return m_keys;
            }
        }

        public bool ContainsKeyWithName(string name)
        {
            foreach(var key in m_keys)
            {
                if(key.KeyName == name)
                {
                    return true;
                }
            }

            return false;
        }

        public void AddKey(AimBlackboardKey<T> key)
        {
            if(!ContainsKeyWithName(key.KeyName))
            {
                m_keys.Add(key);
            }
        }    
        
        public AimBlackboardKey<T> GetKey(string name)
        {
            foreach (var key in m_keys)
            {
                if (key.KeyName == name)
                {
                    return key;
                }
            }

            return null;
        }

        public void SetKey(string keyName, object value)
        {
            foreach(var key in m_keys)
            {
                if (key.KeyName == keyName)
                {
                    key.KeyValue = value;
                }
            }
        }

    }
}
