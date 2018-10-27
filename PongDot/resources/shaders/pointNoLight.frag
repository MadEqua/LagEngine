#version 450 core

out vec4 outColor;

layout (binding = 0) uniform sampler2D spriteMask;
layout (binding = 1) uniform sampler2D perlinNoiseTexture;

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

    const vec3 C1 = vec3(0.8, 0.06, 0.08);
    const vec3 C2 = vec3(0.4, 0.04, 0.01);

    float n = perlinNoise(fs_in.positionWorld.xy) * 1.0;
    vec3 color = mix(C1, C2, n);

    outColor = mask * vec4(color, 1.0);
}