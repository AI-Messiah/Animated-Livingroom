#version 430 core

layout(std430, binding = 0) buffer Centers {
    vec3 centers[];
};

uniform vec3 viewPos;
uniform vec3 starCent;
uniform int lightState;

flat in uint mNum;
in vec3 Pos;
in vec3 Norm;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

const float scale = .817; // this is the scale in relation of the original model applied to the 'model' matrix

const float PI = 3.14159265359;

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a  = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num / max(denom, 0.0001);
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;

    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);
    return ggx1 * ggx2;
}

vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 CalcPBRLight(
    vec3 lightPos,
    vec3 lightColor,
    vec3 N,
    vec3 V,
    vec3 fragPos,
    vec3 albedo,
    float metallic,
    float roughness
)
{
    vec3 L = normalize(lightPos - fragPos);
    vec3 H = normalize(V + L);

    float distance = length(lightPos - fragPos);
    float attenuation = 1.0 / (distance * distance + 1.0);
    vec3 radiance = lightColor * attenuation;

    float NdotL = max(dot(N, L), 0.0);
    float NdotV = max(dot(N, V), 0.0);

    if (NdotL <= 0.0)
        return vec3(0.0);

    // --- Fresnel base reflectivity ---
    vec3 F0 = mix(vec3(0.04), albedo, metallic);

    // --- Cook-Torrance BRDF ---
    float D = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);
    vec3  F = FresnelSchlick(max(dot(H, V), 0.0), F0);

    vec3 numerator = D * G * F;
    float denominator = 4.0 * NdotV * NdotL + 0.0001;
    vec3 specular = numerator / denominator;

    vec3 kS = F;
    vec3 kD = (vec3(1.0) - kS) * (1.0 - metallic);

    vec3 diffuse = kD * albedo / PI;

    return (diffuse + specular) * radiance * NdotL;
}

vec3 CalcTreeLight(vec3 lightPos, vec3 normal, vec3 fragPos)
{
    vec3 lightColor = vec3(1.0);

    vec3 L = normalize(lightPos - fragPos);
    float NdotL = max(dot(normal, L), 0.0);

    float distance = length(lightPos - fragPos) / scale;
    float attenuation = 1.0 / (distance * distance + 1.0);

    return lightColor * NdotL * attenuation;
}

vec3 CalcStarLight(vec3 lightPos, vec3 normal, vec3 fragPos)
{
    vec3 lightColor = vec3(1.0, 1.0, 0.6);

    vec3 L = normalize(lightPos - fragPos);
    float NdotL = max(dot(normal, L), 0.0);

    float distance = length(lightPos - fragPos) / scale;
    float attenuation = 1.0 / (distance * distance + 0.5);

    return lightColor * NdotL * attenuation * 3.0;
}

void main()
{
    // --- World space normal ---
    vec3 N = normalize(Norm);

    // --- View direction (world space) ---
    vec3 V = normalize(viewPos - Pos);

    // --- Material ---
    vec3 albedo;
    float metallic = 0.0;
    float roughness = 0.6;

    if (mNum == 0)
        albedo = vec3(0.18, 0.12, 0.06); // trunk
    else
        albedo = vec3(0.09, 0.22, 0.13); // needles

    vec3 result = vec3(0.0);

    // --- Tree lights ---
    if (lightState == 1)
    {
        for (int i = 0; i < 1008; i++)
        {
            result += CalcPBRLight(
                centers[i],
                vec3(1.0),        // white lights
                N,
                V,
                Pos,
                albedo,
                metallic,
                roughness
            ) / 87.0;
        }
    }

    // --- Star light ---
    result += CalcPBRLight(
        starCent,
        vec3(1.0, 1.0, 0.6) * 3.0,
        N,
        V,
        Pos,
        albedo,
        metallic,
        roughness
    );

    // --- Small ambient term ---
    vec3 ambient = vec3(0.03) * albedo;

    vec3 color = result + ambient;

    // --- HDR tone mapping ---
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0 / 2.2));

    FragColor = vec4(color, 1.0);
    float brightness = dot(color, vec3(0.2126, 0.7152, 0.0722));
    if (brightness > 1.0)
        BrightColor = vec4(color, 1.0);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}