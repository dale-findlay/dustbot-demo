#pragma once

#include "core\ExportMacro.h"
#include <string>

namespace Horizon
{
	namespace Managed
	{
		struct HORIZON_API  ScriptCustomAttributeManaged
		{
			std::string name;
			std::string value;
		};
	}
}