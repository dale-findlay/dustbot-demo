#pragma once

#include "core\ExportMacro.h"
#include "components\Component.h"

namespace Horizon
{
	class HORIZON_API ScriptComponent : public Component
	{
	public:
		ScriptComponent();
		bool CreateComponent(const std::unordered_map<std::string, nlohmann::json::value_type>& componentAttributes) override;
		bool InitComponent() override;
		void ShutdownComponent() override;
		void StartComponent() override;
		void UpdateComponent() override;
		void CreateResources() override;

		virtual void Start();
		virtual void Update();
		virtual void PostUpdate();
	};
}