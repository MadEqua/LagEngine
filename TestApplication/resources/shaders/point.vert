#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec2 texCoord;

uniform mat4 mvpMatrix;

out VertexShaderOut
{
	vec2 texCoord;
} vs_out;

void main()
{
	gl_PointSize = 30.0;
	gl_Position = mvpMatrix * vec4(position, 1.0);
	
	vs_out.texCoord = texCoord;
}