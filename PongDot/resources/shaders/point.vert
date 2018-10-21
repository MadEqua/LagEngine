#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec2 texCoord;

out VertexShaderOut {
    vec3 normal;
    vec3 tangent;
    vec2 texCoord;
} vs_out;

void main() {
    gl_Position = vec4(position, 1.0);
    vs_out.normal = normal;
    vs_out.tangent = tangent;
    vs_out.texCoord = texCoord;
}