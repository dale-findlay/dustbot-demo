#include "Buffer.h"

#include "GLHelper.h"

Horizon::Graphics::Buffer::Buffer(const GLenum target)
	:m_target(target)
{
	GL(glGenBuffers(1, &m_glId));
}

Horizon::Graphics::Buffer::~Buffer()
{
	GL(glDeleteBuffers(1, &m_glId));
}

void Horizon::Graphics::Buffer::Bind() const
{
	GL(glBindBuffer(m_target, m_glId));
}

void Horizon::Graphics::Buffer::Unbind() const
{
	GL(glBindBuffer(m_target, 0));
}
