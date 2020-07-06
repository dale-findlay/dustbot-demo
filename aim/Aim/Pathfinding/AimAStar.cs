using Aim.Core;
using Aim.NavMesh;
using GlmNet;
using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Aim.Pathfinding
{
    public static class AimAStar
    {
        private class AimAStarNode
        {
            public AimAStarNode parentNode;
            public float g;
            public float h;
            public float f;

            public vec3 vertex;

            public AimAStarNode(vec3 vertex, vec3 lastPoint, vec3 targetPoint, AimAStarNode parentNode)
            {
                this.vertex = vertex;
                this.g = CalculateGCost(vertex, lastPoint);
                this.h = glm.length(targetPoint - vertex);
                this.f = g + h;
                this.parentNode = parentNode;
            }

            public void UpdateGCost(float gCost)
            {
                g = gCost;
                f = g + h;
            }

            public static float CalculateGCost(vec3 vertex, vec3 lastPoint)
            {
                return glm.length(lastPoint - vertex);
            }

        }

        public static AimAStarPath AimAStarEvaluate(vec3 startPosition, vec3 targetPoint, AimNavMesh navMesh, bool smoothing = true)
        {
            try
            {

                if (!navMesh.PointInNavMesh(targetPoint))
                {
                    return null;
                }

                List<AimAStarNode> openList = new List<AimAStarNode>();
                List<AimAStarNode> closedList = new List<AimAStarNode>();

                vec3 currentPosition = startPosition;
                vec3 lastPoint = startPosition;
                AimAStarNode lastNode = new AimAStarNode(currentPosition, lastPoint, targetPoint, null);

                List<AimNavMeshTriangle> targetTriangles = navMesh.FindTriangles(targetPoint);

                AimNavMeshTriangle targetTriangle = null;
                if (targetTriangles.Count > 0)
                {
                    targetTriangle = targetTriangles[0];
                }
                else
                {
                    return null;
                }

                while (true)
                {
                    List<AimNavMeshTriangle> trianglesWithCurrentPoint = navMesh.FindTriangles(currentPosition);

                    bool foundTargetTriangle = false;
                    foreach (AimNavMeshTriangle triangle in trianglesWithCurrentPoint)
                    {
                        if (triangle == targetTriangle)
                        {
                            foundTargetTriangle = true;
                        }
                    }

                    if (foundTargetTriangle)
                    {
                        break;
                    }

                    List<vec3> adjacentPoints = navMesh.GetAdjacentPoints(currentPosition);

                    foreach (vec3 point in adjacentPoints)
                    {
                        bool pointFound = false;

                        foreach (AimAStarNode openListNode in openList)
                        {
                            if (openListNode.vertex == point)
                            {
                                pointFound = true;
                                float newGCost = AimAStarNode.CalculateGCost(point, lastPoint);

                                if (newGCost < openListNode.g)
                                {
                                    openListNode.UpdateGCost(newGCost);
                                }
                                break;
                            }
                        }

                        if (!pointFound)
                        {
                            bool found = false;
                            foreach (AimAStarNode closedListNode in closedList)
                            {
                                if (closedListNode.vertex == point)
                                {
                                    found = true;
                                }
                            }

                            if (!found)
                            {
                                openList.Add(new AimAStarNode(point, lastPoint, targetPoint, lastNode));
                            }
                        }
                    }

                    AimAStarNode lowestNode = GetNodeWithLowestFCost(openList);

                    if (lowestNode == null)
                    {
                        return null;
                    }

                    foreach (AimAStarNode node in openList)
                    {
                        closedList.Add(node);
                    }

                    openList = new List<AimAStarNode>();

                    lastNode = lowestNode;
                    lastPoint = currentPosition;
                    currentPosition = lowestNode.vertex;
                }

                if (lastNode != null)
                {
                    AimAStarNode currentNode = lastNode;

                    List<vec3> points = new List<vec3>();

                    while (currentNode.parentNode != null)
                    {
                        //push to the front.
                        points.Insert(0, currentNode.vertex);


                        currentNode = currentNode.parentNode;
                    }

                    points.Add(targetPoint);

                    if (points.Count > 1)
                    { 
                        vec3 currentPoint = points[0];
                        vec3 checkPoint = points[1];

                        for(int i = 1; i < points.Count-1; ++i)
                        {
                            vec3 nextPoint = points[i];
                        }
                    }

                    return new AimAStarPath(points);
                }
                else
                {
                    return null;
                }
            }
            catch (Exception e)
            {
                AimLogger.Instance.LogError(e);
                return null;
            }
        }

        private static AimAStarNode GetNodeWithLowestFCost(List<AimAStarNode> openList)
        {
            AimAStarNode lowest = null;
            foreach (AimAStarNode node in openList)
            {
                if (lowest != null)
                {
                    if (node.f < lowest.f)
                    {
                        lowest = node;
                    }
                }
                else
                {
                    lowest = node;
                }
            }

            return lowest;
        }
    }
}
