#pragma once

#include "core\ExportMacro.h"
#include "utilities\HashUtility.h"

#include <glm\common.hpp>
#include <glm\gtx\hash.hpp>

namespace Horizon
{
	struct HORIZON_API  Vertex1P1U1N
	{
		glm::vec4 position;
		glm::vec2 texCoord;
		glm::vec4 normal;
			   
		bool operator==(const Vertex1P1U1N& other) const {
			return position == other.position && texCoord == other.texCoord && normal == other.normal;
		}
	};
}

namespace std {
	template<> struct hash<Horizon::Vertex1P1U1N> {
		size_t operator()(Horizon::Vertex1P1U1N const& vertex) const {
			size_t s = 0;
			Horizon::HashUtility::HashCombine(s, vertex.position);
			Horizon::HashUtility::HashCombine(s, vertex.texCoord);
			Horizon::HashUtility::HashCombine(s, vertex.normal);
			return s;
		}
	};
}
