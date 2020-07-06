using Aim.Core;
using Aim.Structures;
using HorizonEngine.Subsystems;

namespace DustBotScript
{
    public static class DustBotScript
    {
        public static void Init()
        {
            AimCreateInfo aimCreateInfo = new AimCreateInfo()
            {
                configurationFilePath = VirtualFileSystem.ResolvePath("~/config/aim-config.json")
            };

            AimLib.CreateInstance();
            AimLib.Instance.Init(aimCreateInfo);
        }

        public static void Shutdown()
        {
            
        }
    }
}
