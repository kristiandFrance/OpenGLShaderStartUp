#version 460 core

in vec3 FragColor;
uniform float CurrentTime;

out vec4 FinalColor;

void main()
{
	
	FinalColor = vec4(	FragColor.x * ( sin( CurrentTime * 1.0f ) / 2.0f + 0.5f ),
						FragColor.y * ( sin( CurrentTime * 4.0f ) / 2.0f + 0.5f ),
						FragColor.z * ( sin( CurrentTime * 8.0f ) / 2.0f + 0.5f ),
						1.0f);

}