#pragma once

#include "core\ExportMacro.h"
#include "resources\Resource.h"
#include "platform\TextureCube.h"
#include "platform\structures\TextureData.h"

namespace Horizon
{
	class HORIZON_API TextureCubeResource : public Resource
	{
	public:
		TextureCubeResource(const std::string& resourceId);

		// Inherited via Resource
		virtual bool ResourceLoad(std::string fileName) override;
		virtual bool ResourceInit(const std::unordered_map<std::string, nlohmann::json::value_type>& resourceAttributes) override;
		virtual void ResourceShutdown() override;

		Graphics::TextureCube* texture;
	private:

		std::vector<std::string> GetTextureFaces(const std::string& fileName);

		//right, left, top, bottom, front, back
		std::vector<Graphics::TextureData> m_faces;
	};
}