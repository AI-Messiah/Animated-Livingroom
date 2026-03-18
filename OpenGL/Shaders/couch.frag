#version 430 core

flat in uint matNum;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in mat3 TBN;

uniform sampler2D Texture1;
uniform sampler2D Texture2;
uniform sampler2D Texture3;
uniform sampler2D Texture4;
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

void main(){
	
	if (matNum == 0){
		vec3 albedo = texture(Texture1, TexCoord).rgb;
        float metallic = 0;
        float roughness = texture(Texture4, TexCoord).r;
		vec3 tangentNormal = texture(Texture3, TexCoord).rgb;
		tangentNormal = tangentNormal * 2.0 - 1.0;
		vec3 N = normalize(TBN * tangentNormal);
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
	
        vec3 radiance = vec3(2, 2, 2) * NdotL;

        vec3 color = albedo * .2 + (diffuse + specular) * radiance; 
   
        FragColor = vec4(color, 1.0);
	}else{
		vec3 color = texture(Texture2, TexCoord).rgb;
		vec3 norm = normalize(Normal);
		vec3 lightDir = normalize(lightPos - FragPos);
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 viewDir = normalize(viewPos - FragPos);
		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = 0;
		spec = pow(max(dot(viewDir, reflectDir), 0.0), 8);
		float distance    = length(lightPos - FragPos);
		float attenuation = 1.0 / (1 + .514 * distance + .31 * (distance * distance));
		float inten = .3 + diff * attenuation;
		FragColor = vec4(inten * color, 1);
	}
	BrightColor = vec4(0, 0, 0, 1);
}