using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Aim.BehaviourTree
{
    /// <summary>
    /// Encapsulates a bool.
    /// </summary>
    public class AimCondition
    {
        private bool m_condition = false;
        
        /// <summary>
        /// The condition.
        /// </summary>
        public bool Condition
        {
            get
            {
                return m_condition;
            }
            set
            {
                m_condition = value;
            }
        }
    }
}
