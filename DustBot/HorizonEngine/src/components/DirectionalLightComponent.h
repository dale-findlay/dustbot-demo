#pragma once

#include "core\ExportMacro.h"
#include "components\LightComponent.h"

#include <glm\glm.hpp>

namespace Horizon
{
	class HORIZON_API DirectionalLightComponent : public LightComponent
	{
	public:
		DirectionalLightComponent();
	};
}
