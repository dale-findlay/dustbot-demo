#include "Game.h"

#include "core\Timer.h"
#include "utilities\Logger.h"
#include "utilities\IOUtility.h"
#include "components\ComponentCreationMappings.h"

#include "platform\IMGUI\IMGUI.h"
#include "platform\IMGUI\IMGUIImplementation.h"

#include <iostream>

Horizon::Game::Game()
	:currentSelectedGameObject(nullptr)
{
}

bool Horizon::Game::Init(const GameCreateInfo& gameCreateInfo)
{
	m_gameCreateInfo = gameCreateInfo;

	if (!InitializePreliminarySubsystems())
	{
		std::cout << "Failed to initialize a preliminary engine system." << std::endl;
		return false;
	}

	if (!InitializeContentManagementSubsystems())
	{
		std::cout << "Failed to initialize a content-management engine system." << std::endl;
		return false;
	}

	//Load the first scene in the game. (startup scene)
	uint32 startupSceneId = m_sceneManager->GetStartupSceneId();
	if (!m_sceneManager->LoadScene(startupSceneId))
	{
		m_logger->LogError("Failed to load startup scene. ID: " + std::to_string(startupSceneId));
		return false;
	}

	return true;
}


bool Horizon::Game::InitializePreliminarySubsystems()
{
	m_allocator = EngineAllocator::CreateInstance();
	//Allocate 10mb.
	if (!m_allocator->Init((uint64)pow(10, 7)))
	{
		std::cout << "[FATAL] Failed to initialize core engine allocator.";
		return false;
	}

	m_logger = Logger::CreateInstance();
	if (!m_logger->Init())
	{
		std::cout << "[FATAL] Failed to initialize Logger.";
		return false;
	}

	m_timer = Timer::CreateInstance();
	if (!m_timer->Init())
	{
		m_logger->LogError("Failed to initialize Timer");
		return false;
	}

	m_virtualFileSystem = VirtualFileSystem::CreateInstance();
	if (!m_virtualFileSystem->Init(m_gameCreateInfo.mountedDirectories))
	{
		m_logger->LogError("Failed to initialize VirtualFileSystem");
		return false;
	}

	if (!ValidateCoreEngineResources())
	{
		m_logger->LogError("Failed to validate core engine resources");
		return false;
	}

	m_scriptingRuntime = ScriptingRuntime::CreateInstance();
	if (!m_scriptingRuntime->Init(m_gameCreateInfo.dependentAssembliesDefinitions))
	{
		m_logger->LogError("Failed to initialize mono scripting runtime.");
		return false;
	}

	return true;
}

bool Horizon::Game::InitializeContentManagementSubsystems()
{
	m_resourceManager = ResourceManager::CreateInstance();
	if (!m_resourceManager->Init())
	{
		m_logger->LogError("Failed to initialize ResourceManager");
		return false;
	}

	m_windowManager = WindowManager::CreateInstance();
	if (!m_windowManager->Init(m_gameCreateInfo.windowWidth, m_gameCreateInfo.windowHeight, m_gameCreateInfo.name))
	{
		m_logger->LogError("Failed to initialize WindowManager");
		return false;
	}

	m_inputManager = InputManager::CreateInstance();
	if (!m_inputManager->Init())
	{
		m_logger->LogError("Failed to initialize WindowManager");
		return false;
	}

	m_renderer = Renderer::CreateInstance();
	if (!m_renderer->Init())
	{
		m_logger->LogError("Failed to initialize Renderer");
		return false;
	}

	m_cameraManager = CameraManager::CreateInstance();
	if (!m_cameraManager->Init())
	{
		m_logger->LogError("Failed to initialize CameraManager");
		return false;
	}

	m_lightManager = LightManager::CreateInstance();
	if (!m_lightManager->Init())
	{
		m_logger->LogError("Failed to initialize LightManager");
		return false;
	}

	m_materialFactory = MaterialFactory::CreateInstance();
	if (!m_materialFactory->Init())
	{
		m_logger->LogError("Failed to initialize MaterialFactory");
		return false;
	}

	m_gameObjectFactory = GameObjectFactory::CreateInstance();
	if (!m_gameObjectFactory->Init())
	{
		m_logger->LogError("Failed to initialize GameObjectFactory");
		return false;
	}

	m_componentFactory = ComponentFactory::CreateInstance();
	if (!m_componentFactory->Init())
	{
		m_logger->LogError("Failed to initialize ComponentFactory");
		return false;
	}

	m_sceneManager = SceneManager::CreateInstance();
	if (!m_sceneManager->Init(m_gameCreateInfo.startScene))
	{
		m_logger->LogError("Failed to initialize SceneManager");
		return false;
	}

	return true;
}

bool Horizon::Game::ValidateCoreEngineResources()
{
	if (!IOUtility::FileExists("~/core_resources.json"))
	{
		std::string assetsDir = m_virtualFileSystem->ResolveFilePath("~/");

		m_logger->LogError("Core Resource List could not found. Please ensure that a file called core_resources.json is present in the directory: " + assetsDir);
		return false;
	}

	std::string coreResourcesJson = IOUtility::ReadFileToEnd("~/core_resources.json");

	nlohmann::json doc = nlohmann::json::parse(coreResourcesJson);

	std::vector<std::string> engineResources = doc["coreResources"];

	for (auto resourceIt = engineResources.begin(); resourceIt != engineResources.end(); ++resourceIt)
	{
		if (!IOUtility::FileExists(*resourceIt))
		{
			m_logger->LogWarning("[WARN] Missing Core Engine Resource:  " + *resourceIt);
		}
	}

	return true;
}

void Horizon::Game::Shutdown()
{
	m_sceneManager->Shutdown();
	if (m_sceneManager != nullptr)
	{
		delete m_sceneManager;
	}

	m_componentFactory->Shutdown();
	if (m_componentFactory != nullptr)
	{
		delete m_componentFactory;
	}


	m_gameObjectFactory->Shutdown();
	if (m_gameObjectFactory != nullptr)
	{
		delete m_gameObjectFactory;
	}

	m_cameraManager->Shutdown();
	if (m_cameraManager != nullptr)
	{
		delete m_cameraManager;
	}

	m_materialFactory->Shutdown();
	if (m_materialFactory != nullptr)
	{
		delete m_materialFactory;
	}

	m_renderer->Shutdown();
	if (m_renderer != nullptr)
	{
		delete m_renderer;
	}

	m_inputManager->Shutdown();
	if (m_inputManager != nullptr)
	{
		delete m_inputManager;
	}

	m_resourceManager->Shutdown();
	if (m_resourceManager != nullptr)
	{
		delete m_resourceManager;
	}

	m_windowManager->Shutdown();
	if (m_windowManager != nullptr)
	{
		delete m_windowManager;
	}

	m_scriptingRuntime->Shutdown();
	if (m_scriptingRuntime != nullptr)
	{
		delete m_scriptingRuntime;
	}

	m_virtualFileSystem->Shutdown();
	if (m_virtualFileSystem != nullptr)
	{
		delete m_virtualFileSystem;
	}

	m_timer->Shutdown();
	if (m_timer != nullptr)
	{
		delete m_timer;
	}

	m_logger->Shutdown();
	if (m_logger != nullptr)
	{
		delete m_logger;
	}

	m_allocator->Shutdown();
	if (m_allocator != nullptr)
	{
		delete m_allocator;
	}
}

int Horizon::Game::Run()
{
	bool shouldClose = false;

	bool scriptingRuntimeStart = false;


	while (shouldClose == false)
	{
		UpdateFramebufferSize();
		ImGui_ImplGlfwGL3_NewFrame();

		m_timer->PreUpdateTimer();
		m_inputManager->PreFrameUpdate();
		m_sceneManager->UpdateScene();

		if (!m_sceneManager->IsLoading())
		{
			if (!scriptingRuntimeStart)
			{
				m_scriptingRuntime->Start();
				scriptingRuntimeStart = true;
			}
			
			m_scriptingRuntime->Update();
		}
		
		m_renderer->Draw();
		m_timer->PostUpdateTimer();
		m_inputManager->PostFrameUpdate();

		int state = glfwGetKey(m_windowManager->GetGLFWWindow(), GLFW_KEY_R);
		if (state == GLFW_PRESS)
		{
			m_materialFactory->ReloadTrackedShaders();
		}

		if (true) {
			ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
			ImGui::Begin("GameObject Management");

			auto gameObjects = m_sceneManager->GetObjects();
			ImGui::BeginChild("Objects", ImVec2(150, 0), true);
			for (int i = 0; i < gameObjects.size(); ++i)
			{
				if (ImGui::Selectable(gameObjects[i]->GetName().c_str(), gameObjects[i] == currentSelectedGameObject))
				{
					currentSelectedGameObject = gameObjects[i];
				}
			}

			ImGui::EndChild();
			ImGui::SameLine();

			if (currentSelectedGameObject != nullptr)
			{
				ImGui::BeginGroup();
				ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
				ImGui::Text(currentSelectedGameObject->GetName().c_str());
				ImGui::Separator();

				glm::vec3 position = currentSelectedGameObject->transform.Position();
				ImGui::InputFloat3("Position", &position[0]);
				currentSelectedGameObject->transform.SetPosition(position);

				glm::quat rotation = currentSelectedGameObject->transform.Rotation();
				ImGui::InputFloat4("Rotation", &rotation[0]);
				currentSelectedGameObject->transform.SetRotation(rotation);

				glm::vec3 eulersAngles = currentSelectedGameObject->transform.EulerRotation();
				ImGui::InputFloat3("Euler rotation (degrees)", &eulersAngles[0]);
				currentSelectedGameObject->transform.SetEulerRotation(eulersAngles);

				glm::vec3 scale = currentSelectedGameObject->transform.Scale();
				ImGui::InputFloat3("Scale", &scale[0]);
				currentSelectedGameObject->transform.SetScale(scale);

				bool visible = currentSelectedGameObject->GetActive();
				ImGui::Checkbox("Visible", &visible);
				currentSelectedGameObject->SetActive(visible);

				ImGui::EndChild();
				ImGui::EndGroup();
			}

			ImGui::End();
		}

		DrawDebugFrameInfo();

		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(m_windowManager->GetGLFWWindow());

		m_windowManager->Update();

		shouldClose = glfwWindowShouldClose(m_windowManager->GetGLFWWindow());
	}

	return m_returnCode;
}

void Horizon::Game::UpdateFramebufferSize()
{
	glfwSwapInterval(1);
	int framebufferWidth, frameBufferHeight;
	glfwGetFramebufferSize(m_windowManager->GetGLFWWindow(), &framebufferWidth, &frameBufferHeight);
	glViewport(0, 0, framebufferWidth, frameBufferHeight);
}

void Horizon::Game::DrawDebugFrameInfo()
{
	float frameTime = m_timer->CalculateDeltaTime() * 1000.0f;
	uint32 fps = m_timer->CalculateFramesPerSecond();

	uint64 allocationCount = m_allocator->GetAllocationCount();
	uint64 totalBytesAllocated = m_allocator->GetBytesAllocated();
	uint64 initialAllocation = m_allocator->GetInitialAllocationSize();

	double totalBytesMb = (double)totalBytesAllocated / (double)1000000;
	double initalAllocMb = (double)initialAllocation / (double)1000000;

	std::string memoryStr = "Memory: " + std::to_string(totalBytesMb) + "/" + std::to_string(initalAllocMb) + " (MB) Alloc Count: " + std::to_string(allocationCount);
	
	//This is just to create an transparent window so you can draw some text.
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
	ImGui::SetNextWindowBgAlpha(0);
	ImGui::Begin("BCKGND", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus);

	std::string frameTimeStr = std::to_string(frameTime) + " ms";
	ImGui::GetWindowDrawList()->AddText(nullptr, 16, ImVec2(20, 20), ImColor(1.0f, 1.0f, 1.0f, 1.0f), frameTimeStr.c_str());
	std::string fpsStr = std::to_string(fps) + " FPS";
	ImGui::GetWindowDrawList()->AddText(nullptr, 15, ImVec2(20, 35), ImColor(1.0f, 1.0f, 1.0f, 1.0f), fpsStr.c_str());
	ImGui::GetWindowDrawList()->AddText(nullptr, 15, ImVec2(20, 50), ImColor(1.0f, 0.81f, 0.0f, 1.0f), memoryStr.c_str());

	ImGui::End();
}
