#pragma once

#include "core\ExportMacro.h"
#include "core\TypeAbstractions.h"
#include "core\Subsystem.h"
#include "game\GameCreateInfo.h"

#include <string>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

namespace Horizon
{
	//A class that encapsulates a GLFW window.
	class HORIZON_API WindowManager : public Subsystem<WindowManager>
	{
	public:
		WindowManager();

		bool Init(const uint32& windowWidth, const uint32& windowHeight, const std::string& gameName);
		void Update();		
		void Shutdown();

		uint32 GetWindowWidth() const;
		uint32 GetWindowHeight() const;

		//Returns the GLFWwindow*.
		GLFWwindow* GetGLFWWindow();
	
	private:
		bool InitWindow();
	
		uint32 m_windowWidth;
		uint32 m_windowHeight;
		std::string m_windowTitle;
		
		GLFWwindow* m_window;
	};
}
