#version 460 core

in vec3 FragColor;
in vec2 FragTexCoords;

uniform sampler2D Texture0;
uniform int CurrentFrame;
uniform vec3 ColorJumping;
uniform vec3 Color;

out vec4 FinalColor;

void main()
{

	FinalColor = texture(Texture0, FragTexCoords + vec2( (CurrentFrame + 1) / 52.0f, 0.0f) );
	if (FinalColor.x > 0.94f && FinalColor.y > 0.94f && FinalColor.z > 0.94f)
	{
		FinalColor = vec4( ColorJumping, 1.0f );
	}
}