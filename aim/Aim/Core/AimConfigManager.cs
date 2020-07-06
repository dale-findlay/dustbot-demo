using System;
using Aim.Helpers;
using Aim.Structures;
using Newtonsoft.Json;

namespace Aim.Core
{
    public class AimConfigManager: AimSingleton<AimConfigManager>
    {
        private AimConfigFile m_aimConfigFile;

        public AimConfigManager() : base()
        {}

        /// <summary>
        /// Initialize the config helper with the absolute path to the config file path.
        /// </summary>
        /// <param name="configFilePath"></param>
        public bool Init(string configFilePath)
        {
            string fileContents = FileHelper.ReadToEnd(configFilePath);

            if(string.IsNullOrEmpty(fileContents))
            {
                return false;
            }

            m_aimConfigFile = JsonConvert.DeserializeObject<AimConfigFile>(fileContents);

            return true;
        }

        /// <summary>
        /// Returns the value of the key in the AimConfigFile. If no key is 
        /// </summary>
        /// <param name="key"></param>
        /// <param name="defaultValue"></param>
        /// <returns></returns>
        public uint GetConfig(string key, uint defaultValue = 0)
        {
            var fieldValue = typeof(AimConfigFile).GetField(key);
            uint? value = fieldValue != null ? fieldValue.GetValue(m_aimConfigFile) as uint? : null;
            return value ?? defaultValue;
        }
    }
}
