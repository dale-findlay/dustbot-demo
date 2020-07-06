#pragma once

#include "core\ExportMacro.h"
#include "components\Component.h"

namespace Horizon
{	
	class HORIZON_API LightComponent : public Component
	{
		public:
			LightComponent(uint32 lightType);

			// Inherited via Component
			virtual bool CreateComponent(const std::unordered_map<std::string, nlohmann::json::value_type>& componentAttributes) override;
			virtual bool InitComponent() override;
			virtual void ShutdownComponent() override;
			virtual void StartComponent() override;
			virtual void UpdateComponent() override;
			virtual void CreateResources() override;

			uint32 GetLightType() const;
	protected:
		uint32 m_lightType;
	};
}
