#include "ComponentFactory.h"

#include "core\ExportMacro.h"
#include "subsystems\VirtualFileSystem.h"
#include "components\ComponentCreationMappings.h"
#include "utilities\Logger.h"
#include "utilities\IOUtility.h"

#include <string>
#include <nlohmann\json.hpp>

Horizon::ComponentFactory::ComponentFactory()
{}

bool Horizon::ComponentFactory::Init()
{
	if (!IOUtility::FileExists("~/component_index.json"))
	{
		std::string assetsDir = VirtualFileSystem::GetInstance()->ResolveFilePath("~/");

		Logger::GetInstance()->LogError("Component Index not found. Please ensure that a file called component_index.json is present in the directory: " + assetsDir);
		return false;
	}

	std::string componentIndexJson = IOUtility::ReadFileToEnd("~/component_index.json");

	nlohmann::json doc = nlohmann::json::parse(componentIndexJson);

	const auto& components = doc["components"];
	assert(components.is_array());
	size_t count = components.size();

	for (auto it = components.begin(); it != components.end(); ++it)
	{
		const auto& componentObject = (*it);

		assert(componentObject.is_object());
		assert(IOUtility::ContainsMember(componentObject, "id"));
		assert(IOUtility::ContainsMember(componentObject, "name"));

		uint32 id = componentObject["id"].get<uint32>();
		std::string name = componentObject["name"].get<std::string>();

		m_idNameMap[name] = id;
	}

	RegisterComponentCreateFunctions();
	return true;
}

void Horizon::ComponentFactory::Shutdown()
{

}

Horizon::Component* Horizon::ComponentFactory::CreateComponent(const std::string& componentName, const std::unordered_map<std::string, nlohmann::json::value_type>& componentAttributes, uint32 resourceSetId)
{
	if (m_idNameMap.count(componentName) == 0)
	{
		Logger::GetInstance()->LogError("Failed to create component with name " + componentName + std::string(" was not present in the component index"));
		return nullptr;
	}

	const uint32& componentTypeId = m_idNameMap[componentName];

	if (m_mappedComponents.count(componentTypeId) == 0)
	{
		Logger::GetInstance()->LogError("Failed to create component with name " + componentName + std::string(" a function hasn't been mapped."));
		return nullptr;
	}

	Component* newComponent = m_mappedComponents[componentTypeId](componentAttributes);
	newComponent->resourceSetId = resourceSetId;

	m_components.push_back(newComponent);

	return newComponent;
}

void Horizon::ComponentFactory::CreateComponentNameMapping(const std::string& name, std::function<Component*(std::unordered_map<std::string, nlohmann::json::value_type>)> creationFunction)
{
	if (m_idNameMap.count(name) == 0)
	{
		Logger::GetInstance()->LogError("Failed to create Component name mapping. Component name " + name + std::string(" was not present in the component index"));
		return;
	}

	const uint32& componentTypeId = m_idNameMap[name];

	if (m_mappedComponents.count(componentTypeId) > 0)
	{
		Logger::GetInstance()->LogWarning("Did not create Component name mapping. Component name " + name + std::string(" a mapping already exists."));
		return;
	}

	m_mappedComponents[componentTypeId] = creationFunction;
}
