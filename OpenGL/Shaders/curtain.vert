#version 430 core

layout (location = 0) in vec3 pos;			
layout (location = 1) in vec3 normal;	
layout (location = 2) in vec2 texCoord1;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;
layout (location = 5) in uint texNum;

uniform mat4 mvp;
uniform mat4 model;
uniform float time;
uniform int side;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos; 

void main(){
	TexCoord = texCoord1;

	float relx = (pos.x + 0.67618) / 1.352356;
	float rely = (pos.y + 0.140543) / 0.261015;
	float relz = (pos.z + 1.1939) / 2.38778;

	vec3 relpos = vec3(relx, rely, relz);

	float windStrength = .05;
	vec3 swingDir = vec3(1, 0, 0);

	float swing = sin(time + relpos.y * 3.14159) * windStrength;

	if (side == 0) swing *= -1;

    float influence = (1 - relpos.z) * (1 - relpos.z);

    vec3 offset = swingDir * swing * influence;

	vec3 animatedPos = pos + offset;
	Normal = mat3(transpose(inverse(model))) * normal;
	if (side == 0) Normal.x *= -1;
	FragPos = vec3(model * vec4(animatedPos, 1.0f));
	gl_Position = mvp * vec4(animatedPos, 1.0f);
}