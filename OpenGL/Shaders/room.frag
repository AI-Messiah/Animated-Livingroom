#version 430 core

flat in uint matNum;
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;
in mat3 TBN;

uniform sampler2D Texture1;
uniform sampler2D Texture2;
uniform sampler2D Texture3;
uniform sampler2D Texture4;
uniform samplerCube depthCubemap;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float farPlane;

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

float ShadowCalculation(vec3 fragPos)
{
    // get vector between fragment position and light position
    vec3 fragToLight = fragPos - lightPos;
    // use the light to fragment vector to sample from the depth map    
    float closestDepth = texture(depthCubemap, fragToLight).r;
    // it is currently in linear range between [0,1]. Re-transform back to original value
    closestDepth *= farPlane;
    // now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);
    // now test for shadows
    vec3 N = normalize(Normal);
    vec3 L = normalize(lightPos - fragPos);
    float bias = max(0.00001 * (1.0 - dot(N, L)), 0.000001);
    float shadow = currentDepth -  bias > closestDepth ? 0.35 : 1.0;

    return shadow;
}

void main(){
	vec3 color = vec3(0, 0, 0);
	switch(matNum){
		case 0:
		
			color = vec3(1, 1, 1);
			break;
		case 2:
			color = vec3(.309, .248, .039);
			
			break;		
		case 1:			
			color = texture(Texture1, TexCoord).rgb * .7;
			break;
	}
    vec3 sendColor = vec3(0, 0, 0);
	if (matNum == 3){
        vec3 albedo = texture(Texture2, TexCoord).rgb;
        albedo = pow(albedo, vec3(2.2));
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
	
        vec3 radiance = vec3(1, 1, 1) * NdotL;

        //vec3 color1 = (diffuse + specular) * radiance;
        vec3 color1 = diffuse + specular;
   
        //FragColor = vec4(color1, 1.0);
        sendColor = color1;
        
	}else{
        vec3 norm = normalize(Normal);
	    vec3 lightDir = normalize(lightPos - FragPos);
	    float diff = max(dot(norm, lightDir), 0.0);
	    vec3 viewDir = normalize(viewPos - FragPos);
	    vec3 reflectDir = reflect(-lightDir, norm);
	    float spec = 0;
	    spec = pow(max(dot(viewDir, reflectDir), 0.0), 8);
	
	    float distance    = length(lightPos - FragPos);
	    float attenuation = 1.0 / (1 + .514 * distance + .31 * (distance * distance));
	    float inten = .7;
	    if (matNum != 1) inten = .3 + diff * attenuation;
    	//FragColor = vec4(inten * color, 1);
        sendColor = inten * color;
    }
    if (matNum == 1){
        FragColor = vec4(sendColor, 1);
    }else{
        float shadow = ShadowCalculation(FragPos);
        FragColor = vec4(sendColor * shadow, 1);
    }
    BrightColor = vec4(0, 0, 0, 1);
}