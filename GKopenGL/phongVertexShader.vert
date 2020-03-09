#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat3 NormalMatrix;

out vec3 FragPos;
out vec3 Normal;

void main()
{
    
    vec4 fragPos = view * model * vec4(aPos, 1.0);

    gl_Position = projection * fragPos;

    FragPos = vec3(fragPos);

    Normal = NormalMatrix * aNormal;
    
} 