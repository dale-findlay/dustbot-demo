#pragma once

#include "core\ExportMacro.h"
#include "core\Subsystem.h"
#include "core\TypeAbstractions.h"

#include <map>
#include <queue>

namespace Horizon
{
	class HORIZON_API Renderer : public Subsystem<Renderer>
	{
	public:
		bool Init();
		void Shutdown();
		void Draw();

	private:
		void DrawLoadingScreen();
		void DrawNoActiveCamera();
	};

}
