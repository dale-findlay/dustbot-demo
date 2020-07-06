#pragma once

#include "core\Subsystem.h"
#include "core\GameObject.h"

#include <vector>

namespace Horizon
{
	class GameObject;

	class HORIZON_API GameObjectFactory : public Subsystem<GameObjectFactory>
	{
	public:
		GameObjectFactory();

		GameObject* CreateNewGameObject(std::string name);

		bool Init();

		//Cleans up any GameObjects.
		void Shutdown();

	public:
		std::vector<GameObject*> m_gameObjects;
	};
}
