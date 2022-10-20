#version 330 core

in vec2 TexCoord;
in float Height;

out vec4 color;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;
uniform sampler2D ourTexture3;
uniform sampler2D ourTexture4;

void main()
{
    color = texture(ourTexture1, TexCoord);
};