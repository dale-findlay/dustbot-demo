#include "VertexBuffer.h"

#include "core\TypeAbstractions.h"
#include "platform\GLHelper.h"

Horizon::Graphics::VertexBuffer::VertexBuffer(const void* data, size_t size, uint32 count)
	:Buffer(GL_ARRAY_BUFFER), m_vertexCount(count)
{
	Bind();
	GL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

Horizon::uint32 Horizon::Graphics::VertexBuffer::GetVertexCount() const
{
	return m_vertexCount;
}
