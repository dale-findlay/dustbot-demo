using GlmNet;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Aim.NavMesh
{
    /// <summary>
    /// A single triangle within a navmesh.
    /// </summary>
    public class AimNavMeshTriangle
    {
        public vec3 v1;
        public vec3 v2;
        public vec3 v3;

        public static bool operator== (AimNavMeshTriangle left, AimNavMeshTriangle right)
        {
            if((object)left == null || (object)right == null)
            {
                return (object)left == (object)right;
            }

            if(left != null && right != null)
            {
                return left.v1 == right.v1 && left.v2 == right.v2 && left.v3 == right.v3;
            }
            else
            {
                return false;
            }                       
        }

        public static bool operator !=(AimNavMeshTriangle left, AimNavMeshTriangle right)
        {
            return !(left == right);
        }

        public bool PointWithinTriangle(vec3 targetPoint)
        {
            double x1 = v1.x, z1 = v1.z;
            double x2 = v2.x, z2 = v2.z;
            double x3 = v3.x, z3 = v3.z;

            double a = ((z2 - z3) * (targetPoint.x - x3) + (x3 - x2) * (targetPoint.z - z3)) / ((z2 - z3) * (x1 - x3) + (x3 - x2) * (z1 - z3));
            double b = ((z3 - z1) * (targetPoint.x - x3) + (x1 - x3) * (targetPoint.z - z3)) / ((z2 - z3) * (x1 - x3) + (x3 - x2) * (z1 - z3));
            double c = 1 - a - b;

            if (a >= 0 && a <= 1 && b >= 0 && b <= 1 && c >= 0 && c <= 1)
            {
                return true;
            }

            return false;
        }

        public override bool Equals(object obj)
        {
            var triangle = obj as AimNavMeshTriangle;
            return triangle != null &&
                   EqualityComparer<vec3>.Default.Equals(v1, triangle.v1) &&
                   EqualityComparer<vec3>.Default.Equals(v2, triangle.v2) &&
                   EqualityComparer<vec3>.Default.Equals(v3, triangle.v3);
        }

        public override int GetHashCode()
        {
            var hashCode = -2005719105;
            hashCode = hashCode * -1521134295 + EqualityComparer<vec3>.Default.GetHashCode(v1);
            hashCode = hashCode * -1521134295 + EqualityComparer<vec3>.Default.GetHashCode(v2);
            hashCode = hashCode * -1521134295 + EqualityComparer<vec3>.Default.GetHashCode(v3);
            return hashCode;
        }
    }
}
