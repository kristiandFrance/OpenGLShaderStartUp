#version 460 core

//inputs from vertex shader
in vec2 FragTexCoords;

//uniform inputs
uniform sampler2D Texture0;

//output
out vec4 FinalColor;

void main()
{
    FinalColor = texture(Texture0, FragTexCoords);
}