#version 330 core

in vec2 TexCoord;

out vec4 color;

uniform sampler2D ourTexture2;


void main()
{
	color = texture(ourTexture2, TexCoord);
	
};