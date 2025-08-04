#shader vertex
#version 330 core
        
//specify the attribute, vec 4 even though it is vec2, usually is vec 4 
layout(location = 0) in vec4 position; //this is the vertex attribute to work on in shader 

uniform mat4 u_MVP; //u_ means a uniform
uniform mat4 u_Rot; 


void main()
{
    gl_Position = u_MVP * u_Rot * position; //multiply the position by the MVP matrix and the rotation matrix
};


#shader fragment 
#version 330 core
        
//specify the attribute, vec 4 even though it is vec2, usually is vec 4 
layout(location = 0) out vec4 color;

uniform vec4 u_Color; //u_ means a uniform 

void main()
{
    //color = vec4(1.0, 0.0, 0.0, 1.0); replaced by uniform 
    color = u_Color;
};