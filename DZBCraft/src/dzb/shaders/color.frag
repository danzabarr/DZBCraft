#version 330 core

layout (location = 0) out vec4 result;

uniform vec4 color;

void main()
{
	result = color;
}
