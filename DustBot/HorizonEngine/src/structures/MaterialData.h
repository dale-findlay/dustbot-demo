#pragma once

#include "core\ExportMacro.h"
#include "core\TypeAbstractions.h"
#include "structures\MaterialTextureType.h"

#include <map>

namespace Horizon
{
	struct HORIZON_API  MaterialData
	{
		//Material Texture Type - File name (virtual)
		std::map<MaterialTextureType, std::string> materialTextures;
	};
}