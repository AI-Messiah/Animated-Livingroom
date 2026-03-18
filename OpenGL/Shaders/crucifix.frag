#version 430 core

flat in uint matNum;
in vec3 FragPos;
in vec2 TexCoord;
in mat3 TBN;
in vec3 Normal;

uniform sampler2D Texture1;
uniform sampler2D Texture2;
uniform sampler2D Texture3;
uniform sampler2D Texture4;
uniform sampler2D Texture5;
uniform sampler2D Texture6;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform int normState;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

void main(){
	vec3 color = vec3(0, 0, 0);
	vec3 normMap = vec3(0, 0, 0);
	float ambient = 0;
	if (matNum == 0){
		ambient = texture(Texture4, TexCoord).r;
		color = texture(Texture5, TexCoord).rgb;
		normMap = texture(Texture6, TexCoord).rgb;
	}else{
		ambient = texture(Texture1, TexCoord).r;
		color = texture(Texture2, TexCoord).rgb;
		normMap = texture(Texture3, TexCoord).rgb;
	}
	normMap = normMap * 2.0 - 1.0;
	normMap = normalize(TBN * normMap);
	if (normState == 1) normMap = Normal;
	vec3 lightDir = normalize(lightPos - FragPos);
	vec3 viewDir  = normalize(viewPos - FragPos);
	float diff = max(dot(lightDir, normMap), 0.0);
	vec3 halfWay = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normMap, halfWay), 0.0), 8.0);
	float distance = length(lightPos - FragPos);
	float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);
	diff *= attenuation;
	spec *= attenuation;
	float amount = (.3 * ambient + diff + .2 * spec) * 2;
	FragColor = vec4(color * amount, 1);
	BrightColor = vec4(0, 0, 0, 1);
}