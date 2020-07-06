#pragma once

#include "core\ExportMacro.h"

#include <string>
#include <vector>

namespace Horizon
{
	class HORIZON_API StringTools
	{
	public:
		static std::string ReplaceAll(const std::string& str, const std::string& a, const std::string& b);
		static std::string ToLower(std::string& str);
		static std::string ToUpper(std::string& str);
		static bool StartsWith(const std::string& str, const std::string& a);
		static bool EndsWith(const std::string& str, const std::string& a);
		static bool Contains(const std::string& string, const std::string& search);
		static std::vector<std::string> Split(const std::string& string, const std::string& delimiter);

	};
}
