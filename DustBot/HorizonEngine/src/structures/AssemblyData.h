#pragma once

#include "core\ExportMacro.h"

namespace Horizon
{
	struct HORIZON_API  AssemblyData
	{
		bool valid;
		size_t size;
		char* assemblyData;
	};
}
