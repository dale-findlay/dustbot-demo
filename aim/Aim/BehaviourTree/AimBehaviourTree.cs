using Aim.Core;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Aim.BehaviourTree
{
    public class AimBehaviourTree
    {
        private AimBehaviourTreeRootNode m_rootNode = new AimBehaviourTreeRootNode();

        public void SetRootNode(AimNode node)
        {
            m_rootNode.ClearChildNodes();
            m_rootNode.AddChild(node);
        }

        /// <summary>
        /// Execute the behavior tree.
        /// </summary>
        public void Execute()
        {
            m_rootNode.Execute();
        }
    }
}
