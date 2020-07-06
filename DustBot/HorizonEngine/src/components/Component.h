#pragma once

#include "core\ExportMacro.h"
#include "core\ComponentId.h"
#include "core\TypeAbstractions.h"
#include "subsystems\ComponentFactory.h"

#include <unordered_map>
#include <nlohmann\json.hpp>
#include <mutex>

namespace Horizon
{
	class GameObject;

	class HORIZON_API Component
	{
		friend class ComponentFactory;
		friend class GameObject;
	public:
		Component();
	
		//Called when the component is created. Before Init().
		virtual bool CreateComponent(const std::unordered_map<std::string, nlohmann::json::value_type>& componentAttributes) = 0;

		virtual bool InitComponent() = 0;
		virtual void ShutdownComponent() = 0;

		virtual void StartComponent() = 0;
		virtual void UpdateComponent() = 0;
		
		virtual void PreUpdateComponent();

		virtual void InitResourcesCallback();

		virtual void CreateResources() = 0;

		uint32 GetResourceCount() const;

		GameObject* gameObject;

		uint32 resourceSetId;
	protected:
		~Component();
		ComponentId m_id;


		bool m_initialized;

		std::mutex m_shouldInitMutex;
		bool m_shouldInit;

		//Used to count the number of resources potentially loaded by a component;
		uint32 m_resourceCount;
	};
}