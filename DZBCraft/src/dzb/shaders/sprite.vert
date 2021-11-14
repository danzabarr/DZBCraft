#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;

uniform mat4 pr_matrix;

out DATA
{
	vec3 position;
	vec2 uv;
} vs_out;


void main()
{
	gl_Position = pr_matrix * vec4(position, 1);
	vs_out.position = position;
	vs_out.uv = uv;
}