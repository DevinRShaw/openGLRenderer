#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"


class Renderer {
public: 
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {
		shader.Bind();
		va.Bind();
		ib.Bind();
		glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr); 
    }


};