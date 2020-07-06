#pragma once

#include "core\ExportMacro.h"
#include "core\TypeAbstractions.h"
#include "core\Subsystem.h"
#include "components\CameraComponent.h"

namespace Horizon
{
	//This subsystem keeps track of the game's main camera. 
	//It will also facilitate multiple scene renderings (generating cube maps for example).
	class HORIZON_API CameraManager : public Subsystem<CameraManager>
	{
	public:
		bool Init();
		void Shutdown();

		void SetActiveCameraComponent(CameraComponent* cameraComponent);

		CameraComponent* GetActiveCamera();
	private:
		//This is the camera who's view & projection matrix will be fed to shaders.
		CameraComponent* m_activeCameraComponent;
	};
}
