#pragma once

#include "core\ExportMacro.h"
#include "core\Subsystem.h"

#include <glm\common.hpp>
#include <GL\glew.h>
#include <glfw\glfw3.h>

struct GLFWwindow;

namespace Horizon
{

	class HORIZON_API InputManager : public Subsystem<InputManager>
	{
	public:
		bool Init();
		void Shutdown();
		
		void PreFrameUpdate();
		void PostFrameUpdate();

		bool GetKeyDown(int keyCode);
		bool GetMouseButtonDown(int mouseButton);

		glm::vec2 GetMouseDelta();

		float GetScrollOffset();
		void SetCurrentScrollOffset(float yOffset);

		float GetMouseDeltaX();
		float GetMouseDeltaY();

		glm::vec2 GetMousePosition();

		void SetPreviousMousePosition(glm::vec2 vec);
		void SetCurrentMousePosition(glm::vec2 vec);
	private:
		glm::vec2 m_currentMousePosition;
		glm::vec2 m_previousMousePosition;

		float m_scrollOffset;

	};
}