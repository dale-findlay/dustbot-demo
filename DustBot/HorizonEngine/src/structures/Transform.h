#pragma once

#include "core\ExportMacro.h"

#include <vector>
#include <functional>
#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>

namespace Horizon
{
	class HORIZON_API Transform
	{
	public:
		Transform();

		void SetPosition(const glm::vec3& position);

		//Set the Euler rotation angles in degrees.
		void SetEulerRotation(const glm::vec3& eulerRotation);

		void SetScale(const glm::vec3& scale);
		void SetRotation(const glm::quat& rotation);

		glm::vec3 Position();
		glm::vec4* ManagedPosition();

		glm::vec3 EulerRotation();
		glm::vec4* ManagedEulerRotation();
		
		glm::vec3 Scale();
		glm::vec4* ManagedScale();

		glm::quat Rotation();
		glm::quat* ManagedRotation();

		glm::vec3 Forward();
		glm::vec4* ManagedForward();

		glm::vec3 Right();
		glm::vec4* ManagedRight();

		glm::mat4 Model(bool forceRecalculate = false);

		void AddRecalculateCallback(std::function<void()> callback);

	private:
		glm::mat4 CalculateModel();
		
		std::vector<std::function<void()>> m_recalculateCallbacks;
		bool m_shouldRecalculate;
		glm::mat4 m_model;

		glm::vec3 m_previousPosition;
		glm::vec3 m_position;
		glm::vec4* m_managedPosition;

		glm::vec3 m_previousEulerRotation;
		glm::vec3 m_eulerRotation;
		glm::vec4* m_managedEulerRotation;

		glm::vec3 m_previousScale;
		glm::vec3 m_scale;
		glm::vec4* m_managedScale;
		
		glm::quat m_previousRotation;
		glm::quat m_rotation;
		glm::quat* m_managedRotation;

		glm::vec4* m_managedForward;
		glm::vec4* m_managedRight;
	};
}
