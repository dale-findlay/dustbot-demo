#include "Component.h"

#include "core\GameObject.h"

Horizon::Component::Component()
	: resourceSetId(0), m_initialized(false), m_shouldInit(false)
{}

void Horizon::Component::PreUpdateComponent()
{
	m_shouldInitMutex.lock();
	if (m_shouldInit && !m_initialized)
	{
		InitComponent();
		m_initialized = true;
	}
	m_shouldInitMutex.unlock();
}

void Horizon::Component::InitResourcesCallback()
{
	m_shouldInitMutex.lock();
	m_shouldInit = true;
	m_shouldInitMutex.unlock();
}

Horizon::uint32 Horizon::Component::GetResourceCount() const
{
	return m_resourceCount;
}

Horizon::Component::~Component()
{

}
