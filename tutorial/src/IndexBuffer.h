#pragma once
#include "Renderer.h"


class IndexBuffer {
private:

	unsigned int m_RendererID;

public:
	IndexBuffer(const void* data, unsigned int count) {
		GLCall(glGenBuffers(1, &m_RendererID));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);)
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);)
	}
	~IndexBuffer() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void Bind() {
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);)
	}
	void Unbind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
};