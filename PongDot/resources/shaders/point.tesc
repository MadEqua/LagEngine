#version 450 core

layout (vertices = 3) out;

uniform mat4 modelMatrix;

in VertexShaderOut {
    vec3 normal;
    vec3 tangent;
    vec2 texCoord;
} tesc_in[];

out TesCtrlShaderOut {
    vec3 normal;
    vec3 tangent;
    vec2 texCoord;
} tesc_out[];

#define id gl_InvocationID

void main() {
    if(id == 0) {
        mat3 model = mat3(modelMatrix);
        vec3 v0 = model * gl_in[0].gl_Position.xyz;
        vec3 v1 = model * gl_in[1].gl_Position.xyz;
        vec3 v2 = model * gl_in[2].gl_Position.xyz;

        const float TRIS_PER_LENGTH = 15.0;

        float l0 = length(v2 - v0) * TRIS_PER_LENGTH;
        float l1 = length(v1 - v0) * TRIS_PER_LENGTH;
        float l2 = length(v2 - v1) * TRIS_PER_LENGTH;

        gl_TessLevelInner[0] = min(l0, min(l1, l2));
        gl_TessLevelOuter[0] = l0;
        gl_TessLevelOuter[1] = l1;
        gl_TessLevelOuter[2] = l2;

        /*gl_TessLevelInner[0] = 10.0;
        gl_TessLevelOuter[0] = 10.0;
        gl_TessLevelOuter[1] = 10.0;
        gl_TessLevelOuter[2] = 10.0;*/
    }

    gl_out[id].gl_Position = gl_in[id].gl_Position;
    tesc_out[id].normal = tesc_in[id].normal;
    tesc_out[id].tangent = tesc_in[id].tangent;
    tesc_out[id].texCoord = tesc_in[id].texCoord;
}