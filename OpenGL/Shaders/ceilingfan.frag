#version 430 core

flat in uint matNum;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D Texture1;
uniform sampler2D Texture2;
uniform sampler2D Texture3;
uniform vec3 lightPos;
uniform vec3 viewPos;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

void main(){
	vec3 color = vec3(0, 0, 0);
	switch(matNum){
		case 0:
		case 7:
			color = texture(Texture1, TexCoord).rgb;
			break;
		case 1:
		case 5:
			color = texture(Texture2, TexCoord).rgb;
			break;
		case 2:
		case 3:
		case 4:
		case 6:
		case 8:
			color = texture(Texture3, TexCoord).rgb;
			break;

	}
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = 0;
	spec = pow(max(dot(viewDir, reflectDir), 0.0), 8);
	float inten = .3 + diff;
	FragColor = vec4(inten * color, 1);
	BrightColor = vec4(0, 0, 0, 1);
}