#include "TextureCubeResource.h"

#include "subsystems\EngineAllocator.h"
#include "subsystems\VirtualFileSystem.h"
#include "utilities\IOUtility.h"

#include <experimental\filesystem>
#include <stb\stb_image.h>

namespace fs = std::experimental::filesystem;

using namespace Horizon::Graphics;

Horizon::TextureCubeResource::TextureCubeResource(const std::string & resourceId)
	: Resource(resourceId)
{}

bool Horizon::TextureCubeResource::ResourceLoad(std::string fileName)
{
	std::vector<std::string> textureFaces = GetTextureFaces(fileName);

	for (auto it = textureFaces.begin(); it != textureFaces.end(); ++it)
	{
		TextureData textureData = TextureData();
		textureData.textureWidth = 0;
		textureData.textureHeight = 0;
		textureData.internalFormat = GL_RGBA;
		textureData.textureDataFormat = GL_RGBA;
		textureData.textureDataType = GL_UNSIGNED_BYTE;

		int texChannels = 0;

		std::string fileName = (*it);

		textureData.textureData = stbi_load(fileName.c_str(), &textureData.textureWidth, &textureData.textureHeight, &texChannels, STBI_rgb_alpha);

		m_faces.push_back(textureData);
	}

	return true;
}

bool Horizon::TextureCubeResource::ResourceInit(const std::unordered_map<std::string, nlohmann::json::value_type>& resourceAttributes)
{
	texture = EngineAllocateC(TextureCube, TextureCube(m_faces));
	texture->SetTextureParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	texture->SetTextureParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	texture->SetTextureParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	texture->SetTextureParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	texture->SetTextureParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return true;
}

void Horizon::TextureCubeResource::ResourceShutdown()
{
	for (auto it = m_faces.begin(); it != m_faces.end(); ++it)
	{
		stbi_image_free((*it).textureData);
	}
}

std::vector<std::string> Horizon::TextureCubeResource::GetTextureFaces(const std::string& fileName)
{
	std::vector<std::string> faces;
	faces.reserve(6);

	std::string resolvedPath = VirtualFileSystem::GetInstance()->ResolveFilePath(fileName);
	fs::path p = fs::path(resolvedPath);
	std::string baseFilePath = p.parent_path().string() + "\\";


	std::string cubeResourceJson = IOUtility::ReadFileToEnd(fileName);
	nlohmann::json doc = nlohmann::json::parse(cubeResourceJson);

	assert(IOUtility::ContainsMember(doc, "cubeResource"));

	const auto& cubeResource = doc["cubeResource"];
	assert(cubeResource.is_object());
	assert(cubeResource.size() == 6);

	std::string rightFaceFile = cubeResource["right"].get<std::string>();
	std::string leftFaceFile = cubeResource["left"].get<std::string>();
	std::string topFaceFile = cubeResource["top"].get<std::string>();
	std::string bottomFaceFile = cubeResource["bottom"].get<std::string>();
	std::string frontFaceFile = cubeResource["front"].get<std::string>();
	std::string backFaceFile = cubeResource["back"].get<std::string>();

	faces.push_back(baseFilePath + rightFaceFile);
	faces.push_back(baseFilePath + leftFaceFile);
	faces.push_back(baseFilePath + topFaceFile);
	faces.push_back(baseFilePath + bottomFaceFile);
	faces.push_back(baseFilePath + frontFaceFile);
	faces.push_back(baseFilePath + backFaceFile);

	return faces;
}
