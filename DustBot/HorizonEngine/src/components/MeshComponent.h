#pragma once

#include "core\ExportMacro.h"
#include "core\ResourceId.h"
#include "components\Component.h"
#include "components\DrawableComponent.h"
#include "resources\MeshResource.h"

#include <string>

namespace Horizon
{
	class HORIZON_API MeshComponent : public DrawableComponent
	{
	public:
		MeshComponent();

		// Inherited via DrawableComponent
		virtual bool InitComponent() override;
		virtual void UpdateComponent() override;
		virtual void ShutdownComponent() override;
		virtual void DrawComponent() override;

		virtual void CreateResources() override;

		// Inherited via Component
		virtual bool CreateComponent(const std::unordered_map<std::string, nlohmann::json::value_type>& componentAttributes) override;

	private:
		bool m_wireFrame;
		MeshResource* m_meshResource;
		std::string m_meshResourceId;
		std::vector<std::string> m_materialIds;
	};
}
