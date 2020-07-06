using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using GlmNet;
using HorizonEngine.Subsystems;

namespace HorizonEngine.Core
{
    public abstract class Script
    {
#pragma warning disable CS0649 //unassigned error (the value of m_scriptId is assigned by the ScriptingRuntime in the engine).
        private uint m_scriptId;
#pragma warning restore CS0649

        public GameObject gameObject;
        public Script(uint gameObjectId)
        {
            gameObject = new GameObject(gameObjectId);

            ScriptManager.TrackScript(this);
        }

        /// <summary>
        /// Returns the value of the custom value on a given script.
        /// </summary>
        /// <param name="name"></param>
        /// <returns></returns>
        protected string GetScriptCustomAttribute(string name)
        {
            return _GetCustomAttributeValue(m_scriptId, name);
        }

        public GameObject Find(string gameObjectName)
        {
            uint gameObjectId = _FindGameObject(m_scriptId, gameObjectName);

            if(gameObjectId == 0)
            {
                return null;
            }
            
            return new GameObject(gameObjectId);
        }

        public abstract void Start();
        public abstract void Update();


        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private static extern string _GetCustomAttributeValue(uint scriptId, string name);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private static extern uint _FindGameObject(uint scriptId, string name);
    }
}
