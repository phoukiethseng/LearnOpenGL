#pragma once
#include <vector>

struct BufferLayoutElement {
	size_t count;
	int type;
	bool normalized;
	int offset;
};

class VertexBufferLayout {
/**
	Represent attribute layout descriptor for vertex buffer.
**/
private:
	std::vector<BufferLayoutElement> m_layouts;
	unsigned int m_vertexStride = 0;
	unsigned int m_offset = 0;
public:
	template<typename T>
	void Push(size_t count, bool normalized);
	std::vector<BufferLayoutElement> GetLayout() const;
	unsigned int GetVertexStride() const;
};

