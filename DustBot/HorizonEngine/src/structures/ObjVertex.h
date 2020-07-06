#pragma once

#include "core\ExportMacro.h"
#include "utilities\HashUtility.h"

#include <glm\common.hpp>

namespace Horizon
{
	struct HORIZON_API  ObjVertex
	{
		glm::vec4 position;
		glm::vec2 texCoord;
		glm::vec4 normal;
		int materialId;

		bool operator==(const ObjVertex& other) const {
			return position == other.position && texCoord == other.texCoord && normal == other.normal && materialId == other.materialId;
		}
	};
}

namespace std {
	template<> struct hash<Horizon::ObjVertex> {
		size_t operator()(Horizon::ObjVertex const& vertex) const {
			size_t s = 0;
			Horizon::HashUtility::HashCombine(s, vertex.position);
			Horizon::HashUtility::HashCombine(s, vertex.texCoord);
			Horizon::HashUtility::HashCombine(s, vertex.normal);
			Horizon::HashUtility::HashCombine(s, vertex.materialId);
			return s;
		}
	};
}
