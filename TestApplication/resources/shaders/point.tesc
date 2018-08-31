#version 450 core

layout (vertices = 3) out;

in VertexShaderOut {
	vec3 position;
    vec3 normal;
    vec3 tangent;
    vec2 texCoord;
} tesc_in[];

out TesCtrlShaderOut {
	vec3 position;
    vec3 normal;
    vec3 tangent;
    vec2 texCoord;
} tesc_out[];

#define id gl_InvocationID

void main() {
    if(id == 0) {
        gl_TessLevelInner[0] = 10.0;
        gl_TessLevelOuter[0] = 10.0;
        gl_TessLevelOuter[1] = 10.0;
        gl_TessLevelOuter[2] = 10.0;
    }

    tesc_out[id].position = tesc_in[id].position;
    tesc_out[id].normal = tesc_in[id].normal;
    tesc_out[id].tangent = tesc_in[id].tangent;
    tesc_out[id].texCoord = tesc_in[id].texCoord;
}