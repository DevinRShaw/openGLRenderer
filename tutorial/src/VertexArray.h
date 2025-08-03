#pragma once 
#include "VertexBuffer.h"
#include <iostream>
#include <unordered_map>
#include "Renderer.h"
#include <vector>
#include<string>

struct VertexBufferLayout {
    std::string name;       // Name for intuitive look up in the vertex array 
    unsigned int index;     // Attribute location
    int size;               // Number of components (1-4)
    unsigned int type;      // GL_FLOAT, GL_INT, etc.
    bool normalized;        // Should the data be normalized?
    unsigned int stride;    // Byte stride between attributes
    size_t offset;          // Offset in bytes to first component
};


//creates and appends a VertexBufferLayout to a passed in vector of layouts, layouts then passed into the VertexArray class 
//create a vector, add the attribute arrays same as the call before, and then pass in the vector to vertex array to bind vertex buffer 
void AddVertexBufferLayout(std::vector<VertexBufferLayout>& layouts, std::string name, unsigned int index,
    int size,
    unsigned int type,
    bool normalized,
    unsigned int stride,
    size_t offset)
{

    VertexBufferLayout layout;
    layout.name = name;
    layout.index = index;
    layout.size = size;
    layout.type = type;
    layout.normalized = normalized;
    layout.stride = stride;
    layout.offset = offset;

    layouts.push_back(layout);
}

class VertexArray {

	private:
        //for any future error handling we will need
		std::unordered_map<unsigned int, VertexBufferLayout> layouts;
        unsigned int m_RendererID;

	public:
        VertexArray() {

            glGenVertexArrays(1, &m_RendererID);
        }
        ~VertexArray() {
            GLCall(glDeleteVertexArrays(1, &m_RendererID));
        }

		void AddBuffer( const VertexBuffer& vb, const std::vector<VertexBufferLayout> input_layouts ) {

            Bind();

            vb.Bind();
            
            for (const auto& layout : input_layouts) {
                ASSERT((layouts.find(layout.index) == layouts.end()));
                layouts[layout.index] = layout;
                glVertexAttribPointer(
                    layout.index,
                    layout.size,
                    layout.type,
                    layout.normalized ? GL_TRUE : GL_FALSE,
                    layout.stride,
                    (const void*)layout.offset
                );
                glEnableVertexAttribArray(layout.index);
            }
            

		}

        void Bind() {
            GLCall(glBindVertexArray(m_RendererID));
        }
};