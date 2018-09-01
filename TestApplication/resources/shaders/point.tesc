#version 450 core

layout (vertices = 3) out;

uniform mat4 mvpMatrix;

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
        vec4 p0 = mvpMatrix * gl_in[0].gl_Position;
        vec4 p1 = mvpMatrix * gl_in[1].gl_Position;
        vec4 p2 = mvpMatrix * gl_in[2].gl_Position;
        
        p0 /= p0.w;
        p1 /= p1.w;
        p2 /= p2.w;

        if(p0.z < 0.0 || p1.z < 0.0 || p2.z < 0.0) {
            gl_TessLevelInner[0] = 0.0;
            gl_TessLevelOuter[0] = 0.0;
            gl_TessLevelOuter[1] = 0.0;
            gl_TessLevelOuter[2] = 0.0;
        }
        else {
            float l0 = length(p2.xy - p0.xy) * 16.0 + 1.0;
            float l1 = length(p1.xy - p0.xy) * 16.0 + 1.0;
            float l2 = length(p2.xy - p1.xy) * 16.0 + 1.0;
            gl_TessLevelInner[0] = min(l0, min(l1, l2));
            gl_TessLevelOuter[0] = l0;
            gl_TessLevelOuter[1] = l1;
            gl_TessLevelOuter[2] = l2;
        }

        /*const float TESS_LVL = 10.0;
        gl_TessLevelInner[0] = TESS_LVL;
        gl_TessLevelOuter[0] = TESS_LVL;
        gl_TessLevelOuter[1] = TESS_LVL;
        gl_TessLevelOuter[2] = TESS_LVL;*/
    }

    gl_out[id].gl_Position = gl_in[id].gl_Position;
    tesc_out[id].normal = tesc_in[id].normal;
    tesc_out[id].tangent = tesc_in[id].tangent;
    tesc_out[id].texCoord = tesc_in[id].texCoord;
}