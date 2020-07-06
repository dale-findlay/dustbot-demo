using Aim.Core;
using Aim.Flocking;
using Aim.NavMesh;
using Aim.Pathfinding;
using GlmNet;
using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Aim.Agents
{
    public class AimAgent : AimPoolableObject
    {
        public AimTransform transform = new AimTransform();

        protected vec3 m_currentVelocity = new vec3(0.0f);
        protected vec3 m_acceleration = new vec3(0.0f);
        protected float m_maxForce = 3.0f;
        protected float m_maxSpeed = 0.4f;

        private int m_currentNodeIndex = -1;

        public vec3 Velocity
        {
            get
            {
                return m_currentVelocity;
            }
        }

        protected vec3 Limit(vec3 vec, float max)
        {
            if (glm.length(vec) > max)
            {
                return glm.normalize(vec) * max;
            }
            else
            {
                return vec;
            }
        }

        protected vec3 SetMagnitude(vec3 vec, float mag)
        {
            if(glm.length(vec) == 0)
            {
                return vec;
            }
            
            return glm.normalize(vec) * mag;
        }

        public AimAgent()
        {
            ResetAgentState();
        }

        public void ApplyForce(vec3 force)
        {
            m_acceleration += force;
        }

        public void Seek(vec3 targetPoint)
        {
            vec3 steeringForce = CalculateSeek(targetPoint);
            ApplyForce(steeringForce);
        }

        protected vec3 CalculateSeek(vec3 targetPoint)
        {
            vec3 desired = targetPoint - transform.position;
            if (glm.length(desired) > 0)
            {
                m_currentVelocity = glm.normalize(m_currentVelocity) * m_maxSpeed;
                vec3 steeringForce = desired - m_currentVelocity;

                return desired;
            }

            return new vec3(0.0f, 0.0f, 0.0f);
        }

        public void Flock(AimFlockingVolume volume)
        {

        }

        /// <summary>
        /// This function resets any runtime states of any of the steering behaviors.
        /// </summary>
        public void ResetAgentState()
        {
            m_acceleration = new vec3(0);
            m_currentVelocity = new vec3(m_maxSpeed);
            m_currentNodeIndex = 0;
        }

        public void FollowPath(AimAStarPath path)
        {
            if (path == null || path.PathPoints.Count == 0)
            {
                return;
            }

            if (m_currentNodeIndex >= path.PathPoints.Count)
            {
                m_currentNodeIndex = path.PathPoints.Count - 1;
            }

            if (m_currentNodeIndex == -1)
            {
                m_currentNodeIndex = 0;
            }

            vec3 targetPoint = path.PathPoints[m_currentNodeIndex];

            if (glm.length(targetPoint - transform.position) <= 1)
            {
                m_currentNodeIndex++;
            }

            Seek(targetPoint);
        }

        public void Update()
        {
            m_currentVelocity += m_acceleration;

            if (glm.length(m_currentVelocity) >= m_maxSpeed)
            {
                m_currentVelocity = glm.normalize(m_currentVelocity) * m_maxSpeed;
            }

            transform.position += m_currentVelocity;

            m_acceleration = new vec3(0.0f);
        }
    }
}
