#version 460 core

in vec3 FragColor;
uniform vec3 Color;
uniform float CurrentTime;

out vec4 FinalColor;

void main()
{
	
	FinalColor = vec4(	( FragColor.x * (sin( CurrentTime + 3.14159265358 ) / 2.0f + 0.5f) ) + ( Color.x * (sin( CurrentTime ) / 2.0f + 0.5f) ),
						( FragColor.y * (sin( CurrentTime + 3.14159265358 ) / 2.0f + 0.5f) ) + ( Color.y * (sin( CurrentTime ) / 2.0f + 0.5f) ),
						( FragColor.z * (sin( CurrentTime + 3.14159265358 ) / 2.0f + 0.5f) ) + ( Color.z * (sin( CurrentTime ) / 2.0f + 0.5f) ),
						1.0f);

}					   