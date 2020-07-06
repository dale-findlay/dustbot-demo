#include "LightComponent.h"

Horizon::LightComponent::LightComponent(uint32 lightType)
	:Component(), m_lightType(lightType)
{
}

bool Horizon::LightComponent::CreateComponent(const std::unordered_map<std::string, nlohmann::json::value_type>& componentAttributes)
{
	return true;
}

bool Horizon::LightComponent::InitComponent()
{
	return true;
}

void Horizon::LightComponent::ShutdownComponent()
{

}

void Horizon::LightComponent::StartComponent()
{

}

void Horizon::LightComponent::UpdateComponent()
{

}

void Horizon::LightComponent::CreateResources()
{
}

Horizon::uint32 Horizon::LightComponent::GetLightType() const
{
	return m_lightType;
}
