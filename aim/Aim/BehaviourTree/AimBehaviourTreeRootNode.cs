using Aim.Core;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Aim.BehaviourTree
{
    public class AimBehaviourTreeRootNode : AimNode
    {
        public AimBehaviourTreeRootNode() : base(typeof(AimBehaviourTreeRootNode))
        {}

        public override AimTaskStatus Execute()
        {
            if(m_childNodes.Count > 0)
            {
                return m_childNodes[0].Execute();
            }
            else
            {
                return AimTaskStatus.Failure;
            }
        }
    }
}
