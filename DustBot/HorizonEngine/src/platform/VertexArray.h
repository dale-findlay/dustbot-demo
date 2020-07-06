#pragma once

#include "core\ExportMacro.h"
#include "platform\VertexBuffer.h"
#include "platform\VertexBufferLayout.h"

#include <GL\glew.h>
#include <vector>

namespace Horizon
{
	namespace Graphics
	{
		class HORIZON_API VertexArray
		{
		public:
			VertexArray();
			~VertexArray();

			void Bind() const;
			void Unbind() const;

			void AddBufferLayout(const VertexBuffer* vertexBuffer, const VertexBufferLayout* layout);

		private:
			GLuint m_glId;
		};
	}
}
