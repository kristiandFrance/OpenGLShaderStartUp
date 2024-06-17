#version 460 core

layout (location = 0) in vec3 Position;

uniform mat4 VP;

out vec3 FragTexCoords;

void main()
{
    vec4 Pos = VP * vec4(Position, 1.0f);
    gl_Position = Pos.xyww;
    FragTexCoords = vec3(Position.xyz);
}
