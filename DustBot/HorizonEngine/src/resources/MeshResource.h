#pragma once

#include "core\ExportMacro.h"
#include "resources\Resource.h"
#include "structures\MeshData.h"
#include "structures\GLIndexedDrawable.h"

#include <string>
#include <vector>

namespace Horizon
{
	class HORIZON_API MeshResource : public Resource 
	{
	public:
		MeshResource(const std::string resourceId);

		// Inherited via Resource
		virtual bool ResourceLoad(std::string fileName) override;
		std::vector<Horizon::MeshData> LoadMesh(const std::string & fileName);
		virtual bool ResourceInit(const std::unordered_map<std::string, nlohmann::json::value_type>& resourceAttributes) override;
		virtual void ResourceShutdown() override;

		std::vector<GLIndexedDrawable*> glIndexedDrawables;
		std::vector<MeshData> meshData;

	private:
		std::string GetMaterialIdFromMaterialIndex(const uint32& index, const std::vector<std::string>& materialIds);
	};
}