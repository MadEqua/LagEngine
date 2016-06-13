#version 450 core

layout (location = 0) in vec3 position;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out VertexShaderOut 
{
    vec3 cubeMapDirection;
} vs_out;

void main()
{
	vs_out.cubeMapDirection = vec3(position.x, -position.y, position.z);
    
	mat3 rotView = mat3(viewMatrix);
	vec3 pos = rotView * position;
	
	gl_Position = projectionMatrix * vec4(pos, 1.0);	
	gl_Position.z = gl_Position.w - 0.00001;
}