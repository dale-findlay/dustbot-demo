#pragma once

#include "core\ExportMacro.h"
#include "core\TypeAbstractions.h"
#include "structures\Vertex1P1U1N.h"
#include "structures\MaterialData.h"

#include <vector>

namespace Horizon
{
	struct HORIZON_API  MeshData
	{
		std::vector<Vertex1P1U1N> vertices;
		std::vector<unsigned int> indices;
		//std::vector<MaterialData> defaultMaterials;
	};
}