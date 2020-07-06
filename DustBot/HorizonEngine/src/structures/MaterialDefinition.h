#pragma once

#include "core\ExportMacro.h"
#include "core\TypeAbstractions.h"
#include "structures\MaterialCustomAttribute.h"

#include <string>
#include <unordered_map>

namespace Horizon
{
	/*
		Read from the material_index.json file.
		Used to define a base material from which new instances are created.
	*/
	struct HORIZON_API  MaterialDefinition
	{
		MaterialDefinition()
		{

		}

		MaterialDefinition(std::string id, uint32 materialTypeId, std::unordered_map<std::string, MaterialCustomAttribute> materialAttributes)
			: id(id), materialTypeId(materialTypeId), customAttributes(materialAttributes)
		{}

		std::string id;
		uint32 materialTypeId;
		std::unordered_map<std::string, MaterialCustomAttribute> customAttributes;
	};
}
