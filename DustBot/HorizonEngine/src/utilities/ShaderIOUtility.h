#pragma once

#include "core\ExportMacro.h"
#include "core\TypeAbstractions.h"

#include <string>
#include <map>

namespace Horizon
{
	class HORIZON_API ShaderIOUtility
	{
	public:
		/*
			Returns the various source code parts specified in a shader file.
			Return type: map<shader type, source code>
		*/
		static std::map<std::string, std::string> GetShaderParts(const std::string& fileName);

	};
}
