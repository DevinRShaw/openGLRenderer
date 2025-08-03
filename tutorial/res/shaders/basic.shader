#shader vertex
#version 330 core
        
//specify the attribute, vec 4 even though it is vec2, usually is vec 4 
layout(location = 0) in vec4 position;
void main()
{
    gl_Position = position;
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