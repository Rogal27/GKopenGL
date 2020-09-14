#version 330 core
out vec4 FragColor;

in vec3 FragPos;

uniform vec3 viewPos;

uniform vec3 lightColor;

uniform float constant;
uniform float linear;
uniform float quadratic;

void main()
{
    float distance = length(viewPos - FragPos);
    float attenuation = 1.0 / (constant + linear / 10.0 * distance + quadratic / 50.0 * (distance * distance));
    FragColor = vec4(attenuation * lightColor, 1.0);
}
