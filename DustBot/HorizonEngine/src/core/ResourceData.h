#pragma once

#include "core\ExportMacro.h"
#include "core\TypeAbstractions.h"

#include <string>
#include <unordered_map>
#include <nlohmann\json.hpp>

namespace Horizon
{
	/*
		
	*/
	struct HORIZON_API  ResourceData
	{
		std::string resourceId;
		uint32 resourceTypeId;
		std::string resourcePath;

		//attributes listed in the resource index.
		std::unordered_map<std::string, nlohmann::json::value_type> resourceAttributes;
	};
}
