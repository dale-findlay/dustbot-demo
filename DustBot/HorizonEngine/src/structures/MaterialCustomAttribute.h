#pragma once

#include "core\ExportMacro.h"

#include <string>
#include <nlohmann/json.hpp>

namespace Horizon
{
	struct HORIZON_API  MaterialCustomAttribute
	{
		std::string name;
		std::string attributeType;
		nlohmann::json::value_type value;
	};
}
