#version 430 core

layout (location = 0) in vec3 pos;			
layout (location = 1) in vec3 normal;	
layout (location = 2) in vec2 texCoord1;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;
layout (location = 5) in uint texNum;

uniform mat4 vp;
uniform mat4 model;


flat out uint matNum;
out vec2 TexCoord;
out mat3 TBN;
out vec3 WorldPos;
out vec3 Normal;

void main(){
	matNum = texNum;
	TexCoord = texCoord1;
	vec3 T = normalize(mat3(model) * tangent);
    vec3 B = normalize(mat3(model) * bitangent);
    vec3 N = normalize(mat3(model) * normal);

    TBN = mat3(T, B, N);
    WorldPos = vec3(model * vec4(pos, 1.0));
	Normal = normal;
	gl_Position = vp * vec4(WorldPos, 1.0);
}