#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 NormalMatrix;

uniform vec3 lightPos;
uniform vec3 lightColor;

out vec3 LightingColor;

void main()
{
    vec3 FragPos = vec3(view * model * vec4(aPos, 1.0));
    gl_Position = projection * vec4(FragPos, 1.0);
    vec3 Normal = NormalMatrix * aNormal;
    vec3 LightPos = vec3(view * vec4(lightPos, 1.0));

    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;


    //specular
    float specularStrength = 1.0;
    int exponent = 32;
    
    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), exponent);
    vec3 specular = specularStrength * spec * lightColor; 
            
    LightingColor = ambient + diffuse + specular;
}
