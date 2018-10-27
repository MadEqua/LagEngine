#version 450 core

layout (triangles, equal_spacing, point_mode) in;

uniform mat4 mvpMatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;
uniform float time;

in TesCtrlShaderOut {
    vec3 normal;
    vec3 tangent;
    vec2 texCoord;
} tese_in[];

out TesEvaShaderOut {
    vec3 normalWorld;
	vec3 positionWorld;
} tese_out;


vec2 applyBaricentrics(vec3 coord, vec2 v1, vec2 v2, vec2 v3) {
    return coord.x * v1 + coord.y * v2 + coord.z * v3;
}

vec3 applyBaricentrics(vec3 coord, vec3 v1, vec3 v2, vec3 v3) {
    return coord.x * v1 + coord.y * v2 + coord.z * v3;
}

float random(vec2 v) {
    return fract(sin(v.x * 4587.3) + sin(v.y * 8745.21));
}

vec3 circleAnim(vec3 position, vec3 normal) {

    float t = sin(time + random(position.xz * 114.0) * 165.0);
    float disp = 0.1 * random(position.zy * 100.0) * t;

    gl_PointSize = 5.0 * (t * 0.5 + 1.0);
    return position + normal * disp;
}

void main() {
    vec3 positionLocal = applyBaricentrics(gl_TessCoord, gl_in[0].gl_Position.xyz, gl_in[1].gl_Position.xyz, gl_in[2].gl_Position.xyz);
    vec3 normalLocal = applyBaricentrics(gl_TessCoord, tese_in[0].normal, tese_in[1].normal, tese_in[2].normal);
    vec3 positionDisplaced = circleAnim(positionLocal, normalLocal);

    tese_out.positionWorld = vec3(modelMatrix * vec4(positionDisplaced, 1.0));
    tese_out.normalWorld = normalize(normalMatrix * normalLocal);
    gl_Position = mvpMatrix * vec4(positionDisplaced, 1.0);
}