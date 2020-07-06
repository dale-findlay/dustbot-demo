#pragma once

#include "core\ExportMacro.h"
#include "components\Component.h"

#include <nlohmann\json.hpp>

namespace Horizon
{
	class HORIZON_API DrawableComponent : public Component
	{
	public:
		DrawableComponent()
			:Component()
		{}

		// Inherited via Component
		virtual bool CreateComponent(const std::unordered_map<std::string, nlohmann::json::value_type>& componentAttributes) = 0;
		virtual bool InitComponent() = 0;

		virtual void StartComponent();
		virtual void UpdateComponent() = 0;
		virtual void ShutdownComponent() = 0;

		virtual void CreateResources() = 0;

	public:
		virtual void DrawComponent() = 0;
	};
}