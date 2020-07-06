using Aim.Core;
using GlmNet;
using Newtonsoft.Json;
using ObjParser;
using ObjParser.Types;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Aim.NavMesh
{
    public class AimNavMeshGenerator : AimSingleton<AimNavMeshGenerator>
    {
        public bool Init()
        {
            return true;
        }

        public AimNavMesh GenerateNavMeshFromFile(string filePath)
        {
            Obj obj = new Obj();

            obj.LoadObj(filePath);

            AimNavMesh aimNavMesh = new AimNavMesh();

            List<Vertex> vertices = obj.VertexList;

            int i = 0;
            foreach (var face in obj.FaceList)
            {
                int v1Index = face.VertexIndexList[0] - 1;
                int v2Index = face.VertexIndexList[1] - 1;
                int v3Index = face.VertexIndexList[2] - 1;


                aimNavMesh.aimNavMeshTriangles.Add(new AimNavMeshTriangle()
                {
                    v1 = GetObjVertex(vertices[v1Index]),
                    v2 = GetObjVertex(vertices[v2Index]),
                    v3 = GetObjVertex(vertices[v3Index])
                });

                i++;
            }

            return aimNavMesh;
        }

        private vec3 GetObjVertex(Vertex v)
        {
            return new vec3((float)v.X, (float)v.Y, (float)v.Z);
        }

        public void Shutdown()
        {

        }
    }
}
