#include "Texture2DResource.h"

#include "subsystems\EngineAllocator.h"
#include "subsystems\VirtualFileSystem.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb\stb_image.h>

using namespace Horizon::Graphics;

Horizon::Texture2DResource::Texture2DResource(const std::string & resourceId)
	: Resource(resourceId)
{}

bool Horizon::Texture2DResource::ResourceLoad(std::string fileName)
{
	std::string resolvedFilePath = VirtualFileSystem::GetInstance()->ResolveFilePath(fileName);
	m_textureData = TextureData();
	
	m_textureData.textureWidth = 0;
	m_textureData.textureHeight = 0;
	
	int texChannels = 0;

	m_textureData.textureData = stbi_load(resolvedFilePath.c_str(), &m_textureData.textureWidth, &m_textureData.textureHeight, &texChannels, STBI_rgb_alpha);
	m_textureData.internalFormat = GL_RGBA;
	m_textureData.textureDataFormat = GL_RGBA;
	m_textureData.textureDataType = GL_UNSIGNED_BYTE;

	return true;
}

bool Horizon::Texture2DResource::ResourceInit(const std::unordered_map<std::string, nlohmann::json::value_type>& resourceAttributes)
{
	texture = EngineAllocateC(Texture2D, Texture2D(m_textureData));	
	texture->SetTextureParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	texture->SetTextureParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

	return true;
}

void Horizon::Texture2DResource::ResourceShutdown()
{
	stbi_image_free(m_textureData.textureData);
}
