#include "InputManager.h"

#include "subsystems\WindowManager.h"

#include "platform\IMGUI\IMGUI.h"

static void CursorMoveCallback(GLFWwindow* window, double xpos, double ypos)
{
	Horizon::InputManager::GetInstance()->SetCurrentMousePosition(glm::vec2((float)xpos, (float)ypos));
}

bool Horizon::InputManager::Init()
{
	glfwSetCursorPosCallback(WindowManager::GetInstance()->GetGLFWWindow(), CursorMoveCallback);

	return true;
}

void Horizon::InputManager::Shutdown()
{
}

void Horizon::InputManager::PreFrameUpdate()
{
}

void Horizon::InputManager::PostFrameUpdate()
{
	Horizon::InputManager::GetInstance()->SetPreviousMousePosition(Horizon::InputManager::GetInstance()->GetMousePosition());
	m_scrollOffset = 0;
}

bool Horizon::InputManager::GetKeyDown(int keyCode)
{
	int state = glfwGetKey(WindowManager::GetInstance()->GetGLFWWindow(), keyCode);
	if (state == GLFW_PRESS)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Horizon::InputManager::GetMouseButtonDown(int mouseButton)
{
	return glfwGetMouseButton(WindowManager::GetInstance()->GetGLFWWindow(), mouseButton);
}

glm::vec2 Horizon::InputManager::GetMouseDelta()
{
	glm::vec2 mouseDelta = m_currentMousePosition - m_previousMousePosition;
	return mouseDelta;
}

float Horizon::InputManager::GetScrollOffset()
{
	//IMGui stole the callback so you cannot use conventional scroll callback...
	ImGuiIO& io = ImGui::GetIO();
	return io.MouseWheel;
}

void Horizon::InputManager::SetCurrentScrollOffset(float yOffset)
{
	m_scrollOffset = yOffset;
}

float Horizon::InputManager::GetMouseDeltaX()
{
	return GetMouseDelta().x;
}

float Horizon::InputManager::GetMouseDeltaY()
{
	return GetMouseDelta().y;
}

glm::vec2 Horizon::InputManager::GetMousePosition()
{
	return m_currentMousePosition;
}

void Horizon::InputManager::SetPreviousMousePosition(glm::vec2 vec)
{
	m_previousMousePosition = vec;
}

void Horizon::InputManager::SetCurrentMousePosition(glm::vec2 vec)
{
	m_currentMousePosition = vec;
}

