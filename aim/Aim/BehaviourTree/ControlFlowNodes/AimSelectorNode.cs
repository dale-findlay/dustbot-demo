using Aim.Core;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Aim.BehaviourTree.ControlFlowNodes
{
    public class AimSelectorNode : AimNode
    {
        public AimSelectorNode() : base(typeof(AimSelectorNode))
        {}

        public override AimTaskStatus Execute()
        {
            foreach(AimNode node in m_childNodes)
            {
                var childStatus = node.Execute();
                if(childStatus == AimTaskStatus.Running)
                {
                    return AimTaskStatus.Running;
                }
                else if(childStatus == AimTaskStatus.Success)
                {
                    return AimTaskStatus.Success;
                }
            }

            return AimTaskStatus.Failure;
        }
    }
}
