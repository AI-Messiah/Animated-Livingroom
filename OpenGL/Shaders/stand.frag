#version 430 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D Texture1;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main(){
	vec3 color = texture(Texture1, TexCoord).rgb;
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = 0;
	spec = pow(max(dot(viewDir, reflectDir), 0.0), 8);
	float distance    = length(lightPos - FragPos);
	float attenuation = 1.0 / (1 + .514 * distance + .31 * (distance * distance));
	float inten = .3 + diff * attenuation;
	FragColor = vec4(inten * color, 1);
	BrightColor = vec4(0, 0, 0, 1);
}