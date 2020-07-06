 #pragma once

#include "core\ExportMacro.h"
#include "core\TypeAbstractions.h"
#include "core\Subsystem.h"
#include "core\GameObject.h"
#include "structures\ScriptInstance.h"
#include "structures\DependentAssembly.h"
#include "structures\DependentAssemblyDefinition.h"
#include "structures\AssemblyData.h"

#include <map>
#include <unordered_map>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/class.h>
#include <mono/metadata/mono-config.h>

namespace Horizon
{
	/*
		The subsystem within the engine that manages interfaces with the C# scripting runtime (via Mono).
	*/
	class HORIZON_API ScriptingRuntime : public Subsystem<ScriptingRuntime>
	{
	public:
		bool Init(std::vector<Horizon::Scripting::DependentAssemblyDefinition> dependentAssemblies);
		void Shutdown();

		//'Attach' a script to a game object, returns the script instance instantiated.
		ScriptInstance* AttachScript(const std::string& scriptClassName, std::unordered_map<std::string, std::string> customAttributes, GameObject* gameObject);
		
		//Return the value of a named script attribute (specified in scene json file).
		std::string GetScriptCustomAttribute(const uint32& scriptId, const std::string& customAttributeName);

		//Return a script instance based on the assigned script id.
		ScriptInstance* GetScriptInstance(const uint32& scriptId);
		
		//Execute start on all script instances.
		void Start();

		//Execute Update on all script instances.
		void Update();

		//Execute LateUpdate on all script instances.
		void LateUpdate();

		//Return the root mono domain.
		MonoDomain* GetRootMonoDomain();
		
		//Resolve a class within any of the loaded/dependent assemblies.
		MonoClass* GetClass(const std::string& nameSpace, const std::string& name);

		//Resolve a method within any of the loaded/dependent assemblies.
		MonoMethod* GetMethod(MonoClass* monoClass, const std::string& name);

		//Resolve a class field within any of the loaded/dependent assemblies.
		MonoClassField* GetClassField(MonoClass* monoClass, const std::string& fieldName);
		
		//Invoke an managed method.
		MonoObject* InvokeMethod(MonoClass* monoClass, const std::string& functionName, MonoObject* monoObject, void** args = nullptr, int argCount = -1);

		//Invoke a managed function on a script instance without any regard for what is returned.
		void InvokeMethodNoReturnScriptInstance(ScriptInstance* scriptInstance, const std::string& functionName, void** args = nullptr, int paramCount = -1);


	private:
		//Get any classes required by the subsystem.
		void GetClasses();

		//Register extern-ed methods within the HorizonScript runtime.
		void RegisterInternalCalls();
	
		//Takes the absolute path to the assembly file (.dll)
		AssemblyData LoadAssemblyImageFromFilePath(const std::string& filePath);

		//Reload all dependent assemblies.
		void ReloadAssemblies();
		
		//Root mono domain.
		MonoDomain* m_monoDomain;

		//The second app domain to host the horizon runtime.
		MonoDomain* m_childDomain;

		MonoAssembly* m_assembly;
		MonoImage* m_monoImage;
		std::string m_mainAssemblyNameSpace;

		//namespace to assembly id in m_dependentAssemblies.
		std::unordered_map<std::string, int> m_namespaceToAssemblyMapping;

		//assemly id to DependentAssembly instance (contains MonoAssembly* and MonoImage*).
		std::unordered_map<int, DependentAssembly> m_dependentAssemblies;

		bool m_scriptInstanceRunning;

		std::vector<ScriptInstance*> m_outstandingScriptInstances;
		std::vector<ScriptInstance*> m_scriptInstances;

		MonoClass* m_gameObjectClass;
		MonoClass* m_scriptManagerClass;
		MonoClass* m_transformClass;
		MonoClass* m_vector3Class;
		MonoClass* m_mainScriptClass;
	};
}
