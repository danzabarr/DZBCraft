#version 330 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec2 uv;

uniform mat4 pr_matrix;
uniform mat4 vw_matrix = mat4(1.0);
uniform mat4 ml_matrix = mat4(1.0);

out DATA
{
	vec3 vertex;
	vec2 uv;
} vs_out;


void main()
{
	gl_Position = pr_matrix * vw_matrix * ml_matrix * vec4(vertex, 1);
	vs_out.vertex = vertex;
	vs_out.uv = uv;
}