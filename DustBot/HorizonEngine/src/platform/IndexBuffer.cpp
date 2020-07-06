#include "IndexBuffer.h"
#include "GLHelper.h"

Horizon::Graphics::IndexBuffer::IndexBuffer(unsigned int * data, const size_t size, const uint32 count)
	: Buffer(GL_ELEMENT_ARRAY_BUFFER), m_indexCount(count)
{
	Bind();
	GL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

Horizon::uint32 Horizon::Graphics::IndexBuffer::GetIndexCount() const
{
	return m_indexCount;
}
