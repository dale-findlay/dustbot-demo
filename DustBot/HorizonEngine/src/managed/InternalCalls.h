#pragma once

#include "core\Timer.h"
#include "core\GameObject.h"
#include "subsystems\EngineAllocator.h"
#include "subsystems\SceneManager.h"
#include "subsystems\InputManager.h"
#include "subsystems\GameObjectFactory.h"
#include "subsystems\VirtualFileSystem.h"
#include "managed\GameObjectManaged.h"
#include "managed\ComponentAttribute.h"

#include <string>
#include <locale>
#include <codecvt>
#include <nlohmann/json.hpp>

#pragma warning( push )
#pragma warning( disable : 4172)

//static int GetMonoArrayLength(MonoArray* arr)
//{
//	return *reinterpret_cast<uint32_t*>(12 + (char*)arr);
//}
//
//template<class T>
//T GetMonoArrayElement(MonoArray* arr, int index)
//{
//	return ()
//}

static std::string GetStringFromMonoString(MonoString* monoString)
{
	char* str = mono_string_to_utf8(monoString);
	return std::string(str);
}

static float Managed_CalculateDeltaTime()
{
	return Horizon::Timer::GetInstance()->CalculateDeltaTime();
}

static bool Managed_GetKeyDown(int key)
{
	return Horizon::InputManager::GetInstance()->GetKeyDown(key);
}

static bool Managed_GetMouseButtonDown(int button)
{
	return Horizon::InputManager::GetInstance()->GetMouseButtonDown(button);
}

static glm::vec2* Managed_GetMouseDelta()
{
	glm::vec2 move = Horizon::InputManager::GetInstance()->GetMouseDelta();
	return &move;
}

static float Managed_GetScrollOffset()
{
	return Horizon::InputManager::GetInstance()->GetScrollOffset();
}

static MonoString* Managed_ResolveFilePath(MonoString* virtualFilePath)
{
	std::string resolvedPath = Horizon::VirtualFileSystem::GetInstance()->ResolveFilePath(GetStringFromMonoString(virtualFilePath));
	MonoString* resolvedPathMonoString = mono_string_new(mono_domain_get(), resolvedPath.c_str());

	return resolvedPathMonoString;
}

static MonoString* Managed_GetScriptCustomAttribute(uint32_t scriptId, MonoString* customAttributeName)
{
	std::string customAttributeValue = Horizon::ScriptingRuntime::GetInstance()->GetScriptCustomAttribute(scriptId, GetStringFromMonoString(customAttributeName));
	MonoString* customAttributeValuePathMonoString = mono_string_new(mono_domain_get(), customAttributeValue.c_str());

	return customAttributeValuePathMonoString;
}

static Horizon::uint32 Managed_FindGameObject(uint32_t scriptId, MonoString* name)
{
	std::string nameStr = mono_string_to_utf8(name);
	Horizon::GameObject* gameObject = Horizon::SceneManager::GetInstance()->GetGameObject(nameStr);

	if (!gameObject)
	{
		return 0;
	}

	return gameObject->GetId();
}

static glm::vec4* Managed_GetPosition(uint32_t scriptId)
{
	Horizon::GameObject* gameObject = Horizon::SceneManager::GetInstance()->GetGameObject(scriptId);

	glm::vec4* v = nullptr;
	if (gameObject)
	{
		v = gameObject->transform.ManagedPosition();
	}

	return v;
}

static glm::vec4* Managed_GetEulerRotation(uint32_t scriptId)
{
	Horizon::GameObject* gameObject = Horizon::SceneManager::GetInstance()->GetGameObject(scriptId);

	glm::vec4* v = nullptr;
	if (gameObject)
	{
		v = gameObject->transform.ManagedEulerRotation();
	}

	return v;
}

static glm::vec4* Managed_GetScale(uint32_t scriptId)
{
	Horizon::GameObject* gameObject = Horizon::SceneManager::GetInstance()->GetGameObject(scriptId);

	glm::vec4* v = nullptr;
	if (gameObject)
	{
		v = gameObject->transform.ManagedScale();
	}

	return v;
}

static glm::vec4* Managed_GetForward(uint32_t scriptId)
{
	Horizon::GameObject* gameObject = Horizon::SceneManager::GetInstance()->GetGameObject(scriptId);

	glm::vec4* v = nullptr;
	if (gameObject)
	{
		v = gameObject->transform.ManagedForward();
	}

	return v;
}

static glm::vec4* Managed_GetRight(uint32_t scriptId)
{
	Horizon::GameObject* gameObject = Horizon::SceneManager::GetInstance()->GetGameObject(scriptId);

	glm::vec4 v(0.0f);
	if (gameObject)
	{
		v = glm::vec4(gameObject->transform.Right(), 1.0f);
	}

	return &v;
}


static glm::quat* Managed_GetRotation(uint32_t scriptId)
{
	Horizon::GameObject* gameObject = Horizon::SceneManager::GetInstance()->GetGameObject(scriptId);

	glm::quat* r = nullptr;
	if (gameObject)
	{
		r = gameObject->transform.ManagedRotation();
	}

	return r;
}

static void Managed_SetPosition(uint32_t scriptId, glm::vec4& position)
{
	Horizon::GameObject* gameObject = Horizon::SceneManager::GetInstance()->GetGameObject(scriptId);

	if (gameObject)
	{
		gameObject->transform.SetPosition(position);
	}
}

static void Managed_SetEulerRotation(uint32_t scriptId, glm::vec4& eulerRotation)
{
	Horizon::GameObject* gameObject = Horizon::SceneManager::GetInstance()->GetGameObject(scriptId);

	if (gameObject)
	{
		gameObject->transform.SetEulerRotation(eulerRotation);
	}
}

static void Managed_SetScale(uint32_t scriptId, glm::vec4& scale)
{
	Horizon::GameObject* gameObject = Horizon::SceneManager::GetInstance()->GetGameObject(scriptId);

	if (gameObject)
	{
		gameObject->transform.SetScale(scale);
	}
}

static void Managed_SetRotation(uint32_t scriptId, glm::quat& rotation)
{
	Horizon::GameObject* gameObject = Horizon::SceneManager::GetInstance()->GetGameObject(scriptId);

	if (gameObject)
	{
		gameObject->transform.SetRotation(rotation);
	}
}

static Horizon::uint32 Managed_CreateGameObject(MonoString* name)
{
	std::string nameStr = mono_string_to_utf8(name);

	Horizon::GameObject* gameObject = Horizon::GameObjectFactory::GetInstance()->CreateNewGameObject(nameStr);

	Horizon::SceneManager::GetInstance()->AddGameObject(gameObject);

	return gameObject->GetId();
}

static void Managed_AddComponent(uint32_t gameObjectId, MonoString* componentName, MonoArray* componentAttributes)
{
	Horizon::GameObject* gameObject = Horizon::SceneManager::GetInstance()->GetGameObject(gameObjectId);

	if (gameObject)
	{
		std::string componentNameStr = mono_string_to_utf8(componentName);

		std::unordered_map<std::string, nlohmann::json::value_type> componentJsonAttributes;

		nlohmann::json doc;
		for (int i = 0; i < mono_array_length(componentAttributes); ++i)
		{
			Horizon::Managed::ComponentAttribute* componentAttribute = (Horizon::Managed::ComponentAttribute*)mono_array_addr_with_size(componentAttributes, sizeof(Horizon::Managed::ComponentAttribute), i);
			std::string name =componentAttribute->name;
			std::string val = componentAttribute->value;

			doc[name] = val;

			auto attribute = doc[name];

			componentJsonAttributes[name] = attribute;
		}

		Horizon::Component* component = Horizon::ComponentFactory::GetInstance()->CreateComponent(componentNameStr, componentJsonAttributes, 0);
		component->CreateResources();
		component->InitComponent();
		gameObject->AddComponent(component);
	}
}

static void Managed_AddScript(uint32_t gameObjectId, MonoString* scriptName)
{
	std::string scriptNameStr = mono_string_to_utf8(scriptName);

	Horizon::GameObject* gameObject = Horizon::SceneManager::GetInstance()->GetGameObject(gameObjectId);

	if (gameObject)
	{
		Horizon::ScriptInstance* scriptInstance = Horizon::ScriptingRuntime::GetInstance()->AttachScript(scriptNameStr, std::unordered_map<std::string, std::string>(), gameObject);
				
		Horizon::ScriptingRuntime::GetInstance()->InvokeMethodNoReturnScriptInstance(scriptInstance, "Start");
		scriptInstance->startCalled = true;
	}
}

static MonoString* Managed_GetName(uint32_t gameObjectId)
{
	Horizon::GameObject* gameObject = Horizon::SceneManager::GetInstance()->GetGameObject(gameObjectId);

	if (gameObject)
	{
		const std::string nameStr = gameObject->GetName();

		MonoString* name = mono_string_new(mono_domain_get(), nameStr.c_str());

		return name;
	}

	return nullptr;
}

static bool Managed_GetVisible(uint32_t gameObjectId)
{
	Horizon::GameObject* gameObject = Horizon::SceneManager::GetInstance()->GetGameObject(gameObjectId);

	if (gameObject)
	{
		return gameObject->GetActive();
	}

	return false;
}

static void Managed_SetVisible(uint32_t gameObjectId, bool newVisible)
{
	Horizon::GameObject* gameObject = Horizon::SceneManager::GetInstance()->GetGameObject(gameObjectId);

	if (gameObject)
	{
		gameObject->SetActive(newVisible);
	}
}

#pragma warning( pop )  