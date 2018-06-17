#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec2 texCoord;

uniform mat4 mvpMatrix;
uniform float time;

out VertexShaderOut {
	vec2 texCoord;
	vec3 vertexPos;
} vs_out;


float random(vec2 v) {
    return fract(sin(v.x * 4587.3) + sin(v.y * 8745.21));
}

void wavingAnim() {
    vec3 t = vec3(sin(time + random(position.xz) * 13.2),
                  sin(time + random(position.zx) * 11.1),
                  sin(time + random(position.yx) * 10.7));

    vec3 disp = vec3(0.004 * random(position.zy),
                     0.003 * random(position.xz),
                     0.002 * random(position.yz)) * t;

	vec3 newPos = position + disp;

	//gl_PointSize = 10.0 * (random(position.xy) + 0.5);
	gl_PointSize = 10.0 * (t.x * 0.5 + 1.0);
	gl_Position = mvpMatrix * vec4(newPos, 1.0);

	//float depth = (gl_Position.z / gl_Position.w) * 0.5 + 0.5;
	//gl_PointSize = 10.0 * exp(depth);

	vs_out.texCoord = texCoord;
	vs_out.vertexPos = position;
}

void growingAnim() {
    float t = sin(time + random(position.xy) * 0.3) * 0.5 + 0.5;

    vec3 newPos = mix(vec3(0.0), position, t);

    gl_PointSize = t * 15.0;
    gl_Position = mvpMatrix * vec4(newPos, 1.0);

    vs_out.texCoord = texCoord;
    vs_out.vertexPos = position;
}

void main() {
	wavingAnim();
	//growingAnim();
}