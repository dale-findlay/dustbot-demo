#include "LightManager.h"

#include "components\LightComponent.h"
#include "core\GameObject.h"

#include <glm\common.hpp>

using namespace Horizon::Graphics;

Horizon::LightManager::LightManager()
	:m_shouldRegenerateLightCache(true)
{}

bool Horizon::LightManager::Init()
{
	return true;
}

void Horizon::LightManager::Shutdown()
{
}

void Horizon::LightManager::AddLightComponent(LightComponent* light)
{
	m_lights.push_back(light);
	light->gameObject->transform.AddRecalculateCallback([this] { this->m_shouldRegenerateLightCache = true; });
}

std::vector<Horizon::LightComponent*> Horizon::LightManager::GetLightComponents() const
{
	return m_lights;
}

std::vector<UniformLight> Horizon::LightManager::GetUniformLightData()
{
	if (m_shouldRegenerateLightCache)
	{
		RegenerateLightDataCache();
		m_shouldRegenerateLightCache = false;
	}

	return m_lightDataCache;
}

void Horizon::LightManager::RegenerateLightDataCache()
{
	auto lightComponents = GetLightComponents();

	std::vector<UniformLight> lightData(MAX_LIGHT_COUNT, UniformLight(-1, glm::vec4(0.0f), glm::vec4(0.0f)));

	int i = 0;
	for (auto lightComponentIt = lightComponents.begin(); lightComponentIt != lightComponents.end(); ++lightComponentIt)
	{
		lightData[i].lightType = (*lightComponentIt)->GetLightType();
		lightData[i].lightPos = glm::vec4((*lightComponentIt)->gameObject->transform.Position(), 1.0f);
		lightData[i].lightDirection = glm::vec4((*lightComponentIt)->gameObject->transform.Forward(), 1.0f);
		i++;
	}

	m_lightDataCache = lightData;
}
