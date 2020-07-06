#pragma once

#include "Core\TypeAbstractions.h"
#include "core\ExportMacro.h"
#include "Core\Singleton.h"

namespace Horizon
{
	/*
		A Utility for interfacing the the time measuring components within the engine.
	*/
	class HORIZON_API Timer : public Singleton<Timer>
	{
	public:
		bool Init();
		void Shutdown();

		//Called before a frame is updated.
		void PreUpdateTimer();

		//Called after a frame is updated.
		void PostUpdateTimer();

		//Calculates the frame delta time.
		float32 CalculateDeltaTime();
		
		//Calculates the current number of frames rendered per second as an integer.
		uint32 CalculateFramesPerSecond();

	private:
		float64 m_frequency;
		float64 m_secsPerCount;

		int64 m_preFrameCount;
		int64 m_postFrameCount;
	};
}
