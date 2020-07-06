#include "WindowManager.h"

#include "utilities\Logger.h"

namespace Horizon
{
	void GLFWErrorCallback(int error, const char* description)
	{
		Logger::GetInstance()->LogError(std::to_string(error) + std::string(description));
	}
}

Horizon::WindowManager::WindowManager()
{}

bool Horizon::WindowManager::Init(const uint32& windowWidth, const uint32& windowHeight, const std::string & gameName)
{
	m_windowWidth = windowWidth;
	m_windowHeight = windowHeight;
	m_windowTitle = gameName;

	if (!InitWindow())
	{
		return false;
	}

	//glfwSwapInterval(1);

	return true;
}

void Horizon::WindowManager::Update()
{
	glfwPollEvents();
}

void Horizon::WindowManager::Shutdown()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

Horizon::uint32 Horizon::WindowManager::GetWindowWidth() const
{
	return m_windowWidth;
}

Horizon::uint32 Horizon::WindowManager::GetWindowHeight() const
{
	return m_windowHeight;
}

GLFWwindow* Horizon::WindowManager::GetGLFWWindow()
{
	return m_window;
}

bool Horizon::WindowManager::InitWindow()
{
	//Register the error callback.
	glfwSetErrorCallback(GLFWErrorCallback);

	if (glfwInit())
	{
		m_window = glfwCreateWindow(m_windowWidth, m_windowHeight,
			m_windowTitle.c_str(), nullptr, nullptr);

		if (!m_window)
		{
			Logger::GetInstance()->LogError("Failed to create GLFW window.");
			return false;
		}
	}
	else
	{
		Logger::GetInstance()->LogError("Failed to initialize GLFW.");
		return false;
	}

	return true;
}
