#version 330 core

layout (location = 0) out vec4 color;

uniform sampler2D tex;

const float TEX_SCALE = 1.0 / 16.0;

const float EPSILON_LO = 0.01;
const float EPSILON_HI = 0.99;


in DATA
{
	vec3 vertex;
	vec2 uv;
} fs_in;


void main()
{
	color = texture(tex, uv);
}
