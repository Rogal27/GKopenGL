#version 460 core
out vec4 FragColor;

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 color;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 color;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    vec3 color;
};

#define NR_DIR_LIGHTS 16
#define NR_POINT_LIGHTS 32
#define NR_SPOT_LIGHTS 32

uniform int dirLightsCount;
uniform int pointLightsCount;
uniform int spotLightsCount;

uniform DirLight dirLights[NR_DIR_LIGHTS];
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLights[NR_SPOT_LIGHTS];

uniform vec3 viewPos;

in vec3 FragPos;
in vec3 Normal;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

uniform bool isFogActive;
uniform vec3 backgroundColor;

uniform bool isBlinnPhong;

// function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir);

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = vec3(0.0, 0.0, 0.0);

    //Directional Lights
    for(int i = 0; i < NR_DIR_LIGHTS; i++)
    {
        if(i >= dirLightsCount)
            break;
        result += CalcDirLight(dirLights[i], norm, viewDir);
    }
    //Point Lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        if(i >= pointLightsCount)
            break;
        result += CalcPointLight(pointLights[i], norm, viewDir);
    }
    //Spot Lights
    for(int i = 0; i < NR_SPOT_LIGHTS; i++)
    {
        if(i >= spotLightsCount)
            break;
        result += CalcSpotLight(spotLights[i], norm, viewDir);
    }

    FragColor = vec4(result, 1.0);

    if (isFogActive)
	{
		float viewDistance = length(viewPos - FragPos);
		float fogFactor = 1.0 / exp(viewDistance * 0.08);
		fogFactor = clamp(fogFactor, 0.0, 1.0);
		FragColor = vec4(mix(backgroundColor, FragColor.xyz, fogFactor), 1.0);
	}
}  

// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    float spec = 0.0;
    if (isBlinnPhong)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);
	    spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    }
    else
    {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
    }
    // combine results
    vec3 ambient = light.ambient * light.color * vec3(texture(texture_diffuse1, TexCoords));
    vec3 diffuse = light.diffuse * diff * light.color * vec3(texture(texture_diffuse1, TexCoords));
    vec3 specular = light.specular * spec * light.color * vec3(texture(texture_specular1, TexCoords));
    return (ambient + diffuse + specular);
}

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - FragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    float spec = 0.0;
    if (isBlinnPhong)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);
	    spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    }
    else
    {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
    }
    // attenuation
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results

    vec3 ambient = light.ambient * light.color * vec3(texture(texture_diffuse1, TexCoords));
    vec3 diffuse = light.diffuse * diff * light.color * vec3(texture(texture_diffuse1, TexCoords));
    vec3 specular = light.specular * spec * light.color * vec3(texture(texture_specular1, TexCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - FragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    float spec = 0.0;
    if (isBlinnPhong)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);
	    spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    }
    else
    {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
    }
    // attenuation
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * light.color * vec3(texture(texture_diffuse1, TexCoords));
    vec3 diffuse = light.diffuse * diff * light.color * vec3(texture(texture_diffuse1, TexCoords));
    vec3 specular = light.specular * spec * light.color * vec3(texture(texture_specular1, TexCoords));

    float factor = attenuation * intensity;
    ambient *= factor;
    diffuse *= factor;
    specular *= factor;
    return (ambient + diffuse + specular);
}
