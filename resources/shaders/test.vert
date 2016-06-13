#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec2 texCoord;

uniform mat3 normalMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out VertexShaderOut 
{
	vec3 positionView;
	vec3 normalView;
    vec2 texCoord;    
} vs_out;

void main()
{
	vs_out.normalView = normalMatrix * normal;
	vec4 positionView = viewMatrix * modelMatrix * vec4(position, 1.0);
	vs_out.positionView = positionView.xyz;
    vs_out.texCoord = texCoord.st;
    gl_Position = projectionMatrix * positionView;
}