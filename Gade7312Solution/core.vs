#version 330 core
layout (location = 0) in vec3 aPos;

out float Height;
out vec3 Position;

uniform mat4 m4Model;
uniform mat4 m4View;
uniform mat4 m4Projection;

void main()
{
    Height = aPos.y;
    Position = (m4View * m4Model * vec4(aPos, 1.0)).xyz;
    gl_Position = m4Projection * m4View * m4Model * vec4(aPos, 1.0);
};