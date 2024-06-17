#version 460 core

in vec2 FragTexCoords;
flat in int InstanceID; // Receive instance ID from vertex shader

uniform vec3 Colors[100]; // Adjust array size according to maximum instances

out vec4 FinalColor;

void main()
{
    // Use InstanceID passed from vertex shader
    FinalColor = vec4(Colors[InstanceID], 1.0);
}
