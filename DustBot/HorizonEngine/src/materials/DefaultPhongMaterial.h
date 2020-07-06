#pragma once

#include "core\ExportMacro.h"
#include "core\Material.h"
#include "structures\MaterialCustomAttribute.h"
#include "resources\Texture2DResource.h"
#include "platform\Texture2D.h"

#include <mutex>
#include <atomic>
#include <unordered_map>
#include <string>

namespace Horizon
{
	class HORIZON_API DefaultPhongMaterial : public Material
	{
	public:
		DefaultPhongMaterial(const std::string& materialId, Graphics::ShaderProgram* shaderProgram, const std::unordered_map<std::string, MaterialCustomAttribute>& customAttributes);

		void InitializeMaterial();

		// Inherited via Material
		virtual void MaterialResourceLoadComplete() override;
		virtual bool InitializeMaterialResources() override;

		virtual void UnbindMaterial() override;

		Graphics::Texture2D* diffuseTexture;
		Graphics::Texture2D* normalTexture;
		Graphics::Texture2D* specularTexture;

		float albedoIntensity;
		float specularScale;
		float shininess;
		float roughness;

	private:		
		bool m_materialResourcesLoaded;
		bool m_materialResourceInitialized; 

		Texture2DResource* m_diffuseTexture;

	protected:
		void BindUniforms() override;
		void BindTextures() override;


	};
}
