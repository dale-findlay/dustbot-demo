#pragma once

#include "core\ExportMacro.h"
#include "core\Subsystem.h"
#include "core\TypeAbstractions.h"
#include "core\ResourceData.h"
#include "structures\ResourceSet.h"
#include "structures\ResourceLoadState.h"
#include "resources\Resource.h"

#include <atomic>
#include <map>
#include <queue>
#include <deque>
#include <unordered_map>
#include <string>
#include <thread>
#include <mutex>
#include <functional>

namespace Horizon
{	
	class Resource;

	class HORIZON_API ResourceManager : public Subsystem<ResourceManager>
	{
	public:
		ResourceManager();

		bool Init();
		void Shutdown();
		
		uint32 CreateResourceSet(const std::string& name, std::function<void()> completeCallback);
		void CreateResourceCreateMapping(const std::string& resourceName, std::function<Resource*(std::unordered_map<std::string, std::string>)> creationFunction);
		
		std::atomic<uint32>* AddResource(Resource* resource, uint32 resourceSetId = 0);
		Resource* GetResource(const std::string& resourceId);
		std::vector<std::atomic<uint32>*> AddResources(const std::vector<Resource*>& resources, std::function<void(void)>  callback, uint32 resourceSetId = 0);

		std::string GetResourceFileName(ResourceId* resourceId);
		uint32 GetIncompleteResourcesCount();
		void SetResourceLoadState(const std::string& resourceId, ResourceLoadState resourceLoadState);
		ResourceLoadState GetResourceLoadState(const std::string& resourceId);

		const std::unordered_map<std::string, nlohmann::json::value_type> GetResourceAttributes(const std::string resourceId) const;

	private:

		std::vector<Resource*> m_resources;

		//Component Type Id -> Name
		std::unordered_map<uint32, std::string> m_resourceTypes;
		std::unordered_map<std::string, uint32> m_resourceTypesNameIdMap;

		//Component Type Id -> Create Function
		std::unordered_map<uint32, std::function<Resource*(std::unordered_map<std::string, std::string>)>> m_mappedResources;

		//Resource Id -> ResourceData (type, and other attributes)
		std::unordered_map<std::string, ResourceData> m_resourceIndex;

		std::vector<ResourceSet> m_resourceSets;

		//ResourceId resourceSet.
		std::unordered_map<std::string, uint32> m_resourceSetData;
		std::unordered_map<std::string, ResourceLoadState> m_resourceLoadState;

		std::unordered_map<std::string, std::vector<std::function<void(void)>>> m_resourceCallbacks;

		/*
			Threading Stuff....
		*/

		uint32 m_threadCount;

		std::vector<std::thread> m_threads;

		void ThreadLoopBody();
		bool ThreadActive();

		std::deque<Resource*> m_resourceQueue;
		std::map<std::string, std::atomic<uint32>*> m_atomicCounters;

		std::atomic<bool> m_threadActive;

		std::mutex m_queueMutex;
		std::mutex m_resourceLoadMutex;
		std::mutex m_resourceSetsMutex;
		std::mutex m_atomicCounterMutex;

	};
}
