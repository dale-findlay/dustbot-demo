using GlmNet;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Aim.Flocking
{
    public class AimFlockingVolume
    {
        private vec3 m_v1;
        private vec3 m_v2;
        private vec3 m_v3;
        private vec3 m_v4;
        private vec3 m_v5;
        private vec3 m_v6;
        private vec3 m_v7;
        private vec3 m_v8;

        public AimFlockingVolume(vec3 v1, vec3 v2, vec3 v3, vec3 v4, vec3 v5, vec3 v6, vec3 v7, vec3 v8)
        {
            m_v1 = v1;
            m_v2 = v2;
            m_v3 = v3;
            m_v4 = v4;
            m_v5 = v5;
            m_v6 = v6;
            m_v7 = v7;
            m_v8 = v8;

            CalculateMinMax();
        }

        private void CalculateMinMax()
        {
            foreach (vec3 point in ToArray())
            {
                if(point.x < m_minX)
                {
                    m_minX = point.x;
                }

                if (point.x > m_maxX)
                {
                    m_maxX = point.x;
                }

                if (point.y < m_minY)
                {
                    m_minY = point.y;
                }

                if (point.y > m_maxY)
                {
                    m_maxY = point.y;
                }

                if (point.z < m_minZ)
                {
                    m_minZ = point.z;
                }

                if (point.z > m_maxZ)
                {
                    m_maxZ = point.z;
                }
            }
        }

        private float m_minX = 0.0f;
        public float MinX { get => m_minX; }
        
        private float m_maxX = 0.0f;
        public float MaxX { get => m_maxX; }

        private float m_minY = 0.0f;
        public float MinY { get => m_minY; }

        private float m_maxY = 0.0f;
        public float MaxY { get => m_maxY; }

        private float m_minZ = 0.0f;
        public float MinZ { get => m_minZ; }

        private float m_maxZ = 0.0f;
        public float MaxZ { get => m_maxZ; }

        /// <summary>
        /// Returns an array of vertices that represents the volume.
        /// </summary>
        /// <returns></returns>
        public vec3[] ToArray()
        {
            return new[]
            {
                m_v1,
                m_v2,
                m_v3,
                m_v4,
                m_v5,
                m_v6,
                m_v7,
                m_v8
            };
        }

        public bool PointInVolume()
        {
            return false;
        }
    }
}
