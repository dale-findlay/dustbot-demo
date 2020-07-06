using Aim.Agents;
using Aim.Blackboards;
using Aim.NavMesh;
using Aim.Structures;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Aim.Core
{
    public class AimLib : AimSingleton<AimLib>
    {
        public AimLib() : base()
        {

        }

        /// <summary>
        /// Initialize the Aim Library in accordance with create info structure.
        /// </summary>
        /// <returns></returns>
        public bool Init(AimCreateInfo aimCreateInfo)
        {
            AimLogger.CreateInstance();
            AimLogger.Instance.Init();

            AimLogger.Instance.LogInfo("Aim Initializing.");

            AimConfigManager.CreateInstance();
            if(!AimConfigManager.Instance.Init(aimCreateInfo.configurationFilePath))
            {
                AimLogger.Instance.LogError("Failed to Initialize Aim Config Manager.");
            }

            AimAgentFactory.CreateInstance();
            if (!AimAgentFactory.Instance.Init())
            {
                AimLogger.Instance.LogError("Failed to Initialize Aim Agent Factory.");
            }

            AimBlackboardManager.CreateInstance();
            if (!AimBlackboardManager.Instance.Init())
            {
                AimLogger.Instance.LogError("Failed to Initialize Aim Blackboard Manager.");
            }


            AimNavMeshGenerator.CreateInstance();
            if (!AimNavMeshGenerator.Instance.Init())
            {
                AimLogger.Instance.LogError("Failed to Initialize Aim NavMesh Generator.");
            }

            AimLogger.Instance.LogInfo("Aim Initialized Successfully.");

            return true;
        }


    }
}
