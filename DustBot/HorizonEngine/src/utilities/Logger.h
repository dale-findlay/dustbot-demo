#pragma once

#include "core\ExportMacro.h"
#include "core\Subsystem.h"

#include <string>
#include <exception>
#include <mutex>

namespace Horizon
{
	class HORIZON_API Logger : public Subsystem<Logger>
	{
	public:
		Logger();

		bool Init();
		void Shutdown();

		void Log(const std::string& message, bool newLine = true);
		void LogWarning(const std::string& message, bool newLine = true);
		void LogError(const std::string& message, bool newLine = true);

	private:
		void SetConsoleColor(int color);

		std::string GetSystemTime();

		void* m_hConsole;

		std::mutex m_mutex;
	};
}
