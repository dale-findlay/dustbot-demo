#pragma once
#include "core\ExportMacro.h"
#include "core\TypeAbstractions.h"
#include "platform\Buffer.h"

namespace Horizon
{
	namespace Graphics
	{
		class HORIZON_API VertexBuffer : public Buffer
		{
		public:
			VertexBuffer(const void* data, size_t size, uint32 count);

			uint32 GetVertexCount() const;

		private:
			uint32 m_vertexCount;

		};
	}
}
