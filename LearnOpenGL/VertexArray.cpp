#include "VertexArray.h"
#include "glad/glad.h"
#include <vector>

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_rendererID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_rendererID);
}

void VertexArray::AddBuffer(const VertexBuffer& buffer, const VertexBufferLayout& layout)
{
	Bind();
	buffer.Bind();
	std::vector<BufferLayoutElement> elements = layout.GetLayout();
	size_t size = elements.size();
	for (unsigned int i = 0; i < size; i++)
	{
		auto element = elements[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetVertexStride(), (const void*)element.offset);
	}
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_rendererID);
}

void VertexArray::UnBind() const
{
	glBindVertexArray(0);
}
