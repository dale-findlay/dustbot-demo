using GlmNet;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace HorizonEngine.Core
{
    public class Transform
    {
        private uint m_gameObjectId;

        public void SetGameObjectId(uint gameObjectId)
        {
            m_gameObjectId = gameObjectId;
        }

        public vec4 position
        {
            get
            {
                return _GetPosition(m_gameObjectId);
            }
            set
            {
                _SetPosition(m_gameObjectId, value);
            }
        }

        public vec4 eulerRotation
        {
            get
            {
                return _GetEulerRotation(m_gameObjectId);
            }
            set
            {
                _SetEulerRotation(m_gameObjectId, value);
            }
        }

        public vec4 scale
        {
            get
            {
                return _GetScale(m_gameObjectId);
            }
            set
            {
                _SetScale(m_gameObjectId, value);
            }
        }

        public quat rotation
        {
            get
            {
                return _GetRotation(m_gameObjectId);
            }
            set 
            { 
                _SetRotation(m_gameObjectId, value);
            }
        }

        public vec4 forward
        {
            get
            {
                return _GetForward(m_gameObjectId);
            }
        }

        public vec4 right
        {
            get
            {
               return _GetRight(m_gameObjectId); 
            }
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private static extern ref vec4 _GetPosition(uint gameObjectId);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private static extern void _SetPosition(uint gameObjectId, vec4 position);


        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private static extern ref vec4 _GetEulerRotation(uint gameObjectId);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private static extern void _SetEulerRotation(uint gameObjectId, vec4 eulerRotation);


        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private static extern ref vec4 _GetScale(uint gameObjectId);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private static extern void _SetScale(uint gameObjectId, vec4 scale);


        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private static extern ref quat _GetRotation(uint gameObjectId);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private static extern void _SetRotation(uint gameObjectId, quat rotation);


        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private static extern ref vec4 _GetForward(uint gameObjectId);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private static extern ref vec4 _GetRight(uint gameObjectId);

    }
}
