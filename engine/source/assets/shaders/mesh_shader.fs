#version 330 core

in vec3 vertexColor;
out vec4 FragColor;

void main()
{    
    FragColor = vertexColor;
}