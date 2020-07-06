#pragma once

#include "core\ExportMacro.h"
#include "core\Subsystem.h"
#include "structures\Scene.h"
#include "structures\SceneLoadData.h"
#include "structures\ResourceSet.h"
#include "structures\JsonValue.h"

#include <string>
#include <vector>
#include <mutex>
#include <nlohmann\json_fwd.hpp>

namespace Horizon
{
	class GameObject;

	class HORIZON_API SceneManager : public Subsystem<SceneManager>
	{
	public:
		SceneManager();
		~SceneManager();

		uint32 GetCurrentSceneResourceSetId() const;

		bool Init(const std::string& startScene);
		bool LoadScene(const std::string& sceneName);
		bool LoadScene(const uint32& sceneId);

		Scene* GetSceneInfo(const uint32& sceneId);

		SceneLoadData GetSceneLoadData() const;

		uint32 GetStartupSceneId() const;
		void UpdateScene();
		void Shutdown();

		bool IsLoading() const;

		void FinishedLoadingScene();

		void UnloadCurrentScene();

		void AddGameObject(GameObject* gameObject);

		const std::vector<GameObject*>& GetObjects();
		GameObject* GetGameObject(const uint32& gameObjectId);
		GameObject* GetGameObject(const std::string& name);

	private:
		const uint32 GetSceneId(const std::string& sceneName);

		bool m_sceneActive;

		bool m_loading;
		SceneLoadData m_sceneLoadData;

		uint32 m_currentLoadingSceneResourceSet;

		uint32 m_startSceneId;

		Scene* m_previousScene;
		Scene* m_currentScene;

		std::vector<GameObject*> m_gameObjects;
		std::vector<Scene> m_sceneIndex;

		std::mutex m_loadingMutex;
		std::mutex m_sceneObjectMutex;
	};
}
