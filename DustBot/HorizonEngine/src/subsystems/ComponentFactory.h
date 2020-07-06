#pragma once

#include "core\ExportMacro.h"
#include "core\Subsystem.h"
#include "components\Component.h"
#include "structures\JsonValue.h"

#include <vector>
#include <unordered_map>
#include <functional>
#include <nlohmann\json.hpp>

namespace Horizon
{
	class Component;

	class HORIZON_API ComponentFactory : public Subsystem<ComponentFactory>
	{
	public:
		ComponentFactory();

		bool Init();
		void Shutdown();

		Component* CreateComponent(const std::string& componentName, const std::unordered_map<std::string, nlohmann::json::value_type>& componentAttributes, uint32 resourceSetId);

		void CreateComponentNameMapping(const std::string& name, std::function<Component*(std::unordered_map<std::string, nlohmann::json::value_type>)> creationFunction);

	private:
		std::unordered_map<std::string, uint32> m_idNameMap;
		std::unordered_map<uint32, std::function<Component*(std::unordered_map<std::string, nlohmann::json::value_type>)>> m_mappedComponents;

		std::vector<Component*> m_components;
	};
}
