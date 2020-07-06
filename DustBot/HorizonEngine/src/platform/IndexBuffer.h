#pragma once

#include "core\ExportMacro.h"
#include "core\TypeAbstractions.h"
#include "platform\Buffer.h"

namespace Horizon
{
	namespace Graphics
	{
		class HORIZON_API IndexBuffer : public Buffer
		{
		public:
			IndexBuffer(unsigned int* data, size_t size, const uint32 count);

			uint32 GetIndexCount() const;

		private:
			uint32 m_indexCount;
		};
	}
}
