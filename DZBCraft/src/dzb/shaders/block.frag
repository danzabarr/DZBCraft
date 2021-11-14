#version 330 core

layout (location = 0) out vec4 color;

uniform sampler2D tex;

in DATA
{
	vec3 position;
	vec2 uv;
	float light;
} fs_in;


void main()
{
	//vec4 sunLight =   vec3(1.0, 1.0, 0.9, 1) * (mod(int(fs_in.light), 16) / 16.0);
	//vec4 torchLight = vec3(1.0, 0.9, 0.2, 1) * (floor(fs_in.light / 16) / 16.0);

	vec4 tex = texture(tex, fs_in.uv * 16 / 256);

	if (tex.a < .01) discard;
	
	tex.rgb *= fs_in.light * fs_in.light / 256.0;

	color = tex;
}
