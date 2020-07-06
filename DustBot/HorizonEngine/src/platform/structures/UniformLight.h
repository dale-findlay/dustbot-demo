#pragma once

#include <glm\common.hpp>

namespace Horizon
{
	namespace Graphics
	{
		struct UniformLight {
			int lightType;
			glm::vec4 lightPos;
			glm::vec4 lightDirection;
			
			UniformLight(int lightType, glm::vec4 lightPos, glm::vec4 lightDirection)
				: lightType(lightType), lightPos(lightPos), lightDirection(lightDirection)
			{}
		};
	}
}