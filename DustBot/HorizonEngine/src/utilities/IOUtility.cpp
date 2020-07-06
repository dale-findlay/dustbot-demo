#include "IOUtility.h"

#include "subsystems/VirtualFileSystem.h"
#include "utilities/StringTools.h"
#include "utilities/Logger.h"

#include <assert.h>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <nlohmann/json.hpp>

std::string Horizon::IOUtility::ReadFileToEnd(const std::string& fileName)
{
	std::string resolvedFileName = VirtualFileSystem::GetInstance()->ResolveFilePath(fileName);

	std::ifstream stream(resolvedFileName);

	if (stream.is_open())
	{
		std::stringstream buffer;
		buffer << stream.rdbuf();
		return buffer.str();
	}
	else
	{
		Logger::GetInstance()->LogError("Failed to open file stream on file path: " + resolvedFileName);
		return "";
	}
}

std::string Horizon::IOUtility::GetFilePath(const std::string & filePath)
{
	std::experimental::filesystem::path p(filePath);
	std::experimental::filesystem::path dir = p.parent_path();

	return dir.string();
}

bool Horizon::IOUtility::FileExists(const std::string & filePath)
{
	std::string resolvedFileName = VirtualFileSystem::GetInstance()->ResolveFilePath(filePath);

	return std::experimental::filesystem::exists(resolvedFileName);
}

bool Horizon::IOUtility::ContainsMember(const nlohmann::json::value_type & val, const std::string & memberName)
{
	assert(val.is_object());
	bool containsMember = val.count(memberName) > 0;
	return containsMember;
}