using HorizonEngine.Structures;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace HorizonEngine.Subsystems
{
    public static class InputManager
    {
        public static float ScrollDelta
        {
            get
            {
                return _GetScrollOffset();
            }
        }

        public static GlmNet.vec2 MouseDelta
        {
            get
            {
                return _GetMouseDelta();
            }
        }

        public static float MouseXDelta
        {
            get
            {
                return _GetMouseDelta().x;
            }
        }
        public static float MouseYDelta
        {
            get
            {
                return _GetMouseDelta().y;
            }
        }

        public static bool GetMouseButtonDown(MouseButtons button)
        {
            return _GetMouseButtonDown((int)button);
        }

        public static bool GetKeyDown(Keys key)
        {
            return _GetKeyDown((int)key);
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern bool _GetMouseButtonDown(int mouseButtonCode);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern ref GlmNet.vec2 _GetMouseDelta();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern bool _GetKeyDown(int keyCode);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern float _GetScrollOffset();
    }
}
