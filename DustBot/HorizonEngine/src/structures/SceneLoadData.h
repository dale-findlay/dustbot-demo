#pragma once

#include "core\ExportMacro.h"
#include "core\TypeAbstractions.h"

#include <string>

namespace Horizon
{
	struct HORIZON_API  SceneLoadData
	{
		std::string sceneName;
		uint32 sceneResourceCount;
	};
}
