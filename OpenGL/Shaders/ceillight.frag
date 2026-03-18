#version 430 core

flat in uint matNum;
in vec2 TexCoord;
in mat3 TBN;
in vec3 WorldPos;
in vec3 Normal;

uniform sampler2D Texture1;
uniform sampler2D Texture2;
uniform sampler2D Texture3;
uniform sampler2D Texture4;
uniform sampler2D Texture5;
uniform sampler2D Texture6;
uniform sampler2D Texture7;
uniform sampler2D Texture8;
uniform sampler2D Texture9;
uniform sampler2D Texture10;
uniform sampler2D Texture11;
uniform sampler2D Texture12;

uniform vec3 cameraPos;
uniform vec3 lightPos;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

const float PI = 3.14159265359;

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float distributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);

    float denom = (NdotH * NdotH * (a2 - 1.0) + 1.0);
    return a2 / (PI * denom * denom);
}

float geometrySchlickGGX(float NdotV, float roughness)
{
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;
    return NdotV / (NdotV * (1.0 - k) + k);
}

float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float ggx1 = geometrySchlickGGX(max(dot(N, V), 0.0), roughness);
    float ggx2 = geometrySchlickGGX(max(dot(N, L), 0.0), roughness);
    return ggx1 * ggx2;
}

void main()
{
    float emissionStrength = 3.3;
    // --- Normal mapping ---
    vec3 n = vec3(0, 0, 0);
    switch(matNum){
        case 0:
            n = texture(Texture4, TexCoord).rgb * 2.0 - 1.0;
            break;
        case 1:
            n = texture(Texture8, TexCoord).rgb * 2.0 - 1.0;
            break;
        case 2:
            n = texture(Texture12, TexCoord).rgb * 2.0 - 1.0;
            break;
    }
    
    vec3 N = normalize(TBN * n);

    // --- View & light vectors ---
    vec3 V = normalize(cameraPos - WorldPos);
    vec3 L = normalize(lightPos - WorldPos);
    vec3 H = normalize(V + L);

    vec3 lightColor = vec3(0, 0, 0);
    switch(matNum){
        case 0:
            lightColor = texture(Texture1, TexCoord).rgb;
            break;
        case 1:
            lightColor = texture(Texture5, TexCoord).rgb;
            break;
        case 2:
            lightColor = texture(Texture9, TexCoord).rgb;
            break;
    }

    float distance = length(lightPos - WorldPos);
    float attenuation = 1.0 / (distance * distance + 1.0);
    vec3 radiance = lightColor * attenuation;

    // --- Metallic & roughness ---
    vec2 mr = vec2(0, 0);
    switch(matNum){
        case 0:
            mr = texture(Texture3, TexCoord).rg;
            break;
        case 1:
            mr = texture(Texture7, TexCoord).rg;
            break;
        case 2:
            mr = texture(Texture11, TexCoord).rg;
            break;
    }

    float metallic  = mr.r;
    float roughness = clamp(mr.g, 0.04, 1.0);

    // --- Base reflectance ---
    vec3 albedo = vec3(0.02); // dielectric base
    vec3 F0 = mix(vec3(0.04), albedo, metallic);

    // --- Cook–Torrance BRDF ---
    float NDF = distributionGGX(N, H, roughness);
    float G   = geometrySmith(N, V, L, roughness);
    vec3  F   = fresnelSchlick(max(dot(H, V), 0.0), F0);

    vec3 numerator = NDF * G * F;
    float denom = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
    vec3 specular = numerator / denom;

    vec3 kS = F;
    vec3 kD = (1.0 - kS) * (1.0 - metallic);

    float NdotL = max(dot(N, L), 0.0);
    vec3 reflected = (kD * albedo / PI + specular) * radiance * NdotL;

    // --- Emission ---
    vec3 emissive = vec3(0, 0, 0);
    switch(matNum){
        case 0:
            emissive = texture(Texture2, TexCoord).rgb;
            break;
        case 1:
            emissive = texture(Texture6, TexCoord).rgb;
            break;
        case 2:
            emissive = texture(Texture10, TexCoord).rgb;
            break;
    }
    float emitNdL = max(dot(N, L), 0.0);
    vec3 emission = emissive * emissionStrength * emitNdL;

    // --- Final ---
    FragColor = vec4(reflected + emission, 1.0);

    if (emission.r > 1.0){
        BrightColor = vec4(emission, 1.0);
    }else{
        BrightColor = vec4(0.0);
    }
}