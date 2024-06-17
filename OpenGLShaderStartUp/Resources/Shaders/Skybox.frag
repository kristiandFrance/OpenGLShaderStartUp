#version 460 core

uniform samplerCube Texture_Skybox;

in vec3 FragTexCoords;

out vec4 FinalColor;

void main()
{
    FinalColor = texture(Texture_Skybox, FragTexCoords);
}
