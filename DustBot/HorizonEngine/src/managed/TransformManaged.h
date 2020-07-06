#pragma once

#include "core\ExportMacro.h"

#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>

namespace Horizon
{
	namespace Managed
	{
		struct HORIZON_API  TransformManaged
		{
			glm::vec4 position;
			glm::vec4 eulerRotation;
			glm::vec4 scale;
			glm::quat rotation;

			glm::vec4 forward;
			glm::vec4 right;
		};
	}
}