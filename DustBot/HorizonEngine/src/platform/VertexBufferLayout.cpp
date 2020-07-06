#include "VertexBufferLayout.h"

Horizon::Graphics::VertexBufferLayout::VertexBufferLayout()
	: m_layoutStride(0)
 {}

void Horizon::Graphics::VertexBufferLayout::SetStride(const uint32 stride)
{
	m_layoutStride = stride;
}

unsigned Horizon::Graphics::VertexBufferLayout::GetStride() const
{
	return m_layoutStride;
}

std::vector<Horizon::Graphics::VertexBufferLayoutElement> Horizon::Graphics::VertexBufferLayout::GetLayoutElements() const
{
	return m_layoutElements;
}
