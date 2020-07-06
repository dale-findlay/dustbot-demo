#pragma once

#include "core\ExportMacro.h"
#include "core\Timer.h"
#include "core\GameObject.h"
#include "subsystems\EngineAllocator.h"
#include "subsystems\VirtualFileSystem.h"
#include "subsystems\Renderer.h"
#include "subsystems\ScriptingRuntime.h"
#include "subsystems\WindowManager.h"
#include "subsystems\SceneManager.h"
#include "subsystems\GameObjectFactory.h"
#include "subsystems\ComponentFactory.h"
#include "subsystems\ResourceManager.h"
#include "subsystems\MaterialFactory.h"
#include "subsystems\CameraManager.h"
#include "subsystems\LightManager.h"
#include "subsystems\InputManager.h"
#include "game\GameCreateInfo.h"
#include "utilities\Logger.h"

namespace Horizon
{
	class HORIZON_API Game
	{
	public:
		Game();

		//Initialize game and engine systems & resources.
		bool Init(const GameCreateInfo& gameCreateInfo);

		//Shutdown game and engine systems & resource created/initialized in Init().
		void Shutdown();

		//Run the main game/render loop.
		int Run();

	private:
		//Used to initialize core engine systems that need to be initialized in order to initialize others. eg. Logger.
		bool InitializePreliminarySubsystems();

		//Used to initialize the 'content' part of the engine such as resource management & scene/gameobject management.
		bool InitializeContentManagementSubsystems();

		/*
			Validates that the 'default' resources the engine needs to function are present such as default shaders.
			NOTE: This will not load the resources however. They should be loaded by the subsystems that require them.
		*/
		bool ValidateCoreEngineResources();
		
		//Update the framebuffer size to the current window size.
		void UpdateFramebufferSize();

		//Draw frame information on screen.
		void DrawDebugFrameInfo();
		
		GameObject* currentSelectedGameObject;

		GameCreateInfo m_gameCreateInfo;
		short m_returnCode;

		EngineAllocator* m_allocator;
		Logger* m_logger;
		Timer* m_timer;
		VirtualFileSystem* m_virtualFileSystem;
		ScriptingRuntime* m_scriptingRuntime;
		ResourceManager* m_resourceManager;
		WindowManager* m_windowManager;
		InputManager* m_inputManager;
		CameraManager* m_cameraManager;
		LightManager* m_lightManager;
		SceneManager* m_sceneManager;
		MaterialFactory* m_materialFactory;
		Renderer* m_renderer;
		GameObjectFactory* m_gameObjectFactory;
		ComponentFactory* m_componentFactory;

	};
}