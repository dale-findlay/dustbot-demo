using Aim.Agents;
using Aim.Blackboards;
using Aim.Core;
using Aim.Flocking;
using Aim.Structures;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AimTests
{
    class Program
    {
        static void Main(string[] args)
        {
            AimCreateInfo aimCreateInfo = new AimCreateInfo()
            {
                configurationFilePath = "config/aim-config.json"
            };

            AimLib.CreateInstance();
            AimLib.Instance.Init(aimCreateInfo);

            AimBlackboardManager.CreateInstance();
            AimBlackboardManager.Instance.Init();

            AimAgentFactory.CreateInstance();
            AimAgentFactory.Instance.Init();


            List<AimFlockingAgent> agents = new List<AimFlockingAgent>();

            for(int i = 0; i < 10; ++i)
            {
                AimFlockingAgent agent = AimAgentFactory.Instance.CreateAgent<AimFlockingAgent>();
                agents.Add(agent);
            }

            while (true)
            {
                for (int i = 0; i < agents.Count; ++i)
                {
                    agents[i].Flock();
                    agents[i].Update();
                }
            }
        }
    }
}