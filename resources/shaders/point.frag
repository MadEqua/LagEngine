#version 450 core

out vec4 outColor;

uniform sampler2D diffuseTexture;

in VertexShaderOut 
{
	vec2 texCoord;
} fs_in;

float circle(vec2 coord, float r)
{
	float l = length(coord);
	return step(l, r);
}

void main()
{
	vec2 coord = gl_PointCoord - 0.5;

	float outline = circle(coord, 0.5);
	float inside = circle(coord, 0.4);

	outColor = vec4(texture2D(diffuseTexture, fs_in.texCoord).xyz,  1.0);
	outColor.rgb += inside * vec3(0.02);
	
	if(outline < 0.5) discard;
	//outColor = vec4(vec3(centerNormalized.x, centerNormalized.y, 0.0), 1.0);
}