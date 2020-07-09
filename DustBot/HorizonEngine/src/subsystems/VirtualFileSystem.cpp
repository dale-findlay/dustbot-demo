#include "VirtualFileSystem.h"
#include "utilities/Logger.h"
#include "utilities/StringTools.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <experimental/filesystem>

namespace filesystem = std::experimental::filesystem;

Horizon::VirtualFileSystem::VirtualFileSystem()
{
}

bool Horizon::VirtualFileSystem::Init(const std::vector<std::pair<std::string, std::string>>& mountedDirectories)
{
	bool result = true;
	for (auto it = mountedDirectories.begin(); it != mountedDirectories.end(); ++it)
	{
		const std::pair<std::string, std::string>& mountedDirectory = *it;
		result = MountDirectory(mountedDirectory.first, mountedDirectory.second);
	}

	std::string applicationDirectory = Horizon::VirtualFileSystem::GetApplicationDirectory();
	result = MountDirectory("~/", applicationDirectory + "\\GameAssets");

	if (!result)
	{
		Logger::GetInstance()->LogError("Failed to mount one or more directories.");
	}

	return result;
}

void Horizon::VirtualFileSystem::Shutdown()
{
}

bool Horizon::VirtualFileSystem::RealDirectoryExists(const std::string& directory)
{
	return filesystem::exists(filesystem::path(directory));
}

std::string Horizon::VirtualFileSystem::GetApplicationDirectory()
{
	char result[MAX_PATH];
	GetModuleFileName(nullptr, result, MAX_PATH);

	std::string returnStr = std::string(result);

	for (auto it = returnStr.rbegin(); it != returnStr.rend(); ++it)
	{
		if (*it == '\\')
		{
			returnStr.erase(it.base(), returnStr.end());
			break;
		}
	}

	if (StringTools::EndsWith(returnStr, "\\"))
	{
		returnStr.erase(returnStr.end() - 1);
	}



	return returnStr;
}

bool Horizon::VirtualFileSystem::MountDirectory(const std::string& virtualDirectory, const std::string& realDirectory)
{
	if (!StringTools::StartsWith(virtualDirectory, "~/"))
	{
		Logger::GetInstance()->LogError("Failed to mount virtual directory: " + virtualDirectory + std::string(". It does not begin with ~/."));
		return false;
	}

	if (m_mountedDirectories.count(virtualDirectory))
	{
		Logger::GetInstance()->LogWarning("Did not mount virtual directory: " + virtualDirectory + std::string(". it is already mounted"));
		return true;
	}

	if (!RealDirectoryExists(realDirectory))
	{
		Logger::GetInstance()->LogError("Failed to mount virtual directory: " +
			virtualDirectory + std::string(". Real directory:") + realDirectory + std::string("does not exists"));
		return false;
	}

	//Making sure that all of the paths in the mountedDirs are with forward slashes.
	std::string formattedRealDirectory = StringTools::ReplaceAll(realDirectory, "\\", "/");

	if (StringTools::EndsWith(formattedRealDirectory, "/"))
	{
		formattedRealDirectory.erase(formattedRealDirectory.end() - 1);
	}

	//format is virtual directory with no trailing slashes. formatted directory no trailing slashes.
	m_mountedDirectories[virtualDirectory] = formattedRealDirectory;

	Logger::GetInstance()->Log("Successfully Mounted directory: " + virtualDirectory);

	return true;
}

std::string Horizon::VirtualFileSystem::ResolveFilePath(const std::string& virtualFilePath)
{
	if (!StringTools::StartsWith(virtualFilePath, "~/"))
	{
		Logger::GetInstance()->LogError("Did not resolve virtual file path: " + virtualFilePath + std::string(". It does not begin with ~/. Path returned."));
		return virtualFilePath;
	}

	std::string virtualDirectory = "";

	for (auto it = m_mountedDirectories.begin(); it != m_mountedDirectories.end(); ++it)
	{
		const std::string& mountedDir = (it)->first;

		if (StringTools::Contains(virtualFilePath, mountedDir))
		{
			virtualDirectory = mountedDir;
			break;
		}
	}

	if (virtualDirectory == "")
	{
		Logger::GetInstance()->LogError("Failed to resolve virtual file path: " + virtualFilePath + std::string(". No matching virtual directories found."));
		return virtualDirectory;
	}

	std::string formattedVirtualFilePath = virtualFilePath;

	//this allows us to use our vfs like this ~/some_asset.txt.
	if (virtualDirectory == "~/")
	{
		formattedVirtualFilePath = StringTools::ReplaceAll(formattedVirtualFilePath, "~/", "");
		formattedVirtualFilePath = "/" + formattedVirtualFilePath;
		formattedVirtualFilePath = virtualDirectory + formattedVirtualFilePath;
	}

	std::string resolvedFilePath = StringTools::ReplaceAll(formattedVirtualFilePath, virtualDirectory, m_mountedDirectories[virtualDirectory]);

	return resolvedFilePath;
}
