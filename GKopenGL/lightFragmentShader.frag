#version 460 core
out vec4 FragColor;

in vec3 FragPos;

uniform vec3 viewPos;

uniform vec3 lightColor;

uniform float constant;
uniform float linear;
uniform float quadratic;

uniform bool isFogActive;
uniform vec3 backgroundColor;

void main()
{
    float distance = length(viewPos - FragPos);
    float attenuation = 1.0 / (constant + linear / 10.0 * distance + quadratic / 50.0 * (distance * distance));
    FragColor = vec4(attenuation * lightColor, 1.0);

    if (isFogActive)
	{
		float viewDistance = length(viewPos - FragPos);
		float fogFactor = 1.0 / exp(viewDistance * 0.03);
		fogFactor = clamp(fogFactor, 0.0, 1.0);
		FragColor = vec4(mix(backgroundColor, FragColor.xyz, fogFactor), 1.0);
	}
}
