using Aim.Core;
using GlmNet;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Aim.NavMesh
{
    public class AimNavMesh
    {
        public List<AimNavMeshTriangle> aimNavMeshTriangles = new List<AimNavMeshTriangle>();

        /// <summary>
        /// Returns a random triangle within the navmesh.
        /// </summary>
        /// <returns></returns>
        public AimNavMeshTriangle GetRandomTriangle()
        {
            if (aimNavMeshTriangles.Count > 0)
            {
                int randomIndex = AimRandom.RandomRange(0, aimNavMeshTriangles.Count);
                return aimNavMeshTriangles[randomIndex];
            }
                       
            return new AimNavMeshTriangle();
        }

        public List<AimNavMeshTriangle> FindTriangles(vec3 targetPoint)
        {
            List<AimNavMeshTriangle> triangles = new List<AimNavMeshTriangle>();

            foreach (AimNavMeshTriangle navMeshTriangle in aimNavMeshTriangles)
            {
                if (navMeshTriangle.PointWithinTriangle(targetPoint))
                {
                    triangles.Add(navMeshTriangle);
                }
            }

            return triangles;
        }

        /// <summary>
        /// Generates a random point within the navmesh.
        /// </summary>
        /// <returns></returns>
        public vec3 RandomPoint()
        {
            AimNavMeshTriangle randomTriangle = GetRandomTriangle();

            float r1 = AimRandom.RandomRange(0.0f, 1.0f);
            float r2 = AimRandom.RandomRange(0.0f, 1.0f);

            if ((r1 + r2) > 1.0f)
            {
                r1 = 1.0f - r1;
                r2 = 1.0f - r2;
            }

            float r3 = 1 - r1 - r2;

            float x = (r1 * randomTriangle.v1.x) + (r2 * randomTriangle.v2.x) + (r3 * randomTriangle.v3.x);
            float y = (r1 * randomTriangle.v1.y) + (r2 * randomTriangle.v2.y) + (r3 * randomTriangle.v3.y);
            float z = (r1 * randomTriangle.v1.z) + (r2 * randomTriangle.v2.z) + (r3 * randomTriangle.v3.z);

            vec3 p = new vec3(x, y, z);

            return p;
        }

        /// <summary>
        /// Returns a list of adjacent points to the specified target point.
        /// </summary>
        /// <returns></returns>
        public List<vec3> GetAdjacentPoints(vec3 targetPoint)
        {
            List<vec3> adjacentPoints = new List<vec3>();

            foreach (AimNavMeshTriangle navMeshTriangle in aimNavMeshTriangles)
            {
                if (navMeshTriangle.PointWithinTriangle(targetPoint))
                {
                    adjacentPoints.Add(navMeshTriangle.v1);
                    adjacentPoints.Add(navMeshTriangle.v2);
                    adjacentPoints.Add(navMeshTriangle.v3);
                }
            }

            if(adjacentPoints.Contains(targetPoint))
            {
                //ensure the target point isn't in the list.
                adjacentPoints.Remove(targetPoint);
            }

            return adjacentPoints;
        }

        /// <summary>
        /// Returns true if the specified target point is within the bounds of the navmesh. false otherwise.
        /// </summary>
        /// <param name="targetPoint"></param>
        /// <returns></returns>
        public bool PointInNavMesh(vec3 targetPoint)
        {
            int notInTriangleCount = 0;
            foreach (AimNavMeshTriangle navMeshTriangle in aimNavMeshTriangles)
            {
                if (!navMeshTriangle.PointWithinTriangle(targetPoint))
                {
                    notInTriangleCount++;
                }
            }

            return notInTriangleCount != aimNavMeshTriangles.Count;
        }
    }




}
