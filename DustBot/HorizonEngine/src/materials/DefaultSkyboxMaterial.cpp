#include "DefaultSkyboxMaterial.h"
#include <vector>
#include "platform\Texture.h"
#include "platform\GLHelper.h"

#include "subsystems\EngineAllocator.h"
#include "subsystems\ResourceManager.h"
#include "structures\MaterialCustomAttribute.h"
#include "structures\ResourceLoadState.h"

using namespace Horizon::Graphics;

Horizon::DefaultSkyboxMaterial::DefaultSkyboxMaterial(const std::string & materialId, ShaderProgram * shaderProgram, const std::unordered_map<std::string, MaterialCustomAttribute>& customAttributes)
	: Material(materialId, shaderProgram, customAttributes), m_materialResourcesLoaded(false), skyboxTexture(nullptr), m_materialResourceInitialized(false)
{
}

void Horizon::DefaultSkyboxMaterial::BindUniforms()
{
	Material::BindUniforms();
}

void Horizon::DefaultSkyboxMaterial::BindTextures()
{
	if (m_materialResourcesLoaded == true && m_materialResourceInitialized == false)
	{
		if (skyboxTexture == nullptr && m_skyboxTextureResource != nullptr)
		{
			m_skyboxTextureResource->ResourceInit(ResourceManager::GetInstance()->GetResourceAttributes(m_skyboxTextureResource->GetResourceId()->GetId()));
			skyboxTexture = m_skyboxTextureResource->texture;
		}
		m_materialResourceInitialized = true;
	}

	Material::BindTextures();

	std::vector<Texture*> textures;
	textures.reserve(1);

	textures.push_back(skyboxTexture);

	glDepthMask(GL_FALSE);

	BindActivateTextures(textures);
}

void Horizon::DefaultSkyboxMaterial::InitializeMaterial()
{

}

void Horizon::DefaultSkyboxMaterial::MaterialResourceLoadComplete()
{
	m_materialResourcesLoaded = true;
}

bool Horizon::DefaultSkyboxMaterial::InitializeMaterialResources()
{	
	uint32 resourceSetId = ResourceManager::GetInstance()->CreateResourceSet(GetMaterialId(), std::bind(std::mem_fn(&DefaultSkyboxMaterial::MaterialResourceLoadComplete), this));

	for (auto it = m_customAttributes.begin(); it != m_customAttributes.end(); ++it)
	{
		auto attributeEntry = (*it);
		auto attribute = attributeEntry.second;

		if (attribute.attributeType == "TEXTURECUBE" && attribute.name == "M_SkyboxTexture")
		{
			std::string resourceId = attribute.value.get<std::string>();
			m_skyboxTextureResource = EngineAllocateC(TextureCubeResource, TextureCubeResource(resourceId));

			ResourceManager::GetInstance()->AddResource(m_skyboxTextureResource, resourceSetId);
		}
	}

	return true;
}

void Horizon::DefaultSkyboxMaterial::UnbindMaterial()
{
	std::vector<Texture*> textures;
	textures.reserve(1);

	textures.push_back(skyboxTexture);

	UnbindActivateTextures(textures);

	glDepthMask(GL_TRUE);
}
