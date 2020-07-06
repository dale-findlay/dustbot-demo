#include "Material.h"

#include "core\GameObject.h"
#include "subsystems\CameraManager.h"
#include "components\CameraComponent.h"
#include "platform\GLHelper.h"

#include <assert.h>

using namespace Horizon::Graphics;

Horizon::Material::Material(const std::string& materialId, ShaderProgram* shaderProgram, const std::unordered_map<std::string, MaterialCustomAttribute>& customAttributes)
	: m_materialId(materialId), m_shaderProgram(shaderProgram), m_customAttributes(customAttributes)
{
	assert(m_shaderProgram != nullptr);
}

Horizon::Material::~Material()
{}

void Horizon::Material::BindMaterial()
{
	m_shaderProgram->Use();

	BindTextures();
	BindUniforms();
}

void Horizon::Material::UnbindMaterial()
{

}

Horizon::Graphics::ShaderProgram* Horizon::Material::GetShaderProgram()
{
	return m_shaderProgram;
}

std::string Horizon::Material::GetMaterialId() const
{
	return m_materialId;
}

void Horizon::Material::BindTextures()
{}

void Horizon::Material::BindUniforms()
{
	CameraComponent* activeCamera = CameraManager::GetInstance()->GetActiveCamera();

	if (activeCamera)
	{
		m_shaderProgram->BindUniform("cameraPosition", activeCamera->gameObject->transform.Position());
		m_shaderProgram->BindUniform("view", activeCamera->GetViewMatrix());
		m_shaderProgram->BindUniform("projection", activeCamera->GetProjectionMatrix());
	}
}

void Horizon::Material::BindActivateTextures(const std::vector<Texture*>& textures)
{
	for (int i = 0; i < textures.size(); ++i)
	{
		Texture* texture = textures[i];

		if (texture)
		{
			GL(glActiveTexture(GL_TEXTURE0 + i));
			texture->Bind();
		}
	}
}

void Horizon::Material::UnbindActivateTextures(const std::vector<Texture*>& textures)
{
	for (int i = 0; i < textures.size(); ++i)
	{
		Texture* texture = textures[i];

		if (texture)
		{
			GL(glActiveTexture(GL_TEXTURE0 + i));
			texture->Unbind();
		}
	}
}

