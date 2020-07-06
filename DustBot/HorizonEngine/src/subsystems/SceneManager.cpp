#include "SceneManager.h"

#include "core\GameObject.h"
#include "subsystems\EngineAllocator.h"
#include "subsystems\VirtualFileSystem.h"
#include "subsystems\GameObjectFactory.h"
#include "subsystems\ComponentFactory.h"
#include "subsystems\ScriptingRuntime.h"
#include "subsystems\ResourceManager.h"
#include "subsystems\CameraManager.h"
#include "subsystems\LightManager.h"
#include "utilities\IOUtility.h"
#include "utilities\Logger.h"
#include "structures\SceneLoadData.h"
#include "components\CameraComponent.h"
#include "components\LightComponent.h"

#include <mutex>
#include <nlohmann\json.hpp>

Horizon::SceneManager::SceneManager()
	: m_previousScene(nullptr), m_currentScene(nullptr), m_sceneActive(false)
{}

Horizon::SceneManager::~SceneManager()
{}

Horizon::uint32 Horizon::SceneManager::GetCurrentSceneResourceSetId() const
{
	return m_currentLoadingSceneResourceSet;
}

bool Horizon::SceneManager::Init(const std::string& startScene)
{
	m_loading = false;

	if (!IOUtility::FileExists("~/scene_index.json"))
	{
		std::string assetsDir = VirtualFileSystem::GetInstance()->ResolveFilePath("~/");

		Logger::GetInstance()->LogError("Scene Index not found. Please ensure that a file called scene_index.json is present in the directory: " + assetsDir);
		return false;
	}

	std::string sceneIndexText = IOUtility::ReadFileToEnd("~/scene_index.json");

	nlohmann::json doc = nlohmann::json::parse(sceneIndexText);

	auto scenes = doc["scenes"];

	assert(scenes.is_array());

	size_t count = scenes.size();

	bool startSceneFound = false;
	uint32 startSceneId = 0;
	m_sceneIndex.reserve(count);

	for (auto it = scenes.begin(); it != scenes.end(); ++it)
	{
		const nlohmann::json::value_type& sceneObject = (*it);

		assert(sceneObject.is_object());
		assert(sceneObject["id"] != NULL);
		assert(sceneObject["name"] != NULL);
		assert(sceneObject["filePath"] != NULL);
		assert(sceneObject["friendlyName"] != NULL);

		uint32 sceneId = sceneObject["id"].get<uint32>();
		std::string sceneName = sceneObject["name"].get<std::string>();
		std::string filePath = sceneObject["filePath"].get<std::string>();
		std::string friendlyName = sceneObject["friendlyName"].get<std::string>();

		if (sceneName == startScene)
		{
			startSceneFound = true;
			startSceneId = sceneId;
		}

		m_sceneIndex.emplace_back(sceneId, sceneName, filePath, friendlyName);
	}

	m_startSceneId = startSceneId;

	if (count > 0 && m_sceneIndex.size() > 0)
	{
		if (!startSceneFound)
		{
			m_startSceneId = (m_sceneIndex.begin())->id;
		}
	}

	return true;
}

bool Horizon::SceneManager::LoadScene(const std::string& sceneName)
{
	return LoadScene(GetSceneId(sceneName));
}

bool Horizon::SceneManager::LoadScene(const uint32& sceneId)
{
	if (m_sceneActive)
	{
		UnloadCurrentScene();
	}

	Scene* scene = GetSceneInfo(sceneId);

	m_sceneLoadData = SceneLoadData();
	m_sceneLoadData.sceneName = scene->friendlyName;

	if (scene == nullptr)
	{
		Logger::GetInstance()->LogError("Failed to load scene with id: " + std::to_string(sceneId) + " it was not present in the scene_index");
		return false;
	}

	std::string sceneName = scene->name;

	if (!IOUtility::FileExists(scene->filePath))
	{
		Logger::GetInstance()->LogError("Failed to load scene with name: " + scene->name + " the file path: " + scene->filePath + " does not exist.");
		return false;
	}

	std::string sceneTextJson = IOUtility::ReadFileToEnd(scene->filePath);

		m_currentLoadingSceneResourceSet = ResourceManager::GetInstance()->CreateResourceSet("", std::bind(std::mem_fn(&SceneManager::FinishedLoadingScene), this));

	nlohmann::json doc = nlohmann::json::parse(sceneTextJson);

	assert(IOUtility::ContainsMember(doc, "objects"));
	const auto& objects = doc["objects"];
	assert(objects.is_array());
	size_t objectCount = objects.size();

	CameraComponent* firstCamera = nullptr;

	for (auto it = objects.begin(); it != objects.end(); ++it)
	{
		const auto& object = (*it);
		assert(IOUtility::ContainsMember(object, "name"));
		assert(IOUtility::ContainsMember(object, "transform"));
		assert(IOUtility::ContainsMember(object, "components"));

		std::string name = object["name"].get<std::string>();

		const auto& transform = object["transform"];
		assert(IOUtility::ContainsMember(transform, "position"));
		assert(IOUtility::ContainsMember(transform, "eulerRotation"));
		assert(IOUtility::ContainsMember(transform, "scale"));

		const auto& position = transform["position"];
		const auto& eulerRotation = transform["eulerRotation"];
		const auto& scale = transform["scale"];

		GameObject* newObject = GameObjectFactory::GetInstance()->CreateNewGameObject(name);
		newObject->transform.SetPosition(glm::vec3(position["x"].get<float>(), position["y"].get<float>(), position["z"].get<float>()));
		newObject->transform.SetEulerRotation(glm::vec3(eulerRotation["x"].get<float>(), eulerRotation["y"].get<float>(), eulerRotation["z"].get<float>()));
		newObject->transform.SetScale(glm::vec3(scale["x"].get<float>(), scale["y"].get<float>(), scale["z"].get<float>()));

		if (IOUtility::ContainsMember(object, "scripts"))
		{
			const auto& scripts = object["scripts"];
			for (auto scriptIterator = scripts.begin(); scriptIterator != scripts.end(); ++scriptIterator)
			{
				const auto& script = (*scriptIterator);
				
				std::string name = script["scriptName"];
				
				std::unordered_map<std::string, std::string> customAttributesMap;

				auto& customAttributes = script["customAttributes"];
				for (auto customAttributesIterator = customAttributes.begin(); customAttributesIterator != customAttributes.end(); customAttributesIterator++)
				{
					const auto& customAttribute = (*customAttributesIterator);
					
					std::string attributeName = customAttribute["name"];
					std::string attributeValue = customAttribute["value"];

					customAttributesMap[attributeName] = attributeValue;
				}

				ScriptingRuntime::GetInstance()->AttachScript(name, customAttributesMap, newObject);
			}
		}

		const auto& components = object["components"];
		assert(components.is_array());

		for (auto componentIterator = components.begin(); componentIterator != components.end(); ++componentIterator)
		{
			const auto& component = *componentIterator;
			assert(IOUtility::ContainsMember(component, "componentName"));

			std::string componentName = component["componentName"].get<std::string>();

			std::unordered_map < std::string, nlohmann::json::value_type> componentAttributes;

			for (auto componentAttributeIterator = component.begin(); componentAttributeIterator != component.end(); ++componentAttributeIterator)
			{
				componentAttributes[componentAttributeIterator.key()] = componentAttributeIterator.value();
			}

			componentAttributes.erase("componentName");

			Component* newComponent = ComponentFactory::GetInstance()->CreateComponent(componentName, componentAttributes, m_currentLoadingSceneResourceSet);
			newObject->AddComponent(newComponent);
			newComponent->CreateResources();

			if (componentName == "CameraComponent" && firstCamera == nullptr)
			{
				firstCamera = reinterpret_cast<CameraComponent*>(newComponent);
			}

			LightComponent* lightComp = dynamic_cast<LightComponent*>(newComponent);

			if (lightComp != nullptr)
			{
				LightManager::GetInstance()->AddLightComponent(lightComp);
			}

			m_sceneLoadData.sceneResourceCount += newComponent->GetResourceCount();
		}

		m_sceneObjectMutex.lock();
		m_gameObjects.push_back(newObject);
		m_sceneObjectMutex.unlock();

	}

	if (firstCamera == nullptr)
	{
		Logger::GetInstance()->LogWarning("Loaded scene without a camera being present in the scene.");
	}

	CameraManager::GetInstance()->SetActiveCameraComponent(firstCamera);

	m_sceneActive = true;
	m_loading = true;

	return true;
}

Horizon::Scene* Horizon::SceneManager::GetSceneInfo(const uint32& sceneId)
{
	for (auto it = m_sceneIndex.begin(); it != m_sceneIndex.end(); ++it)
	{
		if (it->id == sceneId)
		{
			return &(*it);
		}
	}

	return nullptr;
}

Horizon::SceneLoadData Horizon::SceneManager::GetSceneLoadData() const
{
	return m_sceneLoadData;
}

Horizon::uint32 Horizon::SceneManager::GetStartupSceneId() const
{
	return m_startSceneId;
}

void Horizon::SceneManager::UpdateScene()
{
	std::vector<GameObject*> objects = SceneManager::GetInstance()->GetObjects();

	for (auto it = objects.begin(); it != objects.end(); ++it)
	{
		GameObject* object = (*it);
		object->Update();
	}
}

void Horizon::SceneManager::Shutdown()
{

}

bool Horizon::SceneManager::IsLoading() const
{
	return m_loading || ResourceManager::GetInstance()->GetIncompleteResourcesCount() != 0;
}

void Horizon::SceneManager::FinishedLoadingScene()
{
	m_loadingMutex.lock();
	m_loading = false;
	m_loadingMutex.unlock();

	m_sceneObjectMutex.lock();
	for (auto it = m_gameObjects.begin(); it != m_gameObjects.end(); ++it)
	{
		(*it)->Start();
	}
	m_sceneObjectMutex.unlock();
}

void Horizon::SceneManager::UnloadCurrentScene()
{
	EngineAllocator::GetInstance()->Reset();
}

void Horizon::SceneManager::AddGameObject(GameObject* gameObject)
{
	m_gameObjects.push_back(gameObject);
}

const std::vector<Horizon::GameObject*>& Horizon::SceneManager::GetObjects()
{
	return m_gameObjects;
}

Horizon::GameObject* Horizon::SceneManager::GetGameObject(const uint32& gameObjectId)
{
	GameObject* foundGameObject = nullptr;
	m_sceneObjectMutex.lock();
	for (auto it = m_gameObjects.begin(); it != m_gameObjects.end(); ++it)
	{
		if ((*it)->GetId() == gameObjectId)
		{
			foundGameObject = (*it);
			break;
		}
	}
	m_sceneObjectMutex.unlock();

	return foundGameObject;
}

Horizon::GameObject* Horizon::SceneManager::GetGameObject(const std::string& name)
{
	GameObject* foundGameObject = nullptr;
	m_sceneObjectMutex.lock();
	for (auto it = m_gameObjects.begin(); it != m_gameObjects.end(); ++it)
	{
		if ((*it)->GetName() == name)
		{
			foundGameObject = (*it);
			break;
		}
	}
	m_sceneObjectMutex.unlock();

	return foundGameObject;
}

const Horizon::uint32 Horizon::SceneManager::GetSceneId(const std::string & sceneName)
{
	for (auto it = m_sceneIndex.begin(); it != m_sceneIndex.end(); ++it)
	{
		if (it->name == sceneName)
		{
			return (*it).id;
		}
	}

	Logger::GetInstance()->LogWarning("Failed to find scene id with scene name: " + sceneName);

	return 0;
}
