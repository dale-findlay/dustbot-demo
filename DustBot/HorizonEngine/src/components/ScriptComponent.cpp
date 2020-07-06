#include "ScriptComponent.h"

Horizon::ScriptComponent::ScriptComponent()
	:Component()
{
}

bool Horizon::ScriptComponent::CreateComponent(const std::unordered_map<std::string, nlohmann::json::value_type>& componentAttributes)
{
	return false;
}

bool Horizon::ScriptComponent::InitComponent()
{
	return false;
}

void Horizon::ScriptComponent::ShutdownComponent()
{
}

void Horizon::ScriptComponent::StartComponent()
{
}

void Horizon::ScriptComponent::UpdateComponent()
{
}

void Horizon::ScriptComponent::CreateResources()
{
}

void Horizon::ScriptComponent::Start()
{

}

void Horizon::ScriptComponent::Update()
{}

void Horizon::ScriptComponent::PostUpdate()
{
}
