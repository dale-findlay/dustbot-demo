using Aim.Core;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Aim.BehaviourTree
{
    public class AimExecutionNode : AimNode
    {
        public delegate AimTaskStatus AimTaskFunc();

        private AimTaskFunc m_aimTaskFunc;

        public AimExecutionNode(AimTaskFunc taskFunc) : base(typeof(AimExecutionNode))
        {
            this.m_aimTaskFunc = taskFunc;
        }

        public override AimTaskStatus Execute()
        {
            return m_aimTaskFunc();
        }
    }
}
