#pragma once

#include "core\ExportMacro.h"
#include "core\TypeAbstractions.h"
#include "utilities\Logger.h"
#include "platform\structures\VertexBufferLayoutElement.h"

#include <GL/glew.h>
#include <vector>
#include <string>

namespace Horizon
{
	namespace Graphics
	{
		class VertexBufferLayout
		{
		public:
			VertexBufferLayout();

			template<typename T>
			void AddElement(const uint32 count, const size_t offset)
			{}

			template<>
			void AddElement<float>(const uint32 count, const size_t offset)
			{
				m_layoutElements.push_back({ count, GL_FLOAT, GL_FALSE, offset });
				m_layoutStride += count * VertexBufferLayoutElement::GetGLTypeSize(GL_FLOAT);
			}

			template<>
			void AddElement<uint32>(const uint32 count, const size_t offset)
			{
				m_layoutElements.push_back({ count, GL_UNSIGNED_INT, GL_FALSE, offset });
				m_layoutStride += count * VertexBufferLayoutElement::GetGLTypeSize(GL_UNSIGNED_INT);
			}

			template<>
			void AddElement<unsigned char>(const uint32 count, const size_t offset)
			{
				m_layoutElements.push_back({ count, GL_UNSIGNED_BYTE, GL_TRUE, offset });
				m_layoutStride += count * VertexBufferLayoutElement::GetGLTypeSize(GL_UNSIGNED_BYTE);
			}


			//Use only when the number of vertex attributes don't match vertex structure.
			void SetStride(uint32 stride);
			uint32 GetStride() const;
			std::vector<VertexBufferLayoutElement> GetLayoutElements() const;

		private:
			std::vector<VertexBufferLayoutElement> m_layoutElements;
			uint32 m_layoutStride;
		};
	}
}