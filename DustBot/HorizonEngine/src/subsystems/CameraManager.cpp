#include "CameraManager.h"

bool Horizon::CameraManager::Init()
{
	return true;
}

void Horizon::CameraManager::Shutdown()
{
}

void Horizon::CameraManager::SetActiveCameraComponent(CameraComponent* cameraComponent)
{
	m_activeCameraComponent = cameraComponent;
}

Horizon::CameraComponent* Horizon::CameraManager::GetActiveCamera()
{
	return m_activeCameraComponent;
}
