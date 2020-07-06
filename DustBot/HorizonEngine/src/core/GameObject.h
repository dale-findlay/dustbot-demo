#pragma once

#include "core\ExportMacro.h"
#include "core\ComponentId.h"
#include "core\TypeAbstractions.h"
#include "structures\Transform.h"
#include "components\Component.h"
#include "utilities\Logger.h"

#include <map>
#include <vector>

namespace Horizon
{
	/*
		Represents a single arbitrary object within a scene. 
	*/
	class HORIZON_API GameObject
	{
		friend class GameObjectFactory;

	public:
		GameObject(): m_active(true) {}
		~GameObject();

	protected:
		GameObject(const uint32& id, const std::string& name);
	
	public:	
		uint32 GetId();
		std::string GetName() const;

		//Return whether a game object is actively being updated.
		bool GetActive() const;

		//Set whether the engine should actively update the object (calling child component UpdateComponent methods).
		void SetActive(const bool& visible);

		//Add a component to the game object.
		bool AddComponent(Component* component);
		
		void Start();
		void Update();
		
		//Return true if a drawable component is attached to the object.
		bool IsDrawable() const;
		
		//Return a list of components attached to the game object of type T.
		template<class T>
		std::vector<T> GetComponents();

		Transform transform;

	private:		
		bool m_drawable;
		bool m_active;
		uint32 m_id;
		std::string m_name;

		//The length of a vector between 
		float32 m_distanceFromCamera;
		
		std::map<ComponentId, Component*> m_components;
	};

	template<class T>
	inline std::vector<T> GameObject::GetComponents()
	{
		std::vector<T> components =std::vector<T>();

		for (auto it = m_components.begin(); it != m_components.end(); ++it)
		{
			if (dynamic_cast<T>((*it).second) != nullptr)
			{
				components.push_back(dynamic_cast<T>((*it).second));
			}
		}

		return components;
	}
}