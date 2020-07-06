using HorizonEngine.Core;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HorizonEngine.Subsystems
{
    public static class ScriptManager
    {
        private static List<Script> m_scripts = new List<Script>();

        public static void TrackScript(Script script)
        {
            m_scripts.Add(script);
        }
       
        public static void Start()
        {
            foreach(var script in m_scripts)
            {
                script.Start();
            }
        }

        public static void Update()
        {
            foreach(var script in m_scripts)
            {
                script.Update();
            }
        }
    }
}
