#include "ScriptingRuntime.h"

#include "core\Timer.h"
#include "core\TypeAbstractions.h"
#include "subsystems\EngineAllocator.h"
#include "subsystems\VirtualFileSystem.h"
#include "subsystems\InputManager.h"
#include "subsystems\InputManager.h"
#include "managed\TransformManaged.h"
#include "managed\GameObjectManaged.h"
#include "managed\InternalCalls.h"
#include "managed\ScriptCustomAttributeManaged.h"
#include "utilities\Logger.h"

#include <fstream>
#include <iostream>
#include <glm\common.hpp>
#include <mono/metadata/mono-config.h>
#include <mono/metadata/threads.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/mono-gc.h>
#include <mono/metadata/environment.h>
#include <mono/metadata/mono-debug.h>

using namespace Horizon::Managed;

bool Horizon::ScriptingRuntime::Init(std::vector<Horizon::Scripting::DependentAssemblyDefinition> dependentAssemblies)
{
	m_scriptInstanceRunning = false;

	mono_set_dirs("C:\\Program Files\\Mono\\lib",
		"C:\\Program Files\\Mono\\etc");

	mono_config_parse(nullptr);

#ifdef MONO_DEBUG_ATTACH
	const char* options[] =
	{
		"--soft-breakpoints",
		"--debugger-agent=transport=dt_socket,address=127.0.0.1:11000,server=y"
	};

	mono_jit_parse_options(sizeof(options) / sizeof(char*), (char**)options);

	mono_debug_init(MONO_DEBUG_FORMAT_MONO);
	Logger::GetInstance()->LogWarning("Waiting For Debugger Attach....");
#endif

	m_monoDomain = mono_jit_init("app");

	m_childDomain = mono_domain_create_appdomain((char*)"app-sub", NULL);
	if (mono_debug_enabled())
	{
		mono_debug_domain_create(m_childDomain);
	}

	mono_domain_set(m_childDomain, false);

	for (auto it = dependentAssemblies.begin(); it != dependentAssemblies.end(); ++it)
	{
		auto dependentAssemblyDefinition = (*it);
		DependentAssembly dependentAssembly;

		dependentAssembly.filePath = VirtualFileSystem::GetInstance()->ResolveFilePath(dependentAssemblyDefinition.filePath);

		AssemblyData assemblyData = LoadAssemblyImageFromFilePath(dependentAssembly.filePath);
		if (assemblyData.valid)
		{
			MonoImage* newImage = nullptr;
			MonoAssembly* newAssembly = nullptr;

			MonoImageOpenStatus monoImageOpenStatus;

			newImage = mono_image_open_from_data_with_name(assemblyData.assemblyData, (uint32_t)assemblyData.size, true, &monoImageOpenStatus, false, dependentAssembly.filePath.c_str());
			newAssembly = mono_assembly_load_from_full(newImage, dependentAssembly.filePath.c_str(), &monoImageOpenStatus, false);
			if (monoImageOpenStatus != MONO_IMAGE_OK || newAssembly == nullptr)
			{
				Logger::GetInstance()->LogError("Failed to load assembly" + dependentAssembly.filePath);
				mono_image_close(newImage);
			}
			else
			{
				dependentAssembly.image = newImage;
				dependentAssembly.assembly = newAssembly;
			}

			if (dependentAssemblyDefinition.mainAssembly)
			{
				m_mainAssemblyNameSpace = dependentAssemblyDefinition.namespaces[0];
				m_assembly = dependentAssembly.assembly;
				m_monoImage = dependentAssembly.image;
			}

			int assemblyId = (int)m_dependentAssemblies.size();

			m_dependentAssemblies[assemblyId] = dependentAssembly;

			for (auto namespaceIt = dependentAssemblyDefinition.namespaces.begin(); namespaceIt != dependentAssemblyDefinition.namespaces.end(); ++namespaceIt)
			{
				m_namespaceToAssemblyMapping[*namespaceIt] = assemblyId;
			}
		}
	}

	RegisterInternalCalls();
	GetClasses();

	MonoMethod* scriptInitMethod = GetMethod(m_mainScriptClass, "Init");
	mono_runtime_invoke(scriptInitMethod, nullptr, nullptr, nullptr);

	return true;
}

void Horizon::ScriptingRuntime::Shutdown()
{
	MonoMethod* scriptShutdownMethod = GetMethod(m_mainScriptClass, "Shutdown");
	mono_runtime_invoke(scriptShutdownMethod, nullptr, nullptr, nullptr);

	mono_jit_cleanup(m_monoDomain);
}

void Horizon::ScriptingRuntime::RegisterInternalCalls()
{
	mono_add_internal_call("HorizonEngine.Core.Time::_CalculateDeltaTime", Managed_CalculateDeltaTime);

	mono_add_internal_call("HorizonEngine.Subsystems.InputManager::_GetKeyDown", Managed_GetKeyDown);
	mono_add_internal_call("HorizonEngine.Subsystems.InputManager::_GetMouseDelta", Managed_GetMouseDelta);
	mono_add_internal_call("HorizonEngine.Subsystems.InputManager::_GetMouseButtonDown", Managed_GetMouseButtonDown);
	mono_add_internal_call("HorizonEngine.Subsystems.InputManager::_GetScrollOffset", Managed_GetScrollOffset);

	mono_add_internal_call("HorizonEngine.Subsystems.VirtualFileSystem::_ResolveFilePath", Managed_ResolveFilePath);
	mono_add_internal_call("HorizonEngine.Core.Script::_GetCustomAttributeValue", Managed_GetScriptCustomAttribute);
	mono_add_internal_call("HorizonEngine.Core.Script::_FindGameObject", Managed_FindGameObject);

	mono_add_internal_call("HorizonEngine.Core.Transform::_GetPosition", Managed_GetPosition);
	mono_add_internal_call("HorizonEngine.Core.Transform::_GetEulerRotation", Managed_GetEulerRotation);
	mono_add_internal_call("HorizonEngine.Core.Transform::_GetScale", Managed_GetScale);
	mono_add_internal_call("HorizonEngine.Core.Transform::_GetRotation", Managed_GetRotation);

	mono_add_internal_call("HorizonEngine.Core.Transform::_GetForward", Managed_GetForward);
	mono_add_internal_call("HorizonEngine.Core.Transform::_GetRight", Managed_GetRight);

	mono_add_internal_call("HorizonEngine.Core.Transform::_SetPosition", Managed_SetPosition);
	mono_add_internal_call("HorizonEngine.Core.Transform::_SetEulerRotation", Managed_SetEulerRotation);
	mono_add_internal_call("HorizonEngine.Core.Transform::_SetScale", Managed_SetScale);
	mono_add_internal_call("HorizonEngine.Core.Transform::_SetRotation", Managed_SetRotation);

	mono_add_internal_call("HorizonEngine.Core.GameObject::_CreateGameObject", Managed_CreateGameObject);
	mono_add_internal_call("HorizonEngine.Core.GameObject::_AddComponent", Managed_AddComponent);
	mono_add_internal_call("HorizonEngine.Core.GameObject::_AddScript", Managed_AddScript);
	mono_add_internal_call("HorizonEngine.Core.GameObject::_GetName", Managed_GetName);
	mono_add_internal_call("HorizonEngine.Core.GameObject::_GetVisible", Managed_GetVisible);
	mono_add_internal_call("HorizonEngine.Core.GameObject::_SetVisible", Managed_SetVisible);
}

Horizon::AssemblyData Horizon::ScriptingRuntime::LoadAssemblyImageFromFilePath(const std::string & filePath)
{
	std::ifstream fileStream(filePath.c_str(), std::ifstream::binary);

	size_t fileLength = 0;
	char* assemblyData;
	if (!fileStream.bad())
	{
		fileStream.seekg(0, fileStream.end);
		fileLength = fileStream.tellg();

		fileStream.seekg(0, fileStream.beg);

		assemblyData = new char[fileLength + 1];

		fileStream.read(assemblyData, fileLength + 1);
		fileStream.close();
	}
	else
	{
		Logger::GetInstance()->LogError("Failed to read file at location" + filePath + ". File stream bad.");
		return { false, 0, nullptr };
	}

	return { true, fileLength, assemblyData };
}

void Horizon::ScriptingRuntime::ReloadAssemblies()
{
	MonoMethod* scriptShutdownMethod = GetMethod(m_mainScriptClass, "Shutdown");
	mono_runtime_invoke(scriptShutdownMethod, nullptr, nullptr, nullptr);

	mono_domain_set(m_monoDomain, false);

	if (mono_debug_enabled())
	{
		mono_debug_domain_unload(m_childDomain);
	}
	else
	{
		mono_domain_unload(m_childDomain);
	}

	mono_gc_collect(mono_gc_max_generation());

	m_childDomain = mono_domain_create_appdomain((char*)"app-sub", NULL);
	if (mono_debug_enabled())
	{
		mono_debug_domain_create(m_childDomain);
	}

	mono_domain_set(m_childDomain, false);

	for (auto it = m_dependentAssemblies.begin(); it != m_dependentAssemblies.end(); ++it)
	{
		DependentAssembly& dependentAssembly = (*it).second;
		AssemblyData assemblyData = LoadAssemblyImageFromFilePath(dependentAssembly.filePath);
		if (assemblyData.valid)
		{
			MonoImage* newImage;
			MonoAssembly* newAssembly;

			MonoImageOpenStatus monoImageOpenStatus;
			mono_image_close(dependentAssembly.image);

			newImage = mono_image_open_from_data_with_name(assemblyData.assemblyData, (uint32_t)assemblyData.size, true, &monoImageOpenStatus, false, dependentAssembly.filePath.c_str());
			newAssembly = mono_assembly_load_from_full(newImage, dependentAssembly.filePath.c_str(), &monoImageOpenStatus, false);
			if (monoImageOpenStatus != MONO_IMAGE_OK || newAssembly == nullptr)
			{
				Logger::GetInstance()->LogError("Failed to load assembly" + dependentAssembly.filePath);
			}
			else
			{
				dependentAssembly.image = newImage;
				dependentAssembly.assembly = newAssembly;
			}

			delete[] assemblyData.assemblyData;
		}
	}

	GetClasses();
	RegisterInternalCalls();

	m_scriptInstanceRunning = true;
	//re set all the objects states.
	for (auto it = m_scriptInstances.begin(); it != m_scriptInstances.end(); ++it)
	{
		ScriptInstance* scriptInstance = (*it);
		scriptInstance->scriptClass = GetClass(m_mainAssemblyNameSpace.c_str(), scriptInstance->scriptName);

		MonoObject* classInstance = mono_object_new(m_childDomain, scriptInstance->scriptClass);
		scriptInstance->scriptClassInstance = classInstance;

		MonoMethod* constructorMethod = mono_class_get_method_from_name(scriptInstance->scriptClass,
			".ctor",
			1);

		mono_field_set_value(classInstance, GetClassField(scriptInstance->scriptClass, "m_scriptId"), &scriptInstance->scriptId);

		uint32 id = scriptInstance->gameObject->GetId();

		void* args[1];
		args[0] = reinterpret_cast<void*>(&id);
		mono_runtime_invoke(constructorMethod, classInstance, args, NULL);
	}
	m_scriptInstanceRunning = false;

	MonoMethod* scriptInitMethod = GetMethod(m_mainScriptClass, "Init");
	mono_runtime_invoke(scriptInitMethod, nullptr, nullptr, nullptr);

	//Call the start on all the objects again.
	Start();

	Logger::GetInstance()->LogWarning("[" + std::to_string(m_dependentAssemblies.size()) + "] Assemblies Reloaded.");
}

void Horizon::ScriptingRuntime::GetClasses()
{
	m_mainScriptClass = GetClass(m_mainAssemblyNameSpace, m_mainAssemblyNameSpace);
	m_gameObjectClass = GetClass("HorizonEngine.Core", "GameObject");
	m_scriptManagerClass = GetClass("HorizonEngine.Subsystems", "ScriptManager");
	m_transformClass = GetClass("HorizonEngine.Structures", "Transform");
	m_vector3Class = GetClass("GlmNet", "vec3");
}

Horizon::ScriptInstance* Horizon::ScriptingRuntime::AttachScript(const std::string& scriptClassName, std::unordered_map<std::string, std::string> customAttributes, GameObject * gameObject)
{
	ScriptInstance* instance = EngineAllocateC(ScriptInstance, ScriptInstance());
	instance->scriptId = (uint32)m_scriptInstances.size() + 1;
	instance->startCalled = false;

	MonoClass* classDef = mono_class_from_name(m_monoImage, m_mainAssemblyNameSpace.c_str(), scriptClassName.c_str());
	instance->scriptClass = classDef;

	MonoObject* classInstance = mono_object_new(m_childDomain, classDef);
	instance->scriptClassInstance = classInstance;

	instance->gameObject = gameObject;

	MonoMethod* constructorMethod = mono_class_get_method_from_name(classDef,
		".ctor",
		1);

	int gameObjectId = instance->gameObject->GetId();
	void* args[1];
	args[0] = (void*)&gameObjectId;

	mono_runtime_invoke(constructorMethod, classInstance, args, NULL);

	mono_field_set_value(classInstance, GetClassField(classDef, "m_scriptId"), &instance->scriptId);

	instance->scriptName = scriptClassName;
	instance->customAttributes = customAttributes;

	if (!m_scriptInstanceRunning)
	{
		m_scriptInstances.push_back(instance);
	}
	else
	{
		m_outstandingScriptInstances.push_back(instance);
	}


	return instance;
}

std::string Horizon::ScriptingRuntime::GetScriptCustomAttribute(const uint32 & scriptId, const std::string & customAttributeName)
{
	for (auto it = m_scriptInstances.begin(); it != m_scriptInstances.end(); ++it)
	{
		if ((*it)->scriptId == scriptId)
		{
			if ((*it)->customAttributes.count(customAttributeName) > 0)
			{
				return (*it)->customAttributes[customAttributeName];
			}
		}
	}

	return "Attribute Not Found";
}

Horizon::ScriptInstance* Horizon::ScriptingRuntime::GetScriptInstance(const uint32 & scriptId)
{
	ScriptInstance* scriptInstance = nullptr;

	for (auto it = m_scriptInstances.begin(); it != m_scriptInstances.end(); ++it)
	{
		if ((*it)->scriptId == scriptId)
		{
			scriptInstance = (*it);
			break;
		}
	}

	return scriptInstance;
}

void Horizon::ScriptingRuntime::Start()
{
	m_scriptInstanceRunning = true;
	InvokeMethod(m_scriptManagerClass, "Start", nullptr);
	m_scriptInstanceRunning = false;

	//Catch any game objects with scripts on them created in the update method. They are added to this list when the scriptInstanceRunning variable is true.
	for (auto it = m_outstandingScriptInstances.begin(); it != m_outstandingScriptInstances.end(); ++it)
	{
		m_scriptInstances.push_back(*it);
	}

	m_outstandingScriptInstances.clear();

	//Run the garbage collector right after its done initializing everything to make sure it doest run later in the middle of update.
	mono_gc_collect(mono_gc_max_generation());

}

void Horizon::ScriptingRuntime::Update()
{
//#ifdef MONO_HOT_RELOAD
//	if (InputManager::GetInstance()->GetKeyDown(GLFW_KEY_R))
//	{
//		ReloadAssemblies();
//	}
//#endif

	m_scriptInstanceRunning = true;
	InvokeMethod(m_scriptManagerClass, "Update", nullptr);
	m_scriptInstanceRunning = false;

	//same goes for update, catch any game objects with scripts on them created in the update method.
	for (auto it = m_outstandingScriptInstances.begin(); it != m_outstandingScriptInstances.end(); ++it)
	{
		m_scriptInstances.push_back(*it);
	}

	m_outstandingScriptInstances.clear();
}

void Horizon::ScriptingRuntime::LateUpdate()
{
	m_scriptInstanceRunning = true;
	for (auto it = m_scriptInstances.begin(); it != m_scriptInstances.end(); ++it)
	{
		auto scriptInstance = (*it);

		if (scriptInstance->gameObject->GetActive())
		{
			InvokeMethodNoReturnScriptInstance(scriptInstance, "LateUpdate");
		}
	}
	m_scriptInstanceRunning = false;

	//see update & start.
	for (auto it = m_outstandingScriptInstances.begin(); it != m_outstandingScriptInstances.end(); ++it)
	{
		m_scriptInstances.push_back(*it);
	}

	m_outstandingScriptInstances.clear();
}

MonoDomain* Horizon::ScriptingRuntime::GetRootMonoDomain()
{
	return m_monoDomain;
}

MonoClass* Horizon::ScriptingRuntime::GetClass(const std::string & nameSpace, const std::string & name)
{
	DependentAssembly assembly;
	int assemblyId = -1;
	if (m_namespaceToAssemblyMapping.count(nameSpace))
	{
		assemblyId = m_namespaceToAssemblyMapping[nameSpace];
	}

	if (assemblyId == -1)
	{
		Logger::GetInstance()->LogError("Failed to resolve assembly for class " + name + " in namespace" + std::string(nameSpace));
		return nullptr;
	}

	assembly = m_dependentAssemblies[assemblyId];
	return mono_class_from_name(assembly.image, nameSpace.c_str(), name.c_str());
}

MonoMethod* Horizon::ScriptingRuntime::GetMethod(MonoClass* monoClass, const std::string & name)
{
	MonoMethod* methodDef = mono_class_get_method_from_name(monoClass, name.c_str(), -1);

	return methodDef;
}

MonoClassField * Horizon::ScriptingRuntime::GetClassField(MonoClass* monoClass, const std::string & fieldName)
{
	return mono_class_get_field_from_name(monoClass, fieldName.c_str());
}

void Horizon::ScriptingRuntime::InvokeMethodNoReturnScriptInstance(ScriptInstance* scriptInstance, const std::string& functionName, void** args, int paramCount)
{
	InvokeMethod(scriptInstance->scriptClass, functionName, scriptInstance->scriptClassInstance, args, paramCount);
}

MonoObject* Horizon::ScriptingRuntime::InvokeMethod(MonoClass* monoClass, const std::string& functionName, MonoObject* monoObject, void** args, int argCount)
{
	MonoObject* exception = nullptr;

	MonoMethod* methodDef = mono_class_get_method_from_name(monoClass, functionName.c_str(), argCount);
	MonoObject* res = mono_runtime_invoke(methodDef, reinterpret_cast<void*>(monoObject), args, &exception);

	if (exception != nullptr)
	{
		mono_print_unhandled_exception(exception);
	}

	return res;
}
