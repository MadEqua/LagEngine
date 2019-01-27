#version 450 core

out vec4 outColor;

layout (binding = 0) uniform sampler2D spriteMask;

uniform unsigned int color;

in TesEvaShaderOut {
    vec3 normalWorld;
    vec3 positionWorld;
} fs_in;

void main() {
    float mask = texture2D(spriteMask, gl_PointCoord).r;
    if(mask < 0.1) discard;

    outColor = mask * vec4(unpackUnorm4x8(color).rgb, 1.0);
}