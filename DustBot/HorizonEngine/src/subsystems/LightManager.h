#pragma once

#include "core\ExportMacro.h"
#include "core\Subsystem.h"
#include "components\LightComponent.h"
#include "platform\structures\UniformLight.h"

#include <vector>

#define MAX_LIGHT_COUNT 100

namespace Horizon
{
	class LightComponent;
	
	class HORIZON_API LightManager : public Subsystem<LightManager>
	{
	public:
		LightManager();

		bool Init();
		void Shutdown();

		void AddLightComponent(LightComponent* light);

		std::vector<LightComponent*> GetLightComponents() const;
		std::vector<Graphics::UniformLight> GetUniformLightData();
		
	private:
		void RegenerateLightDataCache();

		bool m_shouldRegenerateLightCache;

		std::vector<LightComponent*> m_lights;

		std::vector<Graphics::UniformLight> m_lightDataCache;

	};
}
