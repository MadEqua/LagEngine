#version 450 core

out vec4 outColor;

uniform samplerCube cubemap;

in VertexShaderOut 
{
	 vec3 cubeMapDirection;
} fs_in;


void main()
{
	//outColor = vec4(1.0);
	//outColor = (0.5 * texture(diffuseTexture, fs_in.texCoord) + 0.5 * texture(diffuseTexture2, fs_in.texCoord));
	outColor = texture(cubemap, normalize(fs_in.cubeMapDirection));
	//outColor = vec4(normalize(fs_in.cubeMapDirection) * 0.5 + 0.5, 1.0);
}