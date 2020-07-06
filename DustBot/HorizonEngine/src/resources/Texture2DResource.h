#pragma once

#include "core\ExportMacro.h"
#include "resources\Resource.h"
#include "platform\Texture2D.h"
#include "platform\structures\TextureData.h"

namespace Horizon
{
	class HORIZON_API Texture2DResource : public Resource
	{
	public:
		Texture2DResource(const std::string& resourceId);

		// Inherited via Resource
		virtual bool ResourceLoad(std::string fileName) override;
		virtual bool ResourceInit(const std::unordered_map<std::string, nlohmann::json::value_type>& resourceAttributes) override;
		virtual void ResourceShutdown() override;

		Graphics::Texture2D* texture;
	private:
		Graphics::TextureData m_textureData;
	};
}