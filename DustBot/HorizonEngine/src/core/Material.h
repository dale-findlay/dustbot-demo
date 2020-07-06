#pragma once

#include "core\ExportMacro.h"
#include "structures\MaterialCustomAttribute.h"
#include "structures\ResourceSet.h"
#include "platform\ShaderProgram.h"
#include "platform\Texture.h"

#include <unordered_map>
#include <vector>
#include <string>
#include <nlohmann\json.hpp>	

namespace Horizon
{
	/*
		A object that manipulates the state of an attached shader program, and maintaining pointers to any allocated resources within the engine.
	*/
	class HORIZON_API Material
	{
	public:
		Material(const std::string& materialId, Graphics::ShaderProgram* shaderProgram, const std::unordered_map<std::string, MaterialCustomAttribute>& customAttributes);
		~Material();
		
		virtual void MaterialResourceLoadComplete() = 0;
		virtual bool InitializeMaterialResources() = 0;

		std::string GetMaterialId() const;

		//Sets up any shader states and textures and uniform data.
		virtual void BindMaterial();

		//Resets the relevant states of any textures. (This has a performance hit, ensure this is only used for Debug Mode).
		virtual void UnbindMaterial();

		//Return the shader program the material acts on.
		Graphics::ShaderProgram* GetShaderProgram();

	protected:
		virtual void BindTextures();
		virtual void BindUniforms();

		void BindActivateTextures(const std::vector<Graphics::Texture*>& textures);
		void UnbindActivateTextures(const std::vector<Graphics::Texture*>& textures);

		std::unordered_map<std::string, MaterialCustomAttribute> m_customAttributes;
		Graphics::ShaderProgram* m_shaderProgram;
		std::string m_materialId;

	};
}