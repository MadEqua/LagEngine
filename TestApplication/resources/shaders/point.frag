#version 450 core

out vec4 outColor;

layout (binding = 0) uniform sampler2D spriteMask;
layout (binding = 1) uniform sampler2D perlinNoiseTexture;

uniform float time;

in TesEvaShaderOut {
	vec3 position;
	vec2 texCoord;
} fs_in;

float perlinNoise(vec2 st) {
    return texture2D(perlinNoiseTexture, st).r;
}

void main() {
	float mask = texture2D(spriteMask, gl_PointCoord).r;
    if(mask < 0.1) discard;

    float n = perlinNoise(fs_in.position.xy * 0.01);
	vec3 color = vec3(0.08, 0.06, 0.9) * n * mask;

	outColor = vec4(color, 1.0);
}