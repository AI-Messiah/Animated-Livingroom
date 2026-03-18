#version 430 core

layout (location = 0) in vec2 pos;			
layout (location = 1) in vec2 tex;

out vec2 TexCoord;

void main(){
	TexCoord = tex; 
	gl_Position = vec4(pos, -.999, 1.0f);
}