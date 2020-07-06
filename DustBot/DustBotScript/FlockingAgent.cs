using System;
using Aim.Agents;
using Aim.Flocking;
using GlmNet;
using HorizonEngine.Core;
using Newtonsoft.Json;

namespace DustBotScript
{
    public class FlockingAgent : Script
    {
        private AimFlockingAgent m_agent;

        public FlockingAgent(uint gameObjectId) : base(gameObjectId)
        {
        }

        public override void Start()
        {
            m_agent = AimAgentFactory.Instance.CreateAgent<AimFlockingAgent>();
        }

        public override void Update()
        {
            m_agent.Flock();

            m_agent.Update();

            gameObject.transform.position = new vec4(m_agent.transform.position, 1.0f);
        }
    }
}