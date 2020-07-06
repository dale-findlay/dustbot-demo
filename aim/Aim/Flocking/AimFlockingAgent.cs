using Aim.Agents;
using Aim.Blackboards;
using Aim.Core;
using GlmNet;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Aim.Flocking
{
    /// <summary>
    /// An agent that takes part in a flocking simulation.
    /// </summary>
    public class AimFlockingAgent : AimAgent
    {
        protected List<AimFlockingAgent> m_friendAgents = new List<AimFlockingAgent>();

        private AimFlockingBlackboard m_blackBoard = new AimFlockingBlackboard();

        public AimFlockingAgent()
        {
            float friendRadius = 25.0f;
            float friendSeperation = 25.0f;
            float cohesionFactor = 50.0f;

            if (!m_blackBoard.ContainsKeyWithName("friendRadius"))
            {
                m_blackBoard.AddKey(new AimBlackboardKey<AimFlockingBlackboard>("friendRadius", typeof(float), friendRadius, true));
            }

            if (!m_blackBoard.ContainsKeyWithName("friendSeperation"))
            {
                m_blackBoard.AddKey(new AimBlackboardKey<AimFlockingBlackboard>("friendSeperation", typeof(float), friendSeperation, true));
            }

            if (!m_blackBoard.ContainsKeyWithName("cohesionFactor"))
            {
                m_blackBoard.AddKey(new AimBlackboardKey<AimFlockingBlackboard>("cohesionFactor", typeof(float), cohesionFactor, true));
            }
        }

        private vec3 m_moveForce;

        /// <summary>
        /// The move current or previous force of the flocking agent.
        /// </summary>
        public vec3 MoveForce
        {
            get
            {
                return m_moveForce;
            }
        }

        public float FriendRadius
        {
            get
            {
                return (float)m_blackBoard.GetKey("friendRadius").KeyValue;
            }
        }

        public float SeperationRadius
        {
            get
            {
                return (float)m_blackBoard.GetKey("friendSeperation").KeyValue;
            }
        }

        public float CohesionFactor
        {
            get
            {
                return (float)m_blackBoard.GetKey("cohesionFactor").KeyValue;
            }
        }

        public void Flock()
        {
            float noisiness = 0.5f; 
            UpdateFriendAgents();

            float randomSeperation = (float)AimRandom.RandomRange(-2.0f, 2.0f);
            float randomAlign = (float)AimRandom.RandomRange(-1.0f, 1.0f);
            float randomCohesion = (float)AimRandom.RandomRange(-1.0f, 1.0f);

            vec3 alignForce = CalculateAlign() * randomAlign;
            vec3 seperationForce = CalculateSeperate() * randomSeperation;
            vec3 cohesion = CalculateCohesion() * randomCohesion;

            m_moveForce = alignForce + seperationForce + cohesion + (CalculateNoise() * noisiness);

            if (glm.length(m_moveForce) > 0)
            {
                m_moveForce = glm.normalize(m_moveForce) * m_maxForce;
            }

            ApplyForce(m_moveForce);
        }

        private vec3 CalculateCohesion()
        {
            vec3 cohesion = new vec3(0.0f, 0.0f, 0.0f);
            int count = 0;

            foreach (AimFlockingAgent agent in m_friendAgents)
            {
                vec3 distVec = transform.position - agent.transform.position;
                float distance = glm.length(distVec);

                if (distance < FriendRadius && distance > 0)
                {
                    cohesion += agent.transform.position;
                    count++;
                }
            }

            if (count == 0)
            {
                return cohesion;
            }

            cohesion /= count;
            vec3 cohesionFactor = cohesion - transform.position;

            return cohesionFactor;
        }

        private vec3 CalculateNoise()
        {
            return new vec3((float)AimRandom.RandomRange(-20, 20.0),
                            (float)AimRandom.RandomRange(-5, 5),
                            (float)AimRandom.RandomRange(-20, 20.0));
        }

        private vec3 CalculateSeperate()
        {
            vec3 steering = new vec3(0.0f, 0.0f, 0.0f);
            int count = 0;

            foreach (AimFlockingAgent agent in m_friendAgents)
            {
                vec3 distVec = transform.position - agent.transform.position;
                float distance = glm.length(distVec);

                if (distance < SeperationRadius && distance > 0)
                {
                    steering += distVec;
                    count++;
                }
            }

            if (count == 0)
            {
                return steering;
            }


            steering /= count;
            steering *= -1.0f;

            return steering;
        }

        /// <summary>
        /// Calculates the average direction of the nearby flocking agents and seeks a little bit in that direction.
        /// </summary>
        /// <returns></returns>
        private vec3 CalculateAlign()
        {
            vec3 averageDirection = new vec3(0.0f, 0.0f, 0.0f);
            int count = 0;

            foreach (AimFlockingAgent agent in m_friendAgents)
            {
                float distance = glm.length(transform.position - agent.transform.position);

                if (distance < FriendRadius && distance > 0)
                {
                    averageDirection += agent.Velocity;
                    count++;
                }
            }

            if (count == 0)
            {
                return averageDirection;
            }

            averageDirection /= count;
            averageDirection = glm.normalize(averageDirection);
            //averageDirection = SetMagnitude(averageDirection, m_maxSpeed);

            return averageDirection;
        }

        private void UpdateFriendAgents()
        {
            List<AimFlockingAgent> aimFlockingAgents = AimAgentFactory.Instance.GetNearbyAgents<AimFlockingAgent>(this, FriendRadius);

            m_friendAgents = aimFlockingAgents;
        }
    }
}
