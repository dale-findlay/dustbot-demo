#include "ResourceId.h"

Horizon::ResourceId::ResourceId(std::string resourceId)
	:m_resourceId(resourceId)
{
}

std::string Horizon::ResourceId::GetId() const
{
	return m_resourceId;
}
