//glew has to be included first, in docs 
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream> //what is this again 
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"





int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    //docs say you have to have a valid window before you call glew init
    if (glewInit() != GLEW_OK)
        std::cout << "Error glew init" << std::endl;


    std::cout << glGetString(GL_VERSION) << std::endl;



    float positions[] = {
        // x     y     z
        -0.5f, -0.5f, -0.5f, // 0
         0.5f, -0.5f, -0.5f, // 1
         0.5f,  0.5f, -0.5f, // 2
        -0.5f,  0.5f, -0.5f, // 3
        -0.5f, -0.5f,  0.5f, // 4
         0.5f, -0.5f,  0.5f, // 5
         0.5f,  0.5f,  0.5f, // 6
        -0.5f,  0.5f,  0.5f  // 7
    };

    unsigned int indices[] = {
        // front face
        4, 5, 6,
        6, 7, 4,

        // back face
        0, 1, 2,
        2, 3, 0,

        // left face
        0, 4, 7,
        7, 3, 0,

        // right face
        1, 5, 6,
        6, 2, 1,

        // top face
        3, 2, 6,
        6, 7, 3,

        // bottom face
        0, 1, 5,
        5, 4, 0
    };

   

    VertexArray vertexArray = VertexArray();


  
    VertexBuffer cubeBuffer = VertexBuffer(positions, 24 * sizeof(float));
    
    std::vector<VertexBufferLayout> layouts; 
    AddVertexBufferLayout(layouts, "name", 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);


    vertexArray.AddBuffer(cubeBuffer, layouts);
   
    IndexBuffer cubeIndex = IndexBuffer(indices, 36);

    

    //glBindVertexArray(0); // Optional, unbind

    Shader shader("res/shaders/basic.shader");
    shader.Bind();
    shader.SetUniform("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
  
    cubeBuffer.Unbind();
    cubeIndex.Unbind();
    shader.Unbind();




    float r = 0.0f;
    float increment = 0.05f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        shader.Bind();

        //lets make it a function of rendering time, this is the color, we want modulation
        shader.SetUniform("u_Color", r, 0.3f, 0.8f, 1.0f);

        vertexArray.Bind();
        cubeIndex.Bind();

        //dont need because we only have one object 
        //GLCall(glBindVertexArray(vao));
        //GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));

        //new error handling with some cool macro magic 
        GLCall(glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr));


        if (r > 0.8) increment = -0.01f;
        else if (r < 0.0f) increment = 0.01f;

        r += increment;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);



        /* Poll for and process events */
        glfwPollEvents();
    }


    //this cleans up the shaders, should for vertex buffers as well 
    
  

    glfwTerminate();
    return 0;
}