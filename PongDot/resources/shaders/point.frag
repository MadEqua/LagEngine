#version 450 core

out vec4 outColor;

layout (binding = 0) uniform sampler2D spriteMask;
layout (binding = 1) uniform sampler2D perlinNoiseTexture;

uniform unsigned int color1;
uniform unsigned int color2;

const int MAX_POINT_LIGHTS = 8;
uniform unsigned int pointLightCount;
uniform vec3 pointLightPositions[MAX_POINT_LIGHTS];
uniform vec3 pointLightColors[MAX_POINT_LIGHTS];
uniform vec3 pointLightAttenuations[MAX_POINT_LIGHTS];

in TesEvaShaderOut {
    vec3 normalWorld;
    vec3 positionWorld;
} fs_in;

float perlinNoise(vec2 st) {
    return texture2D(perlinNoiseTexture, st).r;
}

vec3 computeBlinnPhongForPoints(vec3 L, vec3 lightColor) {
    vec3 N = normalize(fs_in.normalWorld);
    float diffuse = max(dot(N, L), dot(-N, L));
    return diffuse * lightColor;
}

float computeAttenuation(vec3 L, vec3 attenuationFactors) {
        float lightDistance = length(L);
        return 1.0 / (attenuationFactors.x + 
            attenuationFactors.y * lightDistance + 
            attenuationFactors.z * lightDistance * lightDistance);
}

vec3 lighting() {
    //vec3 l = vec3(0.0f);
    /*for(int i = 0; i < pointLightCount; ++i)  {
        vec3 lightPos = vec3(viewMatrix * vec4(pointLightPositions[i], 1.0));

        l += smoothstep(10.0, 0.0, distance(fs_in.positionEye, lightPos)) * pointLightColors[i];
    }
    return min(l, 1.0);*/

    vec3 lightSum = vec3(0);
    for(int i = 0; i < pointLightCount; ++i) {
        vec3 L = pointLightPositions[i].xyz - fs_in.positionWorld;
        lightSum += computeAttenuation(L, pointLightAttenuations[i]) * computeBlinnPhongForPoints(normalize(L), pointLightColors[i]);
    }
    return lightSum;
}


void main() {
    float mask = texture2D(spriteMask, gl_PointCoord).r;
    if(mask < 0.1) discard;

    float n = perlinNoise(fs_in.positionWorld.xy) * 1.0;
    vec3 color = mix(unpackUnorm4x8(color1).rgb, unpackUnorm4x8(color2).rgb, n) * lighting();

    outColor = mask * vec4(color, 1.0);
}