#version 450 core

out vec4 outColor;

layout (binding = 0) uniform sampler2D spriteMask;
layout (binding = 1) uniform sampler2D perlinNoiseTexture;

uniform unsigned int color1;
uniform unsigned int color2;

in TesEvaShaderOut {
    vec3 normalWorld;
    vec3 positionWorld;
} fs_in;

float perlinNoise(vec2 st) {
    return texture2D(perlinNoiseTexture, st).r;
}

void main() {
    float mask = texture2D(spriteMask, gl_PointCoord).r;
    if(mask < 0.1) discard;

    float n = perlinNoise(fs_in.positionWorld.xy) * 1.0;
    vec3 color = mix(unpackUnorm4x8(color1).rgb, unpackUnorm4x8(color2).rgb, n);

    outColor = mask * vec4(color, 1.0);
}