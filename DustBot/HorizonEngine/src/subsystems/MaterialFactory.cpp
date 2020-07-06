#include "MaterialFactory.h"

#include "subsystems\EngineAllocator.h"
#include "utilities\IOUtility.h"
#include "subsystems\VirtualFileSystem.h"
#include "utilities\Logger.h"
#include "utilities\ShaderIOUtility.h"
#include "materials\DefaultPhongMaterial.h"
#include "structures\MaterialCustomAttribute.h"

#include "materials\MaterialCreationMappings.h"

#include <unordered_map>

using namespace Horizon::Graphics;

bool Horizon::MaterialFactory::Init()
{
	if (!IOUtility::FileExists("~/material_index.json"))
	{
		std::string assetsDir = VirtualFileSystem::GetInstance()->ResolveFilePath("~/");


		Logger::GetInstance()->LogError("Resource Index not found. Please ensure that a file called material_index.json is present in the directory: " + assetsDir);
		return false;
	}

	std::string materialIndexJson = IOUtility::ReadFileToEnd("~/material_index.json");

	nlohmann::json doc = nlohmann::json::parse(materialIndexJson);

	m_materialTypes = ReadMaterialTypesFromFile(doc);
	m_masterMaterials = ReadMasterMaterialsFromFile(doc);
	
	RegisterMaterialCreateFunctions();

	InitializeDefaultMaterial();
	
	return true;
}

void Horizon::MaterialFactory::Shutdown()
{

}

Horizon::Material* Horizon::MaterialFactory::GetDefaultMaterial()
{
	return m_defaultMaterial;
}

Horizon::Material * Horizon::MaterialFactory::GetMaterialFromId(const std::string & materialId)
{
	Material* foundMaterial = GetMaterialInstance(materialId);

	if (foundMaterial == nullptr)
	{
		MaterialDefinition materialDef = GetMaterialDefinition(materialId);

		MaterialType materialType = GetMaterialType(materialDef.materialTypeId);
		const std::string shaderFileName = materialType.shaderLocation;

		auto shaderParts = ShaderIOUtility::GetShaderParts(shaderFileName);

		Shader* vertexShader = EngineAllocateC(Shader, Shader(GL_VERTEX_SHADER, shaderParts["vertex"], shaderFileName, "vertex"));
		Shader* fragmentShader = EngineAllocateC(Shader, Shader(GL_FRAGMENT_SHADER, shaderParts["fragment"], shaderFileName, "fragment"));

		ShaderProgram* shaderProgram = EngineAllocateC(ShaderProgram, ShaderProgram());

		shaderProgram->AttachShader(vertexShader);
		shaderProgram->AttachShader(fragmentShader);

		shaderProgram->BindAttribLocation(0, "position");
		shaderProgram->BindAttribLocation(1, "texCoord");
		shaderProgram->BindAttribLocation(2, "normal");

		shaderProgram->LinkShaderProgram();

		TrackShaderProgram(shaderProgram);

		auto func = m_creationFunctions[materialType.name];

		Material* material = func(materialId, shaderProgram, materialDef.customAttributes);
		m_materialInstances.push_back(material);

		return material;
	}
	else
	{
		return foundMaterial;
	}
}

void Horizon::MaterialFactory::TrackShaderProgram(ShaderProgram * shaderProgram)
{
	m_shaderPrograms.push_back(shaderProgram);
}

void Horizon::MaterialFactory::ReloadTrackedShaders()
{
	for (int i = 0; i < m_shaderPrograms.size(); ++i)
	{
		m_shaderPrograms[i]->QueueReload();
	}
}

void Horizon::MaterialFactory::CreateMaterialNameMapping(const std::string & name, std::function<Material*(const std::string&materialId, ShaderProgram*shaderProgram, const std::unordered_map<std::string, MaterialCustomAttribute>&customAttributes)> creationFunction)
{
	if (GetMaterialType(name).id == 100000000)
	{
		Logger::GetInstance()->LogError("Material with name " + name + " is not present in the material_index");
		return;
	}

	m_creationFunctions[name] = creationFunction;
}


void Horizon::MaterialFactory::InitializeDefaultMaterial()
{
	m_defaultMaterial = GetMaterialFromId("69c80027860152f013c4906142116517004f3409");
	m_defaultShaderProgram = m_defaultMaterial->GetShaderProgram();
}

Horizon::Material* Horizon::MaterialFactory::GetMaterialInstance(const std::string & materialId)
{
	for (auto it = m_materialInstances.begin(); it != m_materialInstances.end(); ++it)
	{
		if ((*it)->GetMaterialId() == materialId)
		{
			return (*it);
		}
	}

	return nullptr;
}

Horizon::MaterialDefinition Horizon::MaterialFactory::GetMaterialDefinition(std::string materialId)
{
	for (auto it = m_masterMaterials.begin(); it != m_masterMaterials.end(); ++it)
	{
		if ((*it).id == materialId)
		{
			return (*it);
		}
	}

	return MaterialDefinition();
}

Horizon::MaterialType Horizon::MaterialFactory::GetMaterialType(int materialTypeId)
{
	for (auto it = m_materialTypes.begin(); it != m_materialTypes.end(); ++it)
	{
		if ((*it).id == materialTypeId)
		{
			return (*it);
		}
	}

	return MaterialType();
}

Horizon::MaterialType Horizon::MaterialFactory::GetMaterialType(const std::string & materialName)
{
	for (auto it = m_materialTypes.begin(); it != m_materialTypes.end(); ++it)
	{
		if ((*it).name == materialName)
		{
			return (*it);
		}
	}

	return MaterialType();
}

std::vector<Horizon::MaterialType> Horizon::MaterialFactory::ReadMaterialTypesFromFile(nlohmann::json & doc)
{
	std::vector<MaterialType> materialTypeInstances;

	assert(IOUtility::ContainsMember(doc, "materialTypes"));

	const auto& materialTypes = doc["materialTypes"];
	assert(materialTypes.is_array());
	size_t materialTypeCount = materialTypes.size();

	materialTypeInstances.reserve(materialTypeCount);

	for (auto it = materialTypes.begin(); it != materialTypes.end(); ++it)
	{
		const auto& materialTypeObject = (*it);

		assert(IOUtility::ContainsMember(materialTypeObject, "id"));
		assert(IOUtility::ContainsMember(materialTypeObject, "name"));
		assert(IOUtility::ContainsMember(materialTypeObject, "shaderLocation"));

		uint32 id = materialTypeObject["id"].get<uint32>();
		std::string name = materialTypeObject["name"].get<std::string>();
		std::string shaderLocation = materialTypeObject["shaderLocation"].get<std::string>();

		materialTypeInstances.emplace_back(id, name, shaderLocation);
	}

	return materialTypeInstances;
}

std::vector<Horizon::MaterialDefinition> Horizon::MaterialFactory::ReadMasterMaterialsFromFile(nlohmann::json & doc)
{
	std::vector<MaterialDefinition> masterMaterialInstances;

	assert(IOUtility::ContainsMember(doc, "masterMaterials"));

	const auto& masterMaterials = doc["masterMaterials"];
	assert(masterMaterials.is_array());
	size_t materialTypeCount = masterMaterials.size();

	masterMaterialInstances.reserve(materialTypeCount);

	for (auto it = masterMaterials.begin(); it != masterMaterials.end(); ++it)
	{
		const auto& masterMaterialObject = (*it);

		assert(IOUtility::ContainsMember(masterMaterialObject, "id"));
		assert(IOUtility::ContainsMember(masterMaterialObject, "materialTypeId"));

		std::string id = masterMaterialObject["id"].get<std::string>();
		uint32 materialTypeId = masterMaterialObject["materialTypeId"].get<uint32>();

		std::unordered_map<std::string, MaterialCustomAttribute> customAttributes;

		const auto& masterMaterialCustomAttributes = masterMaterialObject["customAttributes"];
		for (auto masterMatIterator = masterMaterialCustomAttributes.begin(); masterMatIterator != masterMaterialCustomAttributes.end(); ++masterMatIterator)
		{
			const auto& attributeObject = (*masterMatIterator);
			std::string name = attributeObject["name"].get<std::string>();
			std::string attributeType = attributeObject["attributeType"].get<std::string>();
			nlohmann::json::value_type value = attributeObject["value"];
			customAttributes[name] = { name, attributeType, value };
		}

		masterMaterialInstances.emplace_back(id, materialTypeId, customAttributes);
	}

	return masterMaterialInstances;
}
