#pragma once

#include "core\ExportMacro.h"

#include <string>
#include <nlohmann/json.hpp>

namespace Horizon
{
	class HORIZON_API IOUtility
	{
	public:
		static std::string ReadFileToEnd(const std::string& filePath);

		//Returns only the file path.
		static std::string GetFilePath(const std::string& filePath);

		static bool FileExists(const std::string& filePath);
		static bool ContainsMember(const nlohmann::json::value_type& val, const std::string& memberName);
	};
}
