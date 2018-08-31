#version 450 core

layout (triangles, equal_spacing, point_mode) in;

uniform mat4 mvpMatrix;
uniform float time;

in TesCtrlShaderOut {
	vec3 position;
    vec3 normal;
    vec3 tangent;
    vec2 texCoord;
} tese_in[];

out TesEvaShaderOut {
	vec3 position;
    vec2 texCoord;
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

vec3 wavingAnim(vec3 position) {

    float time = time * 10;
    vec3 t = vec3(sin(time + random(position.xz) * 13.2),
                  sin(time + random(position.zx) * 11.1),
                  sin(time + random(position.yx) * 18.7));

    vec3 disp = vec3(0.04 * random(position.zy),
                     0.03 * random(position.xz),
                     0.02 * random(position.yz)) * t;

    gl_PointSize = 10.0;//10.0 * (t.x * 0.5 + 1.0);
    return position + disp;
}

vec3 growingAnim(vec3 position) {
    float t = sin(time + random(position.xy) * 0.3) * 0.5 + 0.5;
    gl_PointSize = t * 15.0;
    return mix(vec3(0.0), position, t);
}

void main() {
    tese_out.texCoord = applyBaricentrics(gl_TessCoord, tese_in[0].texCoord, tese_in[1].texCoord, tese_in[2].texCoord);
    tese_out.position = applyBaricentrics(gl_TessCoord, tese_in[0].position, tese_in[1].position, tese_in[2].position);

    gl_Position = mvpMatrix * vec4(wavingAnim(tese_out.position), 1.0);
}