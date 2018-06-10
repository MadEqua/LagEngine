#version 450 core

out vec4 outColor;

layout (binding = 0) uniform sampler2D circleSprite;

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
	//vec2 coord = gl_PointCoord - 0.5;

	//float outline = circle(coord, 0.5);
	//float inside = circle(coord, 0.37);
	
	vec3 color = texture2D(circleSprite, gl_PointCoord).rgb;
	if(length(color) < 0.1) discard;
	
	//color *= vec3(0.99, 0.99, 0.99);

	outColor = vec4(color, 1.0);
	
	//outColor = vec4(fs_in.texCoord, 1.0, 1.0);
	//outColor.rgb += inside * vec3(0.4);
	
	//if(outline < 0.5) discard;
	//outColor = vec4(vec3(centerNormalized.x, centerNormalized.y, 0.0), 1.0);
}