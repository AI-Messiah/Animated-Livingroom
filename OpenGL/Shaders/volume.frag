#version 430 core

in vec2 TexCoord;

uniform sampler2D Texture1;

out vec4 FragColor;

void main(){
	FragColor = vec4(texture(Texture1, TexCoord).rgb, 1);
}