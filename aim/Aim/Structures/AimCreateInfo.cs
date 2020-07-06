using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Aim.Structures
{
    /// <summary>
    /// The structure that instructs how aim should be initialized.
    /// </summary>
    public struct AimCreateInfo
    {
        //The filepath of the aim-config.json configuration file.
        public string configurationFilePath;
    }
}
