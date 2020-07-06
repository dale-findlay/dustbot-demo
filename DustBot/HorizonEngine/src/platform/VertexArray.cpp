#include "VertexArray.h"

#include "platform\GLHelper.h"

Horizon::Graphics::VertexArray::VertexArray()
{
	GL(glGenVertexArrays(1, &m_glId));
}

Horizon::Graphics::VertexArray::~VertexArray()
{
	GL(glDeleteVertexArrays(1, &m_glId));
}

void Horizon::Graphics::VertexArray::Bind() const
{
	glBindVertexArray(m_glId);

}

void Horizon::Graphics::VertexArray::Unbind() const
{
	glBindVertexArray(0);
}

void Horizon::Graphics::VertexArray::AddBufferLayout(const VertexBuffer* vertexBuffer, const VertexBufferLayout* layout)
{
	vertexBuffer->Bind();
	const std::vector<VertexBufferLayoutElement>& layoutElements = layout->GetLayoutElements();
	for(int i = 0; i < layoutElements.size(); ++i)
	{
		GL(glEnableVertexAttribArray(i));
		GL(glVertexAttribPointer(i, layoutElements[i].count, layoutElements[i].type, layoutElements[i].normalized, layout->GetStride(), reinterpret_cast<void*>(layoutElements[i].offset)));
	}
}
