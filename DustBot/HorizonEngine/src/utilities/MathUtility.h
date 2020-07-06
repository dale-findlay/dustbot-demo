#pragma once

#include "core\ExportMacro.h"

namespace Horizon
{
	class HORIZON_API MathUtility
	{
	public:
		static float ClampF(float f, float min, float max);
		static int Clamp(int i, int min, int max);
	};

}
