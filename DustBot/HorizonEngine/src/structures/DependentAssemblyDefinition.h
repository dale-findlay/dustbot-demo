#pragma once

#include "core\ExportMacro.h"

#include <string>
#include <vector>

namespace Horizon
{
	namespace Scripting
	{
		struct HORIZON_API  DependentAssemblyDefinition
		{
			std::string filePath;
			std::vector<std::string> namespaces;
			bool mainAssembly;

			DependentAssemblyDefinition(std::string filePath, std::vector<std::string> namespaces, bool mainAssembly)
			{
				this->filePath = filePath;
				this->namespaces = namespaces;
				this->mainAssembly = mainAssembly;
			}
		};
	}
}
