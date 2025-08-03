#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define SHOW(x) std::cout << #x << " is "<<  x << std::endl


//review macros, this is add a breakpoint at compile time this is pretty cool 
#define ASSERT(x) if ((!x)) __debugbreak(); //this is compiler specific since we are using a macro (msvc)

//this will help us call some cleaner error checking 
//this is inserting the function call between the two 
#define GLCall(x) GLClearError();\
    x;\
    ASSERT((GLLogCall(#x,__FILE__,__LINE__))); //file and line are intrinsics, #converts part of maro to a string holy crap 

//loop to clear all the errors that may be present, this is like checking in on openGL and there is an easier way to do this 
static void GLClearError() {
    while (glGetError() != GL_NO_ERROR); //loops and calls repeatedly
}

static bool GLLogCall(const char* function, const char* file, int line) {

    while (GLenum error = glGetError()) { //consume errors and print 
        std::cout << "Error Code: " << error << " @ Function: " << function << " @ file " << file << " @ line " << line << std::endl;

        return false; //badly written, just return on first loop this is just to show you 
    }
    return true;
}
