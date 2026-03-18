#version 430 core

uniform int lightState;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

void main(){
	if (lightState == 1)
    {
		FragColor = vec4(0, .4, 0, 1);
	}else{
		FragColor = vec4(0, .2, 0, 1);
	}
	BrightColor = vec4(0, 0, 0, 1);
}