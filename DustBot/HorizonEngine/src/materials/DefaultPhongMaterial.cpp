#include "DefaultPhongMaterial.h"

#include "subsystems\EngineAllocator.h"
#include "subsystems\ResourceManager.h"
#include "structures\MaterialCustomAttribute.h"
#include "structures\ResourceLoadState.h"
#include "platform\Texture.h"
#include "platform\GLHelper.h"

#include <vector>

using namespace Horizon::Graphics;

Horizon::DefaultPhongMaterial::DefaultPhongMaterial(const std::string & materialId, ShaderProgram * shaderProgram, const std::unordered_map<std::string, MaterialCustomAttribute>& customAttributes)
	: Material(materialId, shaderProgram, customAttributes), m_materialResourcesLoaded(false), diffuseTexture(nullptr), normalTexture(nullptr), specularTexture(nullptr), m_materialResourceInitialized(false)
{
}

void Horizon::DefaultPhongMaterial::BindUniforms()
{
	Material::BindUniforms();

	m_shaderProgram->BindUniform("albedoIntensity", albedoIntensity);
	m_shaderProgram->BindUniform("specularScale", specularScale);
	m_shaderProgram->BindUniform("shininess", shininess);
	m_shaderProgram->BindUniform("roughness", roughness);
}

void Horizon::DefaultPhongMaterial::BindTextures()
{
	if (m_materialResourcesLoaded == true && m_materialResourceInitialized == false)
	{
		if (diffuseTexture == nullptr && m_diffuseTexture != nullptr)
		{
			m_diffuseTexture->ResourceInit(ResourceManager::GetInstance()->GetResourceAttributes(m_diffuseTexture->GetResourceId()->GetId()));
			diffuseTexture = m_diffuseTexture->texture;
		}
		m_materialResourceInitialized = true;
	}

	Material::BindTextures();

	std::vector<Texture*> textures;
	textures.reserve(3);

	textures.push_back(diffuseTexture);
	textures.push_back(normalTexture);
	textures.push_back(specularTexture);

	BindActivateTextures(textures);
}

void Horizon::DefaultPhongMaterial::InitializeMaterial()
{

}

void Horizon::DefaultPhongMaterial::MaterialResourceLoadComplete()
{
	m_materialResourcesLoaded = true;
}

bool Horizon::DefaultPhongMaterial::InitializeMaterialResources()
{	
	uint32 resourceSetId = ResourceManager::GetInstance()->CreateResourceSet(GetMaterialId(), std::bind(std::mem_fn(&DefaultPhongMaterial::MaterialResourceLoadComplete), this));

	for (auto it = m_customAttributes.begin(); it != m_customAttributes.end(); ++it)
	{
		auto attributeEntry = (*it);
		auto attribute = attributeEntry.second;

		if (attribute.attributeType == "TEXTURE2D")
		{
			std::string resourceId = attribute.value.get<std::string>();
			m_diffuseTexture = EngineAllocateC(Texture2DResource, Texture2DResource(resourceId));

			ResourceManager::GetInstance()->AddResource(m_diffuseTexture, resourceSetId);
		}
	}

	return true;
}

void Horizon::DefaultPhongMaterial::UnbindMaterial()
{
	std::vector<Texture*> textures;
	textures.reserve(3);

	textures.push_back(diffuseTexture);
	textures.push_back(normalTexture);
	textures.push_back(specularTexture);

	UnbindActivateTextures(textures);
}
