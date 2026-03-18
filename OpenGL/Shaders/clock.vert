#version 430 core

layout (location = 0) in vec3 pos;			
layout (location = 1) in vec3 normal;	
layout (location = 2) in vec2 texCoord1;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;
layout (location = 5) in uint texNum;

uniform mat4 mvp;
uniform mat4 model;
uniform mat4 secmvp;
uniform mat4 secmodel;
uniform mat4 minmvp;
uniform mat4 minmodel;
uniform mat4 hourmvp;
uniform mat4 hourmodel;

flat out uint matNum;
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

void main(){
	matNum = texNum;
	TexCoord = texCoord1;
	switch(texNum){
		case 3:
			Normal = mat3(transpose(inverse(secmodel))) * normal; 
			FragPos = vec3(secmodel * vec4(pos, 1.0));
			gl_Position = secmvp * vec4(pos, 1.0f);
			break;
		case 4:
			Normal = mat3(transpose(inverse(minmodel))) * normal; 
			FragPos = vec3(minmodel * vec4(pos, 1.0));
			gl_Position = minmvp * vec4(pos, 1.0f);
			break;
		case 5:
			Normal = mat3(transpose(inverse(hourmodel))) * normal; 
			FragPos = vec3(hourmodel * vec4(pos, 1.0));
			gl_Position = hourmvp * vec4(pos, 1.0f);
			break;
		default:
			Normal = mat3(transpose(inverse(model))) * normal; 
			FragPos = vec3(model * vec4(pos, 1.0));
			gl_Position = mvp * vec4(pos, 1.0f);
			break;
	}
}