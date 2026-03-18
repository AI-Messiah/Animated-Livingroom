#version 430 core

in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewPos;
uniform int lightState;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

const float PI = 3.14159265359;

vec3 diffuseBRDF(vec3 albedo)
{
    return albedo / PI;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

void main(){
    vec3 albedo = vec3(0.9);
    float roughness = 0.2;

    vec3 N = normalize(Normal);
    vec3 V = normalize(viewPos - FragPos);

    // Ambient so it isn't black
    vec3 ambient = vec3(0.08) * albedo;

    // Fake translucency using view-based rim
    float rim = pow(1.0 - max(dot(N, V), 0.0), 3.0);
    vec3 translucency = vec3(0.15) * rim;

    vec3 emission = vec3(0.0);

    if (lightState == 1)
    {
        emission = vec3(15.0);   // HDR brightness
        emission += vec3(2.0) * rim; // edge glow
    }

    vec3 color = ambient + translucency + emission;

    // Tone map
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0 / 2.2));

    FragColor = vec4(color, 1.0);

    float brightness = dot(color, vec3(0.2126, 0.7152, 0.0722));
    BrightColor = brightness > .7 ? vec4(color, 1.0) : vec4(0.0);
}