#include "Resource.h"

#include "subsystems\EngineAllocator.h"
#include "subsystems\ResourceManager.h"

Horizon::Resource::Resource(const std::string resourceId)
	: m_resourceId(EngineAllocateC(ResourceId, ResourceId(resourceId)))
{}

Horizon::Resource::~Resource()
{
	if (m_resourceId != nullptr)
	{
		delete m_resourceId;
		m_resourceId = nullptr;
	}
}

Horizon::ResourceId* Horizon::Resource::GetResourceId() const
{
	return m_resourceId;
}

bool Horizon::Resource::LoadResource(std::string fileName)
{
	ResourceManager::GetInstance()->SetResourceLoadState(m_resourceId->GetId(), ResourceLoadState::Loading);

	bool b = ResourceLoad(fileName);

	return b;
}
