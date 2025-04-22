#pragma once

class IndexBuffer {
private:
	unsigned int m_rendererID;
	unsigned int m_count;
public:
	IndexBuffer(const void* buffer, size_t count);
	~IndexBuffer();
	
	void Bind() const;
	void Unbind() const;
};
