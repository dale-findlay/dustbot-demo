#pragma once

#include "core\ExportMacro.h"
#include "core\TypeAbstractions.h"
#include "core\ResourceId.h"

#include <atomic>
#include <mutex>
#include <string>
#include <unordered_map>
#include <nlohmann\json.hpp>

namespace Horizon
{
	class ResourceManager;

	class HORIZON_API Resource
	{
	public:
		friend class ResourceManager;

		Resource(const std::string resourceId);
		~Resource();

		ResourceId* GetResourceId() const;
		
		//Function to actually load the data.
		bool LoadResource(std::string fileName);

		virtual bool ResourceLoad(std::string fileName) = 0;
		
		//Initialize any GL resources and other engine resources.
		virtual bool ResourceInit(const std::unordered_map<std::string, nlohmann::json::value_type>& resourceAttributes) = 0;

		//Shutdown any GL or engine resources and any other allocated memory in Init().
		virtual void ResourceShutdown() = 0;

	private:
		ResourceId* m_resourceId;
		std::atomic<uint32>* m_resourceCounter;
	};

}