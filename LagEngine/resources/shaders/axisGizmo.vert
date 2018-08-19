#version 450 core

layout (location = 0) in vec3 position;

uniform mat4 mvpMatrix;

out VertexShaderOut
{
	vec3 color;
} vs_out;

void main()
{
    gl_Position = mvpMatrix * vec4(position, 1.0);
    vs_out.color = position;
}