#include "ResourceManager.h"

#include "subsystems\EngineAllocator.h"
#include "subsystems\VirtualFileSystem.h"
#include "utilities\Logger.h"
#include "utilities\IOUtility.h"

#include <mutex>
#include <thread>
#include <sstream>

#include <nlohmann\json.hpp>

Horizon::ResourceManager::ResourceManager()
	: m_threadCount(2)
{}

bool Horizon::ResourceManager::Init()
{
	if (!IOUtility::FileExists("~/resource_index.json"))
	{
		std::string assetsDir = VirtualFileSystem::GetInstance()->ResolveFilePath("~/");

		Logger::GetInstance()->LogError("Resource Index not found. Please ensure that a file called resource_index.json is present in the directory: " + assetsDir);
		return false;
	}

	std::string resourceIndexJson = IOUtility::ReadFileToEnd("~/resource_index.json");

	nlohmann::json doc = nlohmann::json::parse(resourceIndexJson);

	const auto& resourceTypes = doc["resourceTypes"];
	assert(resourceTypes.is_array());
	size_t count = resourceTypes.size();

	for (auto it = resourceTypes.begin(); it != resourceTypes.end(); ++it)
	{
		const auto& resourceObject = (*it);

		assert(IOUtility::ContainsMember(resourceObject, "id"));
		assert(IOUtility::ContainsMember(resourceObject, "name"));

		uint32 id = resourceObject["id"].get<uint32>();
		std::string name = resourceObject["name"].get<std::string>();

		m_resourceTypes[id] = name;
		m_resourceTypesNameIdMap[name] = id;
	}

	const auto& resources = doc["resources"];
	assert(resources.is_array());
	size_t resourceCount = resources.size();

	for (auto resourceIterator = resources.begin(); resourceIterator != resources.end(); ++resourceIterator)
	{
		const auto& resource = *resourceIterator;
		assert(IOUtility::ContainsMember(resource, "resourceId"));
		assert(IOUtility::ContainsMember(resource, "resourceType"));
		assert(IOUtility::ContainsMember(resource, "resourcePath"));

		ResourceData resourceData;

		std::string resourceId = resource["resourceId"].get<std::string>();
		uint32 resourceTypeId = resource["resourceType"].get<uint32>();
		std::string resourcePath = resource["resourcePath"].get<std::string>();

		resourceData.resourceId = resourceId;
		resourceData.resourceTypeId = resourceTypeId;
		resourceData.resourcePath = resourcePath;

		std::unordered_map<std::string, nlohmann::json::value_type> resourceAttributes;

		for (auto resourceAttributeIterator = resource.begin(); resourceAttributeIterator != resource.end(); ++resourceAttributeIterator)
		{
			resourceAttributes[resourceAttributeIterator.key()] = resourceAttributeIterator.value();
		}

		resourceAttributes.erase("resourceId");
		resourceAttributes.erase("resourceType");
		resourceAttributes.erase("resourcePath");

		resourceData.resourceAttributes = resourceAttributes;

		m_resourceIndex[resourceId] = resourceData;
	}

	for (uint32 i = 0; i < m_threadCount; ++i)
	{
		m_threads.emplace_back([this] { this->ThreadLoopBody(); });
	}

	m_threadActive = true;
	return true;
}

void Horizon::ResourceManager::Shutdown()
{
	m_threadActive = false;

	for (auto it = m_threads.begin(); it != m_threads.end(); ++it)
	{
		it->join();
	}
}

Horizon::uint32 Horizon::ResourceManager::CreateResourceSet(const std::string& name, std::function<void(void)>  completeCallback)
{
	m_resourceSetsMutex.lock();
	uint32 id = (unsigned int)m_resourceSets.size() + 1;

	m_resourceSets.push_back({ id, name, completeCallback });
	
	m_resourceSetsMutex.unlock();
	return id;
}

void Horizon::ResourceManager::CreateResourceCreateMapping(const std::string& resourceName, std::function<Resource*(std::unordered_map<std::string, std::string>)> creationFunction)
{
	if (m_resourceTypesNameIdMap.count(resourceName) == 0)
	{
		Logger::GetInstance()->LogError("Failed to create Component name mapping. Component name " + resourceName + std::string(" was not present in the component index"));
		return;
	}

	const uint32& componentTypeId = m_resourceTypesNameIdMap[resourceName];

	if (m_mappedResources.count(componentTypeId) > 0)
	{
		Logger::GetInstance()->LogWarning("Did not create Resource name mapping. Resource name " + resourceName + std::string(" a mapping already exists."));
		return;
	}

	m_mappedResources[componentTypeId] = creationFunction;
}

std::string Horizon::ResourceManager::GetResourceFileName(ResourceId* resourceId)
{
	if (resourceId == nullptr)
	{
		Logger::GetInstance()->LogError("Failed to retrieve resource file name. resourceId was null.");
		return "";
	}

	for (auto it = m_resourceIndex.begin(); it != m_resourceIndex.end(); ++it)
	{
		if ((*it).second.resourceId == resourceId->GetId())
		{
			return (*it).second.resourcePath;
		}
	}

	Logger::GetInstance()->LogError("Failed to retrieve resource file name. A resource with the resourceId :" + resourceId->GetId() + " did not exist in the resource_index");
	return "";
}

std::atomic<Horizon::uint32>* Horizon::ResourceManager::AddResource(Resource* resource, uint32 resourceSetId)
{
	if (resource == nullptr)
	{
		Logger::GetInstance()->LogError("ResourceManager could not add resource it was null.");
		return nullptr;
	}

	if (resourceSetId != 0)
	{
		bool resourceSetFound = false;

		for (auto it = m_resourceSets.begin(); it != m_resourceSets.end(); ++it)
		{
			if ((*it).id == resourceSetId)
			{
				resourceSetFound = true;
				break;
			}
		}

		if (!resourceSetFound)
		{
			Logger::GetInstance()->LogError("ResourceSet with Id: " + std::to_string(resourceSetId) + " could not be found.");
			return nullptr;
		}

		m_resourceSetData[resource->m_resourceId->GetId()] = resourceSetId;
	}

	std::atomic<uint32>* resourceCounter = EngineAllocateC(std::atomic<uint32>, std::atomic<uint32>());
	resource->m_resourceCounter = resourceCounter;

	m_atomicCounterMutex.lock();
	m_atomicCounters[resource->GetResourceId()->GetId()] = resourceCounter;
	m_atomicCounterMutex.unlock();

	m_queueMutex.lock();
	m_resourceQueue.push_back(resource);
	m_queueMutex.unlock();

	m_resources.push_back(resource);

	ResourceManager::GetInstance()->SetResourceLoadState(resource->m_resourceId->GetId(), ResourceLoadState::Created);
	return resourceCounter;
}

Horizon::Resource* Horizon::ResourceManager::GetResource(const std::string& resourceId)
{
	Resource* resource = nullptr;

	for (auto it = m_resources.begin(); it != m_resources.end(); ++it)
	{
		if ((*it)->m_resourceId->GetId() == resourceId)
		{
			resource = *it;
		}
	}	
	return resource;
}

std::vector<std::atomic<Horizon::uint32>*> Horizon::ResourceManager::AddResources(const std::vector<Resource*>& resources, std::function<void(void)> callback, uint32 resourceSetId)
{
	std::vector<std::atomic<uint32>*> resourceCounters;
	resourceCounters.reserve(resources.size());

	for (auto it = resources.begin(); it != resources.end(); ++it)
	{
		resourceCounters.push_back(AddResource(*it, resourceSetId));

		if (it + 1 == resources.end())
		{
			m_resourceCallbacks[(*it)->m_resourceId->GetId()].push_back(callback);
		}
	}



	return resourceCounters;
}

Horizon::uint32 Horizon::ResourceManager::GetIncompleteResourcesCount()
{
	m_resourceLoadMutex.lock();

	uint32 incompleteResources = 0;

	for (auto it = m_resourceLoadState.begin(); it != m_resourceLoadState.end(); ++it)
	{
		if ((*it).second != ResourceLoadState::Complete)
		{
			incompleteResources++;
		}
	}

	m_resourceLoadMutex.unlock();

	return incompleteResources;
}

void Horizon::ResourceManager::SetResourceLoadState(const std::string & resourceId, ResourceLoadState resourceLoadState)
{
	m_resourceLoadMutex.lock();
	m_resourceLoadState[resourceId] = resourceLoadState;
	m_resourceLoadMutex.unlock();
}

Horizon::ResourceLoadState Horizon::ResourceManager::GetResourceLoadState(const std::string & resourceId)
{
	ResourceLoadState state;

	m_resourceLoadMutex.lock();
	state = m_resourceLoadState[resourceId];
	m_resourceLoadMutex.unlock();

	return state;
}

const std::unordered_map<std::string, nlohmann::json::value_type> Horizon::ResourceManager::GetResourceAttributes(const std::string resourceId) const
{
	if (!m_resourceIndex.count(resourceId))
	{
		Logger::GetInstance()->LogError("GetResourceAttributes() - Failed to find a resource with the Id of " + resourceId
			+ " in the resourceIndex.");
		return std::unordered_map<std::string, nlohmann::json::value_type>();
	}

	return m_resourceIndex.at(resourceId).resourceAttributes;
}

using namespace std::chrono_literals;
void Horizon::ResourceManager::ThreadLoopBody()
{
	std::stringstream ss;
	ss << "Thread " << std::this_thread::get_id() << " starting.";
	Logger::GetInstance()->LogWarning(ss.str());

	while (ThreadActive())
	{
		uint32 queueCount = 0;
		Resource* resource = nullptr;
		uint32 resourceSetId = 0;

		m_queueMutex.lock();
		queueCount = (unsigned int)m_resourceQueue.size();
		if (queueCount > 0)
		{
			resource = m_resourceQueue.front();
			m_resourceQueue.pop_front();
		}

		m_queueMutex.unlock();

		if (resource != nullptr)
		{
			std::string resourceId = resource->m_resourceId->GetId();
			std::string fileName = GetResourceFileName(resource->m_resourceId);
			resource->LoadResource(fileName);
			
			ResourceManager::GetInstance()->SetResourceLoadState(resourceId, ResourceLoadState::Complete);
			std::this_thread::sleep_for(0.5s);

			/* Resource 'groups' i.e anything created with AddResources.
			The last one in the list is the one that has the callback attached. */
			if (m_resourceCallbacks.count(resourceId) > 0)
			{
				std::vector<std::function<void(void)>> callbacks = m_resourceCallbacks[resourceId];

				for (auto it = callbacks.begin(); it != callbacks.end(); ++it)
				{
					(*it)();
				}
			}

			//Resource set callbacks.
			if (m_resourceSetData.count(resourceId) > 0)
			{
				m_resourceSetsMutex.lock();
				resourceSetId = m_resourceSetData[resourceId];
				
				ResourceSet resourceSet;
				bool resourceSetFound = false;

				std::vector<ResourceSet>::iterator resourceSetIterator;				
				
				for (auto it = m_resourceSets.begin(); it != m_resourceSets.end(); ++it)
				{
					if ((*it).id == resourceSetId)
					{
						resourceSetIterator = it;
						resourceSet = (*it);
						resourceSetFound = true;
						break;
					}
				}

				if (resourceSetFound)
				{
					resourceSet.completeCallback();
				}

				m_resourceSetsMutex.unlock();
			}
		}
		else
		{
			std::this_thread::sleep_for(0.5s);
		}
	}

	ss = std::stringstream();

	ss << "Thread " << std::this_thread::get_id() << " done.";

	Logger::GetInstance()->LogWarning(ss.str());
}

bool Horizon::ResourceManager::ThreadActive()
{
	return m_threadActive;
}

