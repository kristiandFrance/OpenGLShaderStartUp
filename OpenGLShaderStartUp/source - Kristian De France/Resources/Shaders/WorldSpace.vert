#version 460 core

// Vertex Data Interpretation
layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Color;

// Inputs
uniform mat4 QuadModelMat;

// Outputs to Fragment Shader
out vec3 FragColor;

//  Shader Functionality
void main()
{
	gl_Position = QuadModelMat *  vec4(Position, 1.0f);
	FragColor = Color;
}