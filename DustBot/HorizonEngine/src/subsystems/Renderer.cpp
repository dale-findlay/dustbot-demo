#include "Renderer.h"
#include "core\GameObject.h"
#include "structures\SceneLoadData.h"
#include "subsystems\ResourceManager.h"
#include "subsystems\CameraManager.h"
#include "subsystems\SceneManager.h"
#include "subsystems\WindowManager.h"
#include "components\DrawableComponent.h"
#include "components\CameraComponent.h"
#include "platform\IMGUI\IMGUI.h"
#include "platform\IMGUI\IMGUIImplementation.h"
#include "utilities\Logger.h"

#include "utilities\MathUtility.h"


#include <GL\glew.h>

bool Horizon::Renderer::Init()
{
	glfwMakeContextCurrent(WindowManager::GetInstance()->GetGLFWWindow());

	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		Logger::GetInstance()->LogError("Failed to initialized GLEW. Error " + std::to_string(glGetError()));
		return false;
	}

	//Set the GL clear color state.
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(WindowManager::GetInstance()->GetGLFWWindow(), true);

	ImGui::StyleColorsDark();

	return true;
}

void Horizon::Renderer::Shutdown()
{
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
}

void Horizon::Renderer::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (SceneManager::GetInstance()->IsLoading())
	{
		DrawLoadingScreen();
	}
	else
	{
		CameraComponent* activeCamera = CameraManager::GetInstance()->GetActiveCamera();

		if (activeCamera)
		{
			std::vector<GameObject*> objects = SceneManager::GetInstance()->GetObjects();

			for (auto it = objects.begin(); it != objects.end(); ++it)
			{
				GameObject* object = (*it);

				if (object->IsDrawable() && object->GetActive())
				{
					std::vector<DrawableComponent*> drawableComponents = object->GetComponents<DrawableComponent*>();

					for (auto drawableIt = drawableComponents.begin(); drawableIt != drawableComponents.end(); ++drawableIt)
					{
						(*drawableIt)->DrawComponent();
					}
				}
			}
		}
		else
		{
			DrawNoActiveCamera();
		}
	}
}

void Horizon::Renderer::DrawLoadingScreen()
{
	SceneLoadData sceneLoadData = SceneManager::GetInstance()->GetSceneLoadData();

	std::string loadingText = "Loading " + sceneLoadData.sceneName + "...";

	ImVec2 position = ImGui::GetIO().DisplaySize;
	ImVec2 textSize = ImGui::CalcTextSize(20, loadingText.c_str());
	position.x -= 200 + textSize.x;
	position.y -= 80 + textSize.y;

	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
	ImGui::SetNextWindowBgAlpha(0);
	ImGui::Begin("BCKGND", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus);
	ImGui::GetWindowDrawList()->AddText(nullptr, 20, position, ImColor(1.0f, 1.0f, 1.0f, 1.0f), loadingText.c_str());

	int resourceCount = ResourceManager::GetInstance()->GetIncompleteResourcesCount();

	float fraction = (float)(sceneLoadData.sceneResourceCount - resourceCount) / (float)sceneLoadData.sceneResourceCount;
	fraction = MathUtility::ClampF(fraction, 0, 1);

	ImVec2 progressBarPos = position;
	progressBarPos.y += 50;

	ImVec2 progressBarWidth = progressBarPos;
	progressBarWidth.x += 400;
	progressBarWidth.y += 30;

	ImGui::GetWindowDrawList()->AddRect(progressBarPos, progressBarWidth, ImColor(1.0f, 1.0f, 1.0f, 1.0f));

	ImVec2 progressBarFillWidth = progressBarWidth;
	progressBarWidth.x -= 400;
	progressBarWidth.x += 400 * fraction;

	ImGui::GetWindowDrawList()->AddRectFilled(progressBarPos, progressBarWidth, ImColor((float)120 / (float)255, (float)188 / (float)255, (float)97 / (float)255, 1.0f));

	//ImGui::ProgressBar(fraction);

	ImGui::End();
}

void Horizon::Renderer::DrawNoActiveCamera()
{
	ImVec2 displaySize = ImGui::GetIO().DisplaySize;

	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
	ImGui::SetNextWindowBgAlpha(0);
	ImGui::Begin("BCKGND", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus);
	ImGui::GetWindowDrawList()->AddText(nullptr, 20, ImVec2(20, displaySize.y - 40), ImColor(1.0f, 1.0f, 1.0f, 1.0f), "No Active Camera");

	ImGui::End();
}
