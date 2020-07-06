#pragma  once

#include "core\Material.h"
#include "subsystems\EngineAllocator.h"
#include "subsystems\MaterialFactory.h"
#include "structures\MaterialCustomAttribute.h"
#include "platform\ShaderProgram.h"
#include "materials\DefaultPhongMaterial.h"
#include "materials\DefaultSkyboxMaterial.h"

#include <unordered_map>
#include <string>
#include <nlohmann\json.hpp>	

#define CREATEMATERIALCREATEMAPFUNCTION(Material_Type) \
static Horizon::Material* Material_Type##CreateMethod(const std::string& materialId, Graphics::ShaderProgram* shaderProgram, const std::unordered_map<std::string, MaterialCustomAttribute>& customAttributes)\
{\
    Material_Type* material = EngineAllocateC(Material_Type, Material_Type(materialId, shaderProgram, customAttributes));\
	material->InitializeMaterialResources();\
	return material;\
}

#define CREATEMATERIALMAP(Material_Type) \
MaterialFactory::GetInstance()->CreateMaterialNameMapping(#Material_Type, Material_Type##CreateMethod)

namespace Horizon
{
	CREATEMATERIALCREATEMAPFUNCTION(DefaultPhongMaterial)
	CREATEMATERIALCREATEMAPFUNCTION(DefaultSkyboxMaterial)

	static void RegisterMaterialCreateFunctions()
	{
		CREATEMATERIALMAP(DefaultPhongMaterial);
		CREATEMATERIALMAP(DefaultSkyboxMaterial);
	}
}
