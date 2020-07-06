#pragma once

#include "core\ExportMacro.h"
#include "core\TypeAbstractions.h"
#include "structures\DependentAssemblyDefinition.h"

#include <string>
#include <vector>

namespace Horizon
{
	struct HORIZON_API  GameCreateInfo
	{
		std::string name;
		uint32 windowWidth;
		uint32 windowHeight;

		std::string startScene;

		std::vector<std::pair<std::string, std::string>> mountedDirectories;

		std::string rootNamespace;

		std::vector<Horizon::Scripting::DependentAssemblyDefinition> dependentAssembliesDefinitions;

	};
}
