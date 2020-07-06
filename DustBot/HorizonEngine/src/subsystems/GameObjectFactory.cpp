#include "GameObjectFactory.h"

#include "subsystems\EngineAllocator.h"
#include "core\GameObject.h"

Horizon::GameObjectFactory::GameObjectFactory()
{
}

Horizon::GameObject* Horizon::GameObjectFactory::CreateNewGameObject(std::string name)
{
	GameObject* newGameObject = EngineAllocateC(GameObject,GameObject((uint32)m_gameObjects.size() + 1, name));

	m_gameObjects.push_back(newGameObject);

	return newGameObject;
}

bool Horizon::GameObjectFactory::Init()
{
	return true;
}

void Horizon::GameObjectFactory::Shutdown()
{

}
