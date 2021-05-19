#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texturePosition;

out vec2 v2_texturePosition;

void main()
{
    gl_Position.xy = position;
    gl_Position.z = 0.0;
    gl_Position.w = 1.0;
    v2_texturePosition = texturePosition;
};

