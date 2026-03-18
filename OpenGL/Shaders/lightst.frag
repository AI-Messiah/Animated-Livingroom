#version 430 core

uniform int lightState;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

void main()
{
    if (lightState == 1){
        vec3 lightColor = vec3(1, 1, 1);
        vec3 color = lightColor * 7; // MUST be > 1.0 for bloom
        FragColor = vec4(color, 1.0);

        float brightness = dot(color, vec3(0.2126, 0.7152, 0.0722));
        if (brightness > 1.0)
            BrightColor = vec4(color, 1.0);
        else
            BrightColor = vec4(0.0);
    }else{
        vec3 albedo = vec3(0.85);   // white plastic bulb
        float metallic = 0.0;
        float roughness = 0.25;
        vec3 ambient = vec3(0.08) * albedo;
    }
}