#pragma once

#include "core\ExportMacro.h"
#include "core\Subsystem.h"

#include <string>
#include <unordered_map>
#include <vector>

namespace Horizon
{
	class HORIZON_API VirtualFileSystem : public Subsystem<VirtualFileSystem>
	{
	public:
		VirtualFileSystem();

		bool Init(const std::vector<std::pair<std::string, std::string>>& mountedDirectories);
		void Shutdown();

		//returns true if the directory exists.
		static bool RealDirectoryExists(const std::string& directory);

		//Returns the application's directory without trailing \\; seperated by \\.
		static std::string GetApplicationDirectory();

		/*
			Takes a virtual directory and a real direction and creates an one to one mapping of it.
			Use the ResolveFilePath function to resolve virtual file paths.
		*/
		bool MountDirectory(const std::string& virtualDirectory, const std::string& realDirectory);
		
		/*
			Resolves a virtual filepath that starts with ~/.
		*/
		std::string ResolveFilePath(const std::string& virtualFilePath);

	private:

		//Maps virtual directories (key) to real directories (value).
		std::unordered_map<std::string, std::string> m_mountedDirectories;

	};
}
