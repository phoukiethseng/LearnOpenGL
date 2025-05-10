#pragma once

class VertexBuffer {
/**
	Represent a buffer that store vertices and its attributes.
**/
private:
	unsigned int m_rendererID;
public:
	VertexBuffer(const float* buffer, size_t count);
	~VertexBuffer();
	
	void Bind() const;
	void Unbind() const;
	
};
