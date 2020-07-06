#pragma once

#include "core\ExportMacro.h"
#include "core\TypeAbstractions.h"
#include "managed\TransformManaged.h"

namespace Horizon
{
	namespace Managed
	{
		struct HORIZON_API GameObjectManaged
		{
			uint32 m_gameObjectId;
			TransformManaged transform;
		};
	}
}