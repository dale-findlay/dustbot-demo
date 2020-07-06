#pragma once

#include "core\ExportMacro.h"
#include "core\Subsystem.h"
#include "core\TypeAbstractions.h"
#include "core\Material.h"
#include "structures\MaterialType.h"
#include "structures\MaterialDefinition.h"

#include <vector>

#include <nlohmann\json.hpp>

namespace Horizon
{
	class HORIZON_API MaterialFactory : public Subsystem<MaterialFactory>
	{
	public:
		bool Init();
		void Shutdown();

		//Returns the default material.
		Material* GetDefaultMaterial();
		
		//Generate a material instance from an id defined in the material_index.
		Material* GetMaterialFromId(const std::string& materialId);

		//Track the shader program to allow reloading.
		void TrackShaderProgram(Graphics::ShaderProgram* shaderProgram);

		//Reloads any shaders tracked with TrackShaderProgram().
		void ReloadTrackedShaders();

		void CreateMaterialNameMapping(const std::string& name, std::function<Material*(const std::string& materialId, Graphics::ShaderProgram* shaderProgram, const std::unordered_map<std::string, MaterialCustomAttribute>& customAttributes)> creationFunction);

	private:

		void InitializeDefaultMaterial();
		Material* GetMaterialInstance(const std::string& materialId);

		MaterialDefinition GetMaterialDefinition(std::string materialId);
		MaterialType GetMaterialType(int materialTypeId);
		MaterialType GetMaterialType(const std::string& materialName);

		std::vector<MaterialType> ReadMaterialTypesFromFile(nlohmann::json& doc);
		std::vector<MaterialDefinition> ReadMasterMaterialsFromFile(nlohmann::json& doc);

		Material* m_defaultMaterial;
		Graphics::Shader* m_defaultVertexShader;
		Graphics::Shader* m_defaultFragmentShader;
		Graphics::ShaderProgram* m_defaultShaderProgram;

		std::vector<Material*> m_materialInstances;
		std::vector<MaterialType> m_materialTypes;
		std::vector<MaterialDefinition> m_masterMaterials;

		std::unordered_map < std::string, std::function<Material*(const std::string&materialId, Graphics::ShaderProgram* shaderProgram, const std::unordered_map<std::string, MaterialCustomAttribute>&customAttributes)>> m_creationFunctions;

		std::vector<Graphics::ShaderProgram*> m_shaderPrograms;
	};
}
