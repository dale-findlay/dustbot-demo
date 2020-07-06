#pragma once

#include "core\ExportMacro.h"
#include "core\Material.h"
#include "structures\MaterialCustomAttribute.h"
#include "platform\TextureCube.h"
#include "resources\TextureCubeResource.h"

#include <mutex>
#include <atomic>
#include <unordered_map>
#include <string>

namespace Horizon
{
	class HORIZON_API DefaultSkyboxMaterial : public Material
	{
	public:
		DefaultSkyboxMaterial(const std::string& materialId, Graphics::ShaderProgram* shaderProgram, const std::unordered_map<std::string, MaterialCustomAttribute>& customAttributes);

		void InitializeMaterial();

		// Inherited via Material
		virtual void MaterialResourceLoadComplete() override;
		virtual bool InitializeMaterialResources() override;

		virtual void UnbindMaterial() override;

		Graphics::TextureCube* skyboxTexture;

	private:		
		bool m_materialResourcesLoaded;
		bool m_materialResourceInitialized; 

		TextureCubeResource* m_skyboxTextureResource;

	protected:
		void BindUniforms() override;
		void BindTextures() override;


	};
}
