#version 460 core

in vec3 FragColor;
uniform float CurrentTime;

out vec4 FinalColor;

void main()
{

	FinalColor = vec4(FragColor, 1.0f) * abs(sin(CurrentTime) / 2 + 1);

}