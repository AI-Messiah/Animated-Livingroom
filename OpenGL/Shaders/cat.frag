#version 430 core

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

void main()
{		
    vec3 albedo = texture(Texture1, TexCoord).rgb;
    float metallic = texture(Texture2, TexCoord).r;
    float roughness = texture(Texture3, TexCoord).r;
    vec3 N = normalize(Normal);
    vec3 V = normalize(viewPos - FragPos);
    vec3 L = normalize(lightPos - FragPos);
    vec3 H = normalize(V + L);
    
    float NdotL = max(dot(N, L), 0.0);
    float NdotV = max(dot(N, V), 0.0);
    float NdotH = max(dot(N, H), 0.0);
    float VdotH = max(dot(V, H), 0.0);

    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    vec3 F = fresnelSchlick(VdotH, F0);

    float D = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);  
        
    vec3 numerator    = D * G * F;
    float denominator = max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
    vec3 specular     = numerator / denominator;  
            
    vec3 kS = F;
    vec3 kD = (vec3(1.0) - kS) * (1.0 - metallic);

    vec3 diffuse = kD * albedo / PI;
	
    vec3 radiance = vec3(1, 1, 1) * NdotL;

    vec3 color = (diffuse + specular) * radiance; 
   
    FragColor = vec4(color, 1.0);
} 