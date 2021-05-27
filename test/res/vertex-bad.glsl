#version 330 core

layout(location = 0) in vec4 position
layout(location = 1) in vec4 vertexColor

uniform mat4 u_mvp;

out vec4 v_color;

void main()
{
    gl_Position = u_mvp * position;
    gl_Position.z = 0.0;
    gl_Position.w = 1.0
    v_color = vertexColor;
};

