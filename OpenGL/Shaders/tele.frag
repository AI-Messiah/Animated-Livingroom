#version 430 core

flat in uint matNum;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D Texture1;
uniform vec3 lightPos;
uniform vec3 viewPos;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

void main(){
	vec3 color = vec3(0, 0, 0);
	switch(matNum){
		case 0:
			color = vec3(1, 1, 1);
			break;
		case 1:
			color = vec3(0, 0, 0);			
			break;
		case 2:
			color = texture(Texture1, TexCoord).rrr;
			break;
		case 3:
			color = vec3(.002, .046, .001);
			break;
		case 4:			
			color = vec3(.199, .199, .199);
			break;
	}
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = 0;
	if (matNum == 1 || matNum > 2) spec = pow(max(dot(viewDir, reflectDir), 0.0), 8);
	float distance    = length(lightPos - FragPos);
	float attenuation = 1.0 / (1 + .514 * distance + .31 * (distance * distance));
	float inten = .3 + diff * attenuation + spec * .5;
	FragColor = vec4(inten * color, 1);
	BrightColor = vec4(0, 0, 0, 1);
}