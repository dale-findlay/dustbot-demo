#pragma once
#include "core\Singleton.h"

namespace Horizon
{
	template<typename T>
	class Subsystem : public Singleton<T>
	{
	public:
		Subsystem() {}

		//Add some function that are shared with all subsystems.
	};
}