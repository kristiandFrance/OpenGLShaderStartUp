#version 460 core

//vertex data interpretation
layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoords;
layout (location = 2) in vec3 Normal;
layout (location = 3) in mat4 InstancedModel;

//uniform VP Matrix
uniform mat4 VP;


//outputs to fragment shader
out vec2 FragTexCoords;
out vec3 FragNormal;
out vec3 FragPos;

//shader functionality
void main()
{
    gl_Position = VP * InstancedModel * vec4(Position, 1.0f);

    FragTexCoords = TexCoords;
    FragNormal = mat3(transpose(inverse(InstancedModel))) * Normal;
    FragPos = vec3(InstancedModel * vec4(Position, 1.0f));
}