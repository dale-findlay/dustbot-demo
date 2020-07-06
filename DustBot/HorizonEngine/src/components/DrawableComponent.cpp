#include "DrawableComponent.h"

bool Horizon::DrawableComponent::CreateComponent(const std::unordered_map<std::string,nlohmann::json::value_type>& componentAttributes)
{
	return false;
}

bool Horizon::DrawableComponent::InitComponent()
{
	return false;
}

void Horizon::DrawableComponent::StartComponent()
{
}

void Horizon::DrawableComponent::ShutdownComponent()
{
}
