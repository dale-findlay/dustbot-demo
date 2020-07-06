#pragma once

#include "core\ExportMacro.h"
#include "core\TypeAbstractions.h"

#include <string>

namespace Horizon
{
	/*
		Read from the material_index.json file.
		Used to identify and validate materials during creation.	
	*/
	struct HORIZON_API  MaterialType
	{
		MaterialType() :
			id(100000000)
		{}

		MaterialType(uint32 id, std::string name, std::string shaderLocation)
			: id(id), name(name), shaderLocation(shaderLocation)
		{}
		
		
		uint32 id;
		std::string name;
		std::string shaderLocation;
	};
}