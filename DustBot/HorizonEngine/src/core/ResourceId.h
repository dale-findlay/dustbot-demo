#pragma once

#include "core\ExportMacro.h"

#include <string>

namespace Horizon
{
	class ResourceId
	{
	public:
		ResourceId(std::string resourceId);
		std::string GetId() const;

	private:
		std::string m_resourceId;
	};
}