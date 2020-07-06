using Aim.Core;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Aim.BehaviourTree
{
    public class AimBehaviourTreeConditionNode : AimNode
    {
        private AimCondition m_condition = null;

        public AimBehaviourTreeConditionNode(ref AimCondition condition) : base(typeof(AimBehaviourTreeConditionNode))
        {
            m_condition = condition;
        }
        
        public override AimTaskStatus Execute()
        {
            return AimTaskStatusHelper.FromBoolean(m_condition.Condition);
        }
    }
}
