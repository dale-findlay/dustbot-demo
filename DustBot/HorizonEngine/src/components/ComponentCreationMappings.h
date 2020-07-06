#pragma once

#include "core\ExportMacro.h"
#include "subsystems\EngineAllocator.h"
#include "subsystems\ComponentFactory.h"
#include "components\Component.h"
#include "components\ScriptComponent.h"
#include "components\MeshComponent.h"
#include "components\CameraComponent.h"
#include "components\DirectionalLightComponent.h"
#include "components\PointLightComponent.h"

#include <unordered_map>
#include <string>
#include <nlohmann\json.hpp>	


#define CREATECOMPONENTCREATEMAPFUNCTION(ComponentType) \
static Horizon::Component* ComponentType##CreateMethod(const std::unordered_map<std::string, nlohmann::json::value_type>& componentAttributes)\
{\
    ComponentType* component = EngineAllocateC(ComponentType, ComponentType());\
    component->CreateComponent(componentAttributes);\
	return component;\
}

#define CREATECOMPONENTMAP(ComponentType) \
ComponentFactory::GetInstance()->CreateComponentNameMapping(#ComponentType, ComponentType##CreateMethod)


namespace Horizon
{
	CREATECOMPONENTCREATEMAPFUNCTION(ScriptComponent)
 	CREATECOMPONENTCREATEMAPFUNCTION(MeshComponent)
	CREATECOMPONENTCREATEMAPFUNCTION(CameraComponent)
	CREATECOMPONENTCREATEMAPFUNCTION(DirectionalLightComponent)
	CREATECOMPONENTCREATEMAPFUNCTION(PointLightComponent)

	static void RegisterComponentCreateFunctions()
	{
		CREATECOMPONENTMAP(ScriptComponent);
		CREATECOMPONENTMAP(MeshComponent);
		CREATECOMPONENTMAP(CameraComponent);
		CREATECOMPONENTMAP(DirectionalLightComponent);
		CREATECOMPONENTMAP(PointLightComponent);
	}
}