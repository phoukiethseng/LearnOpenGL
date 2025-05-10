#include "VertexBufferLayout.h"
#include "glad/glad.h"

template<>
void VertexBufferLayout::Push<float>(size_t count, bool normalized)
{
	BufferLayoutElement element =  { count, GL_INT, normalized, m_offset };
	m_layouts.push_back(element);
	m_offset += sizeof(int) * count;
	m_vertexStride += sizeof(int) * count;
}

template<>
void VertexBufferLayout::Push<int>(size_t count, bool normalized)
{
	BufferLayoutElement element =  { count, GL_FLOAT, normalized, m_offset };
	m_layouts.push_back(element);
	m_offset += sizeof(float) * count;
	m_vertexStride += sizeof(float) * count;
}

std::vector<BufferLayoutElement> VertexBufferLayout::GetLayout() const
{
	return m_layouts;
}

unsigned int VertexBufferLayout::GetVertexStride() const
{
	return m_vertexStride;
}
