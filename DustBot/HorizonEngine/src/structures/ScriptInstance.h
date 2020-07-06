#pragma once

#include "core\ExportMacro.h"
#include "core\GameObject.h"

#include <string>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

namespace Horizon
{
	struct HORIZON_API  ScriptInstance
	{
		uint32 scriptId;
		bool startCalled;
		std::string scriptName;
		MonoClass* scriptClass;
		MonoObject* scriptClassInstance;
		std::unordered_map<std::string, std::string> customAttributes;
		GameObject* gameObject;

		std::vector<GameObject*> accessedGameObjects;

	};
}
