#pragma once

#include "core\ExportMacro.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

namespace Horizon
{
	struct HORIZON_API  DependentAssembly
	{
		std::string filePath;
		MonoAssembly* assembly;
		MonoImage* image;
	};
}
