#version 450 core

out vec4 outColor;

uniform samplerCube cubemap;

in VertexShaderOut 
{
	 vec3 cubeMapDirection;
} fs_in;


void main()
{
	outColor = texture(cubemap, normalize(fs_in.cubeMapDirection));
}