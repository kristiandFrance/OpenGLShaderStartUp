#version 460 core

// Vertex data interpretation
layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoords;
layout (location = 2) in mat4 InstancedModel;

// Uniform VP Matrix
uniform mat4 VP;

// Outputs to fragment shader
out vec2 FragTexCoords;
flat out int InstanceID; // Pass instance ID to fragment shader

void main()
{
    gl_Position = VP * InstancedModel * vec4(Position, 1.0f);
    FragTexCoords = TexCoords;
    InstanceID = gl_InstanceID; // Assign the instance ID
}
