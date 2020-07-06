using GlmNet;
using HorizonEngine.Structures;
using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace HorizonEngine.Core
{
    public class GameObject
    {
        private  uint m_gameObjectId;
        private Transform m_transform;
        public uint Id
        {
            get
            {
                return m_gameObjectId;
            }
        }
        public Transform transform
        {
            get
            {
                return m_transform;
            }
            set
            {
                m_transform.SetGameObjectId(m_gameObjectId);
            }
        }

        public string Name
        {
            get
            {
                return _GetName(m_gameObjectId);
            }
        }
        
        public bool Visible
        {
            get
            {
                return _GetVisible(m_gameObjectId);
            }
            set
            {
                _SetVisible(m_gameObjectId, value);
            }
        }

        public GameObject(string name="New GameObject")
        {
            m_gameObjectId = _CreateGameObject(name);
            this.m_transform = new Transform();
            this.m_transform.SetGameObjectId(m_gameObjectId);
        }

        public GameObject(uint gameObjectId)
        {
            this.m_gameObjectId = gameObjectId;
            this.m_transform = new Transform();
            this.m_transform.SetGameObjectId(m_gameObjectId);
        }

        public void AddComponent<T>(T component)  where T : Component
        {
            Type type = component.GetType();
            var attributes = component.Attributes.ToArray();
            _AddComponent(m_gameObjectId, type.Name, attributes);
        }

        public void AddScript(string scriptName)
        {
            _AddScript(m_gameObjectId, scriptName);
        }
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private static extern uint _CreateGameObject(string name);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private static extern void _AddComponent(uint gameObjectId, string componentName, ComponentAttribute[] atrributes);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private static extern void _AddScript(uint gameObjectId, string scriptName);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private static extern string _GetName(uint gameObjectId);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private static extern bool _GetVisible(uint gameObjectId);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private static extern void _SetVisible(uint gameObjectId, bool newVisible);

    }
}
