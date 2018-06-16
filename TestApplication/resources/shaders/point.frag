#version 450 core

out vec4 outColor;

layout (binding = 0) uniform sampler2D circleSprite;

in VertexShaderOut 
{
	vec2 texCoord;
	vec3 vertexPos;
} fs_in;


float random(vec2 v) {
    return fract(sin(v.x * 4587.3) * sin(v.y * 8745.21));
}

void main()
{
	vec3 color = texture2D(circleSprite, gl_PointCoord).rgb;
    if(length(color) < 0.1) discard;

    float rdm = random(fs_in.vertexPos.xy);
    rdm *= 5.5;

	color *= vec3(0.05, 0.08, 0.8) * rdm;

	outColor = vec4(color, 1.0);
}