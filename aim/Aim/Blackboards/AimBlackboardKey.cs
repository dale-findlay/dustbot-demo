using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Aim.Blackboards
{
    public class AimBlackboardKey<T>
    {
        private readonly bool m_instanceSynced;
        private readonly string m_name;
        private readonly Type m_type;
        private object m_value;

        public AimBlackboardKey(string name, Type type, object value, bool instanceSynced = false)
        {
            m_type = type;
            KeyValue = value;
            m_instanceSynced = instanceSynced;
            m_name = name;

            if (m_instanceSynced)
            {
                AimBlackboardManager.Instance.AddInstanceSyncedKey<T>(this);
            }
        }

        /// <summary>
        /// The name of the key.
        /// </summary>
        public string KeyName
        {
            get
            {
                return m_name;
            }
        }

        /// <summary>
        /// The value of the key.
        /// </summary>
        public object KeyValue
        {
            get
            {
                if (m_instanceSynced)
                {
                    return AimBlackboardManager.Instance.GetInstanceSyncedKey<T>(KeyName).GetLocalKeyValue();
                }

                return m_value;
            }
            set
            {
                object newValue = value;

                if (newValue.GetType() != m_type)
                {
                    throw new Exception($"[Aim] Cannot set blackboard key value on Blackboard name:{m_name} type: {m_type.Name} new value {value}");
                }

                if (m_instanceSynced)
                {
                    AimBlackboardManager.Instance.SetInstanceSyncedKey<T>(KeyName, newValue);
                }
                else
                {
                    m_value = newValue;
                }
            }
        }

        private object GetLocalKeyValue()
        {
            return m_value;
        }

        /// <summary>
        /// The type of the key.
        /// </summary>
        public Type KeyType
        {
            get
            {
                return m_type;
            }
        }

        /// <summary>
        /// This method sets the value of the key invariably. i.e. no type checking is performed.
        /// </summary>
        /// <param name="name"></param>
        /// <param name="newValue"></param>
        private void SetLocalKeyValue(string name, object newValue)
        {
            m_value = newValue;
        }
    }
}
