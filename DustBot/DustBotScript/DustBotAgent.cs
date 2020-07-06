using System;
using System.Text;
using System.Linq;
using System.Threading.Tasks;
using System.Diagnostics;
using System.Collections.Generic;

using GlmNet;
using Newtonsoft.Json;
using HorizonEngine.Core;
using HorizonEngine.Structures;
using HorizonEngine.Subsystems;
using HorizonEngine.Utilities;

using Aim.Core;
using Aim.Agents;
using Aim.NavMesh;
using Aim.BehaviourTree;
using Aim.Pathfinding;
using Aim.BehaviourTree.ControlFlowNodes;

namespace DustBotScript
{
    public class DustBotAgent : Script
    {
        private AimAgent m_agent;

        private string navMeshFilePath;
        AimNavMesh m_aimNavMesh;
        AimBehaviourTree aimBehaviourTree;
        private AimCondition m_dustPathGenerated = new AimCondition();
        private AimCondition m_dustCollected = new AimCondition();
        private vec3 m_dustPosition;

        GameObject dustPostObject;

        AimAStarPath m_dustPath;

        public DustBotAgent(uint gameObjectId) : base(gameObjectId)
        {}

        public override void Start()
        {
            navMeshFilePath = GetScriptCustomAttribute("NavMesh");

            dustPostObject = Find("Dust Post");

            Console.WriteLine(navMeshFilePath);

            if (!string.IsNullOrEmpty(navMeshFilePath))
            {
                Console.WriteLine(navMeshFilePath);
                string resolvedPath = VirtualFileSystem.ResolvePath(navMeshFilePath);
                m_aimNavMesh = AimNavMeshGenerator.Instance.GenerateNavMeshFromFile(resolvedPath);
            }

            AimSequenceNode sequence = new AimSequenceNode();

            AimSelectorNode dustGetSelector = new AimSelectorNode();
            AimBehaviourTreeConditionNode dustGetCondition = new AimBehaviourTreeConditionNode(ref m_dustPathGenerated);
            AimSequenceNode dustGetSequence = new AimSequenceNode();
            AimExecutionNode generateDustPosTask = new AimExecutionNode(GenerateDustPosition);
            AimExecutionNode getPathTask = new AimExecutionNode(GetPathToDust);

            AimSelectorNode dustCollectSelector = new AimSelectorNode();
            AimBehaviourTreeConditionNode dustCollectCondition = new AimBehaviourTreeConditionNode(ref m_dustCollected);
            AimSequenceNode dustCollectSequence = new AimSequenceNode();
            AimExecutionNode moveTorwardsDustTask = new AimExecutionNode(MoveToDust);
            AimExecutionNode collectDustTask = new AimExecutionNode(CollectDust);

            aimBehaviourTree = new AimBehaviourTree();
            aimBehaviourTree.SetRootNode(sequence);

            sequence.AddChild(dustGetSelector);
            dustGetSelector.AddChild(dustGetCondition);

            dustGetSelector.AddChild(dustGetSequence);
            dustGetSequence.AddChild(generateDustPosTask);
            dustGetSequence.AddChild(getPathTask);

            sequence.AddChild(dustCollectSelector);
            dustCollectSelector.AddChild(dustCollectCondition);

            dustCollectSelector.AddChild(dustCollectSequence);
            dustCollectSequence.AddChild(moveTorwardsDustTask);
            dustCollectSequence.AddChild(collectDustTask);

            m_agent = AimAgentFactory.Instance.CreateAgent<AimAgent>();
            m_agent.transform.position = new vec3(gameObject.transform.position);
        }

        public AimTaskStatus GenerateDustPosition()
        {
            m_dustPosition = m_aimNavMesh.RandomPoint();

            if(m_aimNavMesh.PointInNavMesh(m_dustPosition))
            {
                dustPostObject.transform.position = new vec4(m_dustPosition, 1.0f);

               return AimTaskStatus.Success;
            }

            return AimTaskStatus.Failure;
        }

        public AimTaskStatus GetPathToDust()
        {
            m_agent.ResetAgentState();
            m_dustPath = AimAStar.AimAStarEvaluate(new vec3(gameObject.transform.position), m_dustPosition, m_aimNavMesh);
           
            if (m_dustPath == null || m_dustPath.LastPoint == null)
            {
                GenerateDustPosition();
                return AimTaskStatus.Failure;
            }

            m_dustPathGenerated.Condition = true;

            return AimTaskStatus.Success;
        }

        public AimTaskStatus MoveToDust()
        {           
            if(glm.length(m_agent.transform.position - m_dustPath.LastPoint) > 0.5)
            {
                m_agent.FollowPath(m_dustPath);
                return AimTaskStatus.Running;
            }

            return AimTaskStatus.Success;
        }

        public AimTaskStatus CollectDust()
        {
            //delete dust from world
                        
            m_dustPathGenerated.Condition = false;
            m_dustCollected.Condition = false;
            return AimTaskStatus.Failure;
        }

        public override void Update()
        {
            aimBehaviourTree.Execute();

            //This is a safety measure to ensure the state of the agents transform is the-
            //same  as the game objects.
            m_agent.transform.position = new vec3(gameObject.transform.position);
            m_agent.Update();

            //update game object to reflect changes within the agent.
            gameObject.transform.position = new vec4(m_agent.transform.position, 1.0f);
        }
    }
}
