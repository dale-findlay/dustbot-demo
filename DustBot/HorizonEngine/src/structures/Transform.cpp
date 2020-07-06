#include "Transform.h"

#include "subsystems\EngineAllocator.h"
#include "utilities\Logger.h"

#include <glm\common.hpp>
#include <glm\gtx\euler_angles.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\quaternion.hpp>

#include <string>

Horizon::Transform::Transform()
	: m_shouldRecalculate(true)
{
	m_managedPosition = EngineAllocateC(glm::vec4, glm::vec4());
	m_managedEulerRotation = EngineAllocateC(glm::vec4, glm::vec4());
	m_managedScale = EngineAllocateC(glm::vec4, glm::vec4());
	m_managedRotation = EngineAllocateC(glm::quat, glm::quat());

	m_managedForward = EngineAllocateC(glm::vec4, glm::vec4());
	m_managedRight = EngineAllocateC(glm::vec4, glm::vec4());
}

void Horizon::Transform::SetPosition(const glm::vec3& position)
{
	m_position = position;

	glm::vec4 v = glm::vec4(m_position, 1.0f);

	memcpy(m_managedPosition, &v, sizeof(glm::vec4));

	if (m_previousPosition != m_position)
	{
		m_shouldRecalculate = true;
		for (auto it = m_recalculateCallbacks.begin(); it != m_recalculateCallbacks.end(); ++it)
		{
			auto callback = (*it);

			callback();
		}
	}
}

void Horizon::Transform::SetEulerRotation(const glm::vec3& eulerRotation)
{
	m_eulerRotation = eulerRotation;
	m_rotation = glm::quat(glm::radians(eulerRotation));

	glm::vec4 v = glm::vec4(m_eulerRotation, 1.0f);

	memcpy(m_managedEulerRotation, &v, sizeof(glm::vec4)); 
	memcpy(m_managedRotation, &m_rotation, sizeof(glm::quat));

	if (m_previousEulerRotation != m_eulerRotation)
	{
		m_shouldRecalculate = true;
		for (auto it = m_recalculateCallbacks.begin(); it != m_recalculateCallbacks.end(); ++it)
		{
			auto callback = (*it);

			callback();
		}
	}
}

void Horizon::Transform::SetScale(const glm::vec3& scale)
{
	m_scale = scale;

	glm::vec4 v = glm::vec4(m_scale, 1.0f);
	memcpy(m_managedScale, &v, sizeof(glm::vec4));

	if (m_previousScale != m_scale)
	{
		m_shouldRecalculate = true;
		for (auto it = m_recalculateCallbacks.begin(); it != m_recalculateCallbacks.end(); ++it)
		{
			auto callback = (*it);

			callback();
		}
	}
}

void Horizon::Transform::SetRotation(const glm::quat& rotation)
{
	m_rotation = rotation;

	glm::vec3 eulerAngles = glm::eulerAngles(rotation);
	m_eulerRotation = glm::degrees(eulerAngles);
	
	glm::vec4 v = glm::vec4(m_eulerRotation, 1.0f);

	memcpy(m_managedEulerRotation, &v, sizeof(glm::vec4));
	memcpy(m_managedRotation, &m_rotation, sizeof(glm::quat));

	if (m_previousRotation != m_rotation)
	{
		m_shouldRecalculate = true;
		for (auto it = m_recalculateCallbacks.begin(); it != m_recalculateCallbacks.end(); ++it)
		{
			auto callback = (*it);

			callback();
		}
	}
}

glm::vec3 Horizon::Transform::Position()
{
	return m_position;
}

glm::vec4* Horizon::Transform::ManagedPosition()
{
	return m_managedPosition;
}

glm::vec3 Horizon::Transform::EulerRotation()
{
	return m_eulerRotation;
}

glm::vec4* Horizon::Transform::ManagedEulerRotation()
{
	return m_managedEulerRotation;
}

glm::vec3 Horizon::Transform::Scale()
{
	return m_scale;
}

glm::vec4 * Horizon::Transform::ManagedScale()
{
	return m_managedScale;
}

glm::quat Horizon::Transform::Rotation()
{
	return m_rotation;
}

glm::quat* Horizon::Transform::ManagedRotation()
{
	return m_managedRotation;
}

glm::vec3 Horizon::Transform::Forward()
{
	glm::mat4 tModel = CalculateModel();

	glm::vec4 forward = tModel[2];
	
	return glm::vec3(forward.x, forward.y, forward.z);
}

glm::vec4* Horizon::Transform::ManagedForward()
{
	glm::vec4 v = glm::vec4(Forward(), 1.0f);

	memcpy(m_managedForward, &v, sizeof(glm::vec4));

	return m_managedForward;
}

glm::vec3 Horizon::Transform::Right()
{
	glm::mat4 tModel = CalculateModel();

	glm::vec4 right = tModel[0];

	return glm::vec3(right.x, right.y, right.z);
}

glm::vec4* Horizon::Transform::ManagedRight()
{
	glm::vec4 v = glm::vec4(Right(), 1.0f);
	
	memcpy(m_managedRight, &v, sizeof(glm::vec4));

	return m_managedRight;
}

glm::mat4 Horizon::Transform::Model(bool forceRecalculate)
{
	if (m_shouldRecalculate || forceRecalculate)
	{
		m_model = CalculateModel();
		m_shouldRecalculate = false;
	}
	
	return m_model;
}

void Horizon::Transform::AddRecalculateCallback(std::function<void()> callback)
{
	m_recalculateCallbacks.push_back(callback);
}

glm::mat4 Horizon::Transform::CalculateModel()
{
	glm::mat4 model;

	glm::mat4 translation = glm::translate(glm::mat4(), m_position);
	glm::mat4 rotation = glm::toMat4(m_rotation);
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), m_scale);

	model = translation * rotation * scale;

	m_previousPosition = m_position;
	m_previousEulerRotation = m_eulerRotation;
	m_previousRotation = m_rotation;
	m_previousScale = m_scale;

	return model;
}
