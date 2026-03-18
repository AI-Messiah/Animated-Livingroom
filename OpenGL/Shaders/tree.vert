#version 430 core

layout (location = 0) in vec3 pos;			
layout (location = 1) in vec3 normal;	
layout (location = 2) in vec2 texCoord1;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;
layout (location = 5) in uint texNum;

uniform mat4 mvp;

flat out uint mNum;
out vec3 Pos;
out vec3 Norm;

void main(){
	mNum = texNum;
	Pos = pos;
	Norm = normal;
	gl_Position = mvp * vec4(pos, 1.0f);
}