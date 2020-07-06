using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using Aim.Core;
using GlmNet;
using Newtonsoft.Json;

namespace Aim.Agents
{
    /// <summary>
    /// This class is used to create and manage Agents within a given simulation.
    /// </summary>
    public class AimAgentFactory : AimSingleton<AimAgentFactory>
    {
        private static Dictionary<Type, List<object>> m_agentPools = new Dictionary<Type, List<object>>();
        private static uint m_agentPoolSize = 0;

        public AimAgentFactory() : base()
        {}

        public bool Init()
        {
            m_agentPoolSize = AimConfigManager.Instance.GetConfig("poolSize");

            m_agentPools[typeof(AimAgent)] = new List<object>();

            return true;
        }

        /// <summary>
        /// Creates a new Agent.
        /// </summary>
        /// <returns></returns>
        public T CreateAgent<T>() where T : AimAgent, new()
        {
            if (!m_agentPools.ContainsKey(typeof(T)))
            {
                m_agentPools[typeof(T)] = new List<object>();
            }
            
            T agent = new T();
            agent.ActiveInPool = true;
            m_agentPools[typeof(T)].Add(agent);

            return agent;
        }

        public List<T> GetNearbyAgents<T>(T agent, float friendRadius)
            where T : AimAgent, new()
        {
            if (!m_agentPools.ContainsKey(typeof(T)))
            {
                m_agentPools[typeof(T)] = new List<object>();
                return new List<T>();
            }

            List<object> pool = m_agentPools[typeof(T)];

            List<T> agentsNearby = new List<T>();

            foreach (object obj in pool)
            {
                T agentToTest = (T)obj;

                if (agentToTest == agent)
                {
                    continue;
                }

                vec3 otherPosition = agentToTest.transform.position;

                if (Math.Abs(otherPosition.x - agent.transform.position.x) < friendRadius &&
                    Math.Abs(otherPosition.y - agent.transform.position.y) < friendRadius &&
                    Math.Abs(otherPosition.z - agent.transform.position.z) < friendRadius)
                {
                    agentsNearby.Add(agentToTest);
                }

            }

            return agentsNearby;
        }
    }
}
