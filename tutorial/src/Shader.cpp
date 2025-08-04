#include "Shader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream> 

#include "macros.h"




Shader::Shader(const std::string& filepath)
	: m_FilePath(filepath), m_RendererID(0)
{
    ShaderProgramSource source = ParseShader(filepath);
    m_RendererID =  CreateShader(source.VertexSource, source.FragmentSource);
   
}   

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}


//handling shaders in a seperate file to use for this 
ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);
    ASSERT(stream.is_open());


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
            else if (line.find("fragment") != std::string::npos)
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
 unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
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

int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
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


void Shader::Bind() const
{
    
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
   
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix) {
    //gldocs for knowing wassup with this one 
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

unsigned int Shader::GetUniformLocation(const std::string& name) 
{
    GLCall( unsigned int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1) std::cout << "Uniform " << name << " doesn't exist" << std::endl;
    return location;

	
}
