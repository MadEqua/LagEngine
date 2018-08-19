#version 450 core

out vec4 outColor;

in VertexShaderOut
{
	vec3 color;
} fs_in;

void main() 
{
	outColor = vec4(fs_in.color, 1.0);
}