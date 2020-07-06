#include "CameraComponent.h"

#include "core\GameObject.h"
#include "subsystems\WindowManager.h"

#include <glm\gtc\matrix_transform.hpp>

Horizon::CameraComponent::CameraComponent()
	:Component()
{
}

bool Horizon::CameraComponent::CreateComponent(const std::unordered_map<std::string, nlohmann::json::value_type>& componentAttributes)
{
	return true;
}

bool Horizon::CameraComponent::InitComponent()
{
	return false;
}

void Horizon::CameraComponent::ShutdownComponent()
{
}

void Horizon::CameraComponent::StartComponent()
{
	fov = 45.0f;
	nearPlane = 0.1f;
	farPlane = 10000.0f;
}

void Horizon::CameraComponent::UpdateComponent()
{
	glm::mat4 view = glm::inverse(gameObject->transform.Model());

	m_viewMatrix = view;

	float aspectRatio = (float)WindowManager::GetInstance()->GetWindowWidth() / (float)WindowManager::GetInstance()->GetWindowHeight();

	m_projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

void Horizon::CameraComponent::CreateResources()
{
}

glm::mat4 Horizon::CameraComponent::GetViewMatrix() const
{
	return m_viewMatrix;
}

glm::mat4 Horizon::CameraComponent::GetProjectionMatrix() const
{
	return m_projectionMatrix;
}
