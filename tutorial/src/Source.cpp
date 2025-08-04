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
#include "macros.h"
const double PI = 3.14159265358979323846;






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

    //This is example of using glm projection, this adjusts the ratio to create a square in non square window 
	glm::mat4 projection = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f); 

    glm::mat4 identityMatrix = glm::mat4(1.0f);
	glm::mat4 rotation = glm::rotate(identityMatrix, 1.0f, glm::vec3(1, 0 ,0)); // translate the cube to the center of the screen
    rotation = glm::rotate(rotation, 1.0f, glm::vec3(0, 1, 0)); // translate the cube to the center of the screen



    

    //glBindVertexArray(0); // Optional, unbind

    Shader shader("res/shaders/basic.shader");
    shader.Bind();
    shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
  
    cubeBuffer.Unbind();
    cubeIndex.Unbind();
    shader.Unbind();




    float r = 0.0f;
    float increment = 0.05f;
    float spin1 = 0.0f;
	float spin2 = 0.0f;

    Renderer renderer;

    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

		//Can use a macro here to bind everytime we set uniform? Could be useful for quick development 
        shader.Bind();
        shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

        
		shader.SetUniformMat4f("u_MVP", projection);
        shader.SetUniformMat4f("u_Rot", rotation);

		renderer.Draw(vertexArray, cubeIndex, shader);


        if (r > 0.8) increment = -0.0001f;
        else if (r < 0.0f) increment = 0.0001f;

        spin1 += 0.0001f;
        spin2 += 0.0002f;


        rotation = glm::rotate(identityMatrix, spin1 , glm::vec3(1, 0, 0)); // translate the cube to the center of the screen
        rotation = glm::rotate(rotation, spin2 , glm::vec3(0, 1, 0)); // translate the cube to the center of the screen

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