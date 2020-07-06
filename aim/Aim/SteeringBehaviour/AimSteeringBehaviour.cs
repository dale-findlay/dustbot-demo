using Aim.Agents;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Aim.SteeringBehaviour
{
    public abstract class AimSteeringBehaviour
    {
        public abstract void Update(AimAgent agent);
    }
}
