//glew has to be included first, in docs 
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream> //what is this again 

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

//handling shaders in a seperate file to use for this 
static ShaderProgramSource ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);


    //for indexing the shaders into stringstream 
    enum class ShaderType 
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    ShaderType type = ShaderType::NONE;
    std::stringstream ss[2]; //we use this to continously put values into string 

    //is there a way we can write this to adapt to the number of shaders defined in the file, that way we don't have hard coded two in there 

    //getline consumes lines by newline, line is holding each new line we consume from getline 
    std::string line;
    while (getline(stream, line))
    {

        //once we see a shader, we set the stringstream index to start adding to 
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if(line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }
    return { ss[0].str(), ss[1].str() };

}





//helper to be called, static just means that only can be used in file where it is defined 
static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    //review this block later, to understand the internals 
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str(); //convert to c string for use, same as &source[0] pointer to first part of bytes 
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    //need error handling to understand what is happening
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {

        int length;
        //read docs on this 
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        //char arrays have to have constant length, alloca is allocatontion on the stack, clever work around to random thing about char arrays 
        //char message [length] not allowed, since length is variable 
        char* message = (char*)alloca(length * sizeof(char));

        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "failed to compile shader of type" << type << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;

    }




    return id;
}

//source code is within the strings that are passed into this
static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    //provide openGL with our shader code to compile, then bind that shader back 
    //the API is inconsistent, dont worry about it too much 
    unsigned int program = glCreateProgram();
    //vertex shader, point transforms
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    //fragment shader, rasterize changes, coloring book 
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    //delete the intermediate object files
    //they are linked into a program now 
    glDeleteShader(vs);
    glDeleteShader(fs);

    //glDetachShader, usually not a good idea, deletes the source code, good to keep for debugging 


    return program;
}





int test3(void)
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





    //also a buffer holding our data pertaing to the triangle 
    //attributes are used to specify what the buffer structure is 
    float positions[]{
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,

         0.5f,  0.5f,
        -0.5f,  0.5f,
        -0.5f, -0.5f,
    };


    //id of the buffer put into the pointer of this value 
    unsigned int buffer;
    //num buffers, pointer to unsigned int -> create a buffer, assign id to the unsigned int
    glGenBuffers(1, &buffer);
    //now we bind to a target to describe how that buffer behaves 
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    //data into that created and targeted buffer 
    glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions, GL_STATIC_DRAW);


    /*

        // (x, y, r, g, b) for each vertex
        float vertexData[] = {
            -1.0f, -1.0f, 1.0f, 0.0f, 0.0f,  // Vertex 1
                0.0f,  1.0f, 0.0f, 1.0f, 0.0f,  // Vertex 2
                1.0f, -1.0, 0.0f, 0.0f, 1.0f   // Vertex 3
        };

        //we offset to get to the color attributes, change number of components, but stride is the same

        // Position attribute (starts at byte 0)
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(0));
        // Color attribute (starts at byte 2 * sizeof(float))
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(2 * sizeof(float)));

    */

    //index of vertex attribute to enable, review this more later, how exactly vertex attribute stuff is working for rendering 
    glEnableVertexAttribArray(0);
    //index, number of components per vertex, type, normalize, stride, points to first component of first generic vertex = reviwe later wtf 
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    
    ShaderProgramSource source = ParseShader("res/shaders/basic.shader");

    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    glUseProgram(shader);






    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        //we need vertex attributes to actually draw call, ignore for now
        // 
        //mode, offset to first starting index, count of indices to be rendered
        //draws what is currently bound 
        glDrawArrays(GL_TRIANGLES, 0, 6);


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }


    //this cleans up the shaders, should for vertex buffers as well 
    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}