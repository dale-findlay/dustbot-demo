#include "GameObject.h"

#include "components\DrawableComponent.h"

Horizon::GameObject::GameObject(const uint32 & id, const std::string & name)
	: m_id(id), m_name(name), m_active(true)
{}

Horizon::GameObject::~GameObject()
{
}

Horizon::uint32 Horizon::GameObject::GetId()
{
	return m_id;
}

std::string Horizon::GameObject::GetName() const
{
	return m_name;
}

bool Horizon::GameObject::GetActive() const
{
	return m_active;
}

void Horizon::GameObject::SetActive(const bool & visible)
{
	m_active = visible;
}

bool Horizon::GameObject::AddComponent(Component* component)
{
	if (component == nullptr)
	{
		Logger::GetInstance()->LogError("You may not add a component that was null.");
		return false;
	}

	if (dynamic_cast<DrawableComponent*>(component) != nullptr)
	{
		m_drawable = true;
	}

	ComponentId componentId = (ComponentId)m_components.size();
	component->m_id = componentId;
	component->gameObject = this;
	m_components[componentId] = component;

	return true;
}

void Horizon::GameObject::Start()
{
	for (auto it = m_components.begin(); it != m_components.end(); ++it)
	{
		(*it).second->StartComponent();
	}
}

void Horizon::GameObject::Update()
{
	if (m_active)
	{
		for (auto it = m_components.begin(); it != m_components.end(); ++it)
		{
			(*it).second->PreUpdateComponent();
			(*it).second->UpdateComponent();
		}
	}
}

bool Horizon::GameObject::IsDrawable() const
{
	return m_drawable;
}