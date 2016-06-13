#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec2 texCoord;

uniform mat4 mvpMatrix;

out VertexShaderOut 
{
    vec3 cubeMapDirection;
} vs_out;

void main()
{
	//vs_out.cubeMapDirection = normalize(modelMatrix * vec4(position, 1.0)).xyz;
	vs_out.cubeMapDirection = vec3(position.x, -position.y, position.z);
    gl_Position = mvpMatrix * vec4(position, 1.0);
}