#pragma once

#include "core\ExportMacro.h"
#include "core\TypeAbstractions.h"

#include <string>
#include <functional>

namespace Horizon
{
	struct HORIZON_API  ResourceSet
	{
		uint32 id;
		std::string name;
		std::function<void(void)> completeCallback;
	};
}
