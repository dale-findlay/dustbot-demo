#pragma once

#include "core\ExportMacro.h"
#include "core\TypeAbstractions.h"

#include <string>

namespace Horizon
{
	namespace Graphics
	{
		struct HORIZON_API  VertexAttribute
		{
			uint32 index;
			std::string name;

			VertexAttribute(const uint32& index, const std::string& name)
			{
				this->index = index;
				this->name = name;
			}
		};
	}
}