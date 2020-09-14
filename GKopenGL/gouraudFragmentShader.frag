#version 460 core
out vec4 OutFragColor;
  
in vec4 FragColor;

void main()
{
    OutFragColor = FragColor;
}
