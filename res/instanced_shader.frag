#version 330 core

//the output of our Fragment Shader is the pixel color
out vec4 FragColor;

//the varying variables coming from the Vertex Shader
in vec3 fragCol;

void main()
{
    FragColor = vec4(fragCol, 1.0);
}