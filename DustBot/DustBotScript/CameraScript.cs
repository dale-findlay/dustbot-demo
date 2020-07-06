using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using GlmNet;
using Newtonsoft.Json;
using HorizonEngine.Core;
using HorizonEngine.Structures;
using HorizonEngine.Subsystems;
using HorizonEngine.Utilities;

namespace DustBotScript
{
    public class CameraScript : Script
    {
        private float m_xMove;
        private float m_yMove;
        private float m_distance = 25.0f;
        private float m_mouseSensitivity = 25;

        private CameraType m_cameraType = CameraType.Debug;

        public float moveSpeed = 250;

        public CameraScript(uint gameObjectId) : base(gameObjectId)
        {}

        public override void Start()
        {
            var pos = gameObject.transform.position;
            
            string cameraType = GetScriptCustomAttribute("CameraType");
            m_cameraType = (CameraType)Enum.Parse(typeof(CameraType), cameraType);
        }

        public override void Update()
        {
            switch (m_cameraType)
            {
                case CameraType.Orbit:
                    {
                        UpdateOrbitCamera();
                        break;
                    }
                case CameraType.Debug:
                    {
                        UpdateDebugCamera();
                        break;
                    }
            }
        }

        private void UpdateDebugCamera()
        {
            if (InputManager.GetMouseButtonDown(MouseButtons.Right))
            {

                m_xMove -= InputManager.MouseDelta.x * m_mouseSensitivity * 0.0002f;
                m_yMove += InputManager.MouseDelta.y * m_mouseSensitivity * 0.0002f;

                quat q = new quat(new vec3(-m_yMove, m_xMove, 0.0f));

                gameObject.transform.rotation = q;
            }

            vec4 movementBase = GetDebugCameraMoveBase();
            gameObject.transform.position = gameObject.transform.position + movementBase;

        }

        private vec4 GetDebugCameraMoveBase()
        {
            float extraMoveSpeed = 0.0f;

            if (InputManager.GetKeyDown(Keys.Left_shift))
            {
                extraMoveSpeed = 0.25f * moveSpeed;
            }

            float moveSpeedWithExtra = moveSpeed + extraMoveSpeed;

            vec4 movementBase = new vec4();
            if (InputManager.GetKeyDown(Keys.W))
            {
                movementBase += gameObject.transform.forward * Time.DeltaTime * moveSpeedWithExtra;
            }

            if (InputManager.GetKeyDown(Keys.S))
            {
                movementBase -= gameObject.transform.forward * Time.DeltaTime * moveSpeedWithExtra;
            }

            if (InputManager.GetKeyDown(Keys.A))
            {
                movementBase += gameObject.transform.right * Time.DeltaTime * moveSpeedWithExtra;
            }

            if (InputManager.GetKeyDown(Keys.D))
            {
                movementBase -= gameObject.transform.right * Time.DeltaTime * moveSpeedWithExtra;
            }

            return movementBase;
        }

        private void UpdateOrbitCamera()
        {
            if (InputManager.GetMouseButtonDown(MouseButtons.Right))
            {
                m_xMove -= InputManager.MouseDelta.x * m_mouseSensitivity * 0.0002f;
                m_yMove += InputManager.MouseDelta.y * m_mouseSensitivity * 0.0002f;

                quat q = new quat(new vec3(m_yMove, m_xMove, 0.0f));

                m_distance -= InputManager.ScrollDelta * 5;
                m_distance = MathUtil.Clamp(m_distance, 0, 100);

                vec3 position = q * new vec3(0, 2, -m_distance);

                gameObject.transform.position = new vec4(position.x, position.y, position.z, 1.0f);
                gameObject.transform.rotation = q;
            }
        }
    }
}
