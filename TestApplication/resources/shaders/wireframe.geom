#version 450 core

layout (points) in;
layout(triangle_strip, max_vertices = 3) out;

in TesEvaShaderOut {
	vec3 position;
    vec2 texCoord;
} gs_in[];

out GeomShaderOut {
	noperspective vec3 distToEdges;
} gs_out;


uniform uvec2 viewportSize;


vec2 clipToViewportSpace(vec4 pointClip) {
	pointClip.xy /= pointClip.w;
	pointClip.xy = ((pointClip.xy * 0.5) + 0.5) * viewportSize;
	return pointClip.xy;
}

void emitVertex(int i, vec3 distToEdges) {
	gs_out.distToEdges = distToEdges;
	gl_Position = gl_in[i].gl_Position;
	
	EmitVertex();
}

void main() {

	vec2 p0 = clipToViewportSpace(gl_in[0].gl_Position);
	vec2 p1 = clipToViewportSpace(gl_in[1].gl_Position);
	vec2 p2 = clipToViewportSpace(gl_in[2].gl_Position);

	vec2 e0 = p1 - p0;
	vec2 e1 = p2 - p1;
	vec2 e2 = p2 - p0;

	//Triangle area = 1/2 * abs(cross(u, v)) with u and v being vectors from the same vertex
	float doubleTriArea = abs((e0.x * e2.y) - (e0.y * e2.x)); 

	//Triangle area = Base * Height / 2 => Height = 2 * Area / Base
	float h0 = doubleTriArea / length(e0);
	float h1 = doubleTriArea / length(e1);
	float h2 = doubleTriArea / length(e2);

	emitVertex(0, vec3(0, h1, 0));
	emitVertex(1, vec3(0, 0, h2));
	emitVertex(2, vec3(h0, 0, 0));
	EndPrimitive();
}