#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in float light;

uniform mat4 pr_matrix;
uniform mat4 vw_matrix = mat4(1.0);
uniform vec2 chunk;

out DATA
{
	vec3 position;
	vec2 uv;
	float light;
} vs_out;


void main()
{
	gl_Position = pr_matrix * vw_matrix * (vec4(position.x + chunk.x * 16, position.y, position.z + chunk.y * 16, 1));
	vs_out.position = position;
	vs_out.uv = uv;
	vs_out.light = light;
}