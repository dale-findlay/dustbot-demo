using System;
using System.Collections.Generic;
using HorizonEngine.Core;
using HorizonEngine.Components;
using GlmNet;

namespace DustBotScript
{
    public class FlockingDemo : Script
    {
        public FlockingDemo(uint gameObjectId) : base(gameObjectId)
        {}

        List<GameObject> m_boids = new List<GameObject>();

        public override void Start()
        {
            string boidModel = GetScriptCustomAttribute("boidModelResourceId");
            string boidCountStr = GetScriptCustomAttribute("boidCount");

            int boidCount = int.Parse(boidCountStr);

            for(int i = 0; i < boidCount; ++i)
            {
                GameObject newGameObject = new GameObject($"Boid ({i})");
                newGameObject.Visible = true;
                newGameObject.transform.scale = new vec4(1.0f, 1.0f, 1.0f, 1.0f);

                //bird.obj.
                MeshComponent meshComponent = new MeshComponent(boidModel);

                newGameObject.AddComponent<MeshComponent>(meshComponent);

                meshComponent.Dispose();

                newGameObject.AddScript("FlockingAgent");
                
                m_boids.Add(newGameObject);
            }
        }

        public override void Update()
        {}
    }
}