#pragma once

#include "core\ExportMacro.h"
#include "core\TypeAbstractions.h"
#include "components\Component.h"

#include <glm\glm.hpp>

namespace Horizon
{
	enum CameraType
	{
		Perspective,
		Orthographic
	};

	class HORIZON_API CameraComponent : public Component
	{
	public:
		CameraComponent();
		// Inherited via Component
		virtual bool CreateComponent(const std::unordered_map<std::string, nlohmann::json::value_type>& componentAttributes) override;
		virtual bool InitComponent() override;
		virtual void ShutdownComponent() override;
		
		virtual void StartComponent() override;
		virtual void UpdateComponent() override;
		
		virtual void CreateResources() override;

		glm::mat4 GetViewMatrix() const;
		glm::mat4 GetProjectionMatrix() const;
		
		float32 fov;
		float32 nearPlane;
		float32 farPlane;

	protected:
		glm::mat4 m_viewMatrix;
		glm::mat4 m_projectionMatrix;
	};
}
