#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texturePosition;

uniform mat4 u_mvp;

out vec2 v2_texturePosition;

void main()
{
    gl_Position = u_mvp * position;
    gl_Position.z = 0.0;
    gl_Position.w = 1.0;
    v2_texturePosition = texturePosition;
};

