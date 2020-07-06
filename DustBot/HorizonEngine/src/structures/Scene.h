#pragma once

#include "core\ExportMacro.h"
#include "core\TypeAbstractions.h"

#include <string>

namespace Horizon
{
	struct HORIZON_API  Scene
	{
		uint32 id;
		std::string name;
		std::string filePath;
		std::string friendlyName;

		Scene(const uint32& id, const std::string& name, const std::string& filePath, const std::string& friendlyName)
			: id(id), name(name), filePath(filePath), friendlyName(friendlyName)
		{}

	};
}
