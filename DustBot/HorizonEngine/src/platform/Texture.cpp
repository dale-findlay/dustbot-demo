#include "Texture.h"

#include "platform\GLHelper.h"

Horizon::Graphics::Texture::Texture(const GLenum target)
	: m_target(target), m_glId(0)
{
	GL(glGenTextures(1, &m_glId));
}

Horizon::Graphics::Texture::~Texture()
{
	GL(glDeleteTextures(1, &m_glId));
}

void Horizon::Graphics::Texture::Bind() const
{
	GL(glBindTexture(m_target, m_glId));
}

void Horizon::Graphics::Texture::Unbind() const
{
	GL(glBindTexture(GetTarget(), 0));
}

void Horizon::Graphics::Texture::SetTextureParameter(const GLenum name, const GLint value)
{
	Bind();
	glTexParameteri(GetTarget(), name, value);
}

GLuint Horizon::Graphics::Texture::GetId() const
{
	return m_glId;
}

GLenum Horizon::Graphics::Texture::GetTarget() const
{
	return m_target;
}