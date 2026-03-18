#version 430 core

layout (location = 0) in vec3 pos;			
layout (location = 1) in vec3 normal;	
layout (location = 2) in vec2 texCoord1;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;
layout (location = 5) in uint texNum;

uniform mat4 model;
uniform mat4 rotmod;

void main()
{
    if (texNum == 0 || texNum == 1){
        gl_Position = rotmod * vec4(pos, 1.0);
    }else{
        gl_Position = model * vec4(pos, 1.0);
    }
}