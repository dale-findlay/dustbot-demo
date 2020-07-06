using Aim.BehaviourTree;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Aim.Core
{
    public abstract class AimNode
    {
        private Type m_nodeType;
        protected List<AimNode> m_childNodes = new List<AimNode>();

        public Type NodeType
        {
            get
            {
                return m_nodeType;
            }
        }

        public List<AimNode> ChildNodes
        {
            get
            {
                return m_childNodes;
            }
        }

        public AimNode(Type nodeType)
        {
            this.m_nodeType = nodeType;
        }

        public void AddChild(AimNode aimNode)
        {
            m_childNodes.Add(aimNode);
        }
        internal void ClearChildNodes()
        {
            m_childNodes.Clear();
        }

        public abstract AimTaskStatus Execute();
    }
}
