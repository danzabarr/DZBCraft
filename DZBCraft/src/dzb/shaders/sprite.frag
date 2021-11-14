#version 330 core

layout (location = 0) out vec4 color;

uniform sampler2D tex;

in DATA
{
	vec3 position;
	vec2 uv;
} fs_in;


void main()
{
	vec4 c = texture(tex, fs_in.uv);
	if (c.a < .01) discard;
	color = c;
}
