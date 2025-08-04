#pragma once
#include "macros.h"


class IndexBuffer {
private:

	unsigned int m_RendererID;
	int count;

public:
	IndexBuffer(const void* data, unsigned int count) {
		GLCall(glGenBuffers(1, &m_RendererID));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);)
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);)
		this->count = count;
	}
	~IndexBuffer() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void Bind() const {
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);)
	}
	void Unbind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	int GetCount() const {
		return count;
	}
};