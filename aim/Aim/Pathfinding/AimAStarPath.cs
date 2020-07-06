using GlmNet;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Aim.Pathfinding
{
    public class AimAStarPath
    {
        private List<vec3> m_pathPoints;
        private vec3 m_lastPoint;

        public AimAStarPath(List<vec3> pathPoints)
        {
            this.m_pathPoints = pathPoints;

            if (m_pathPoints.Count > 0)
            {
                m_lastPoint = m_pathPoints[m_pathPoints.Count - 1];
            }
        }

        public vec3 LastPoint
        {
            get
            {
                return m_lastPoint;
            }
        }

        public List<vec3> PathPoints
        {
            get
            {
                return m_pathPoints;
            }
        }

    }
}
