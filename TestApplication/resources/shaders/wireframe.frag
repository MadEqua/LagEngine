
#version 450 core

out vec4 outColor;

in GeomShaderOut {
	noperspective vec3 distToEdges;
} fs_in;

void main() {

    const vec3 WIREFRAME_COLOR = vec3(0.0);
	const float WIREFRAME_THICKNESS = 2.0;
		
	float wireframeStrength = 0.0;

	float minDistToEdge = min(fs_in.distToEdges.x, min(fs_in.distToEdges.y, fs_in.distToEdges.z));
	if(minDistToEdge <= WIREFRAME_THICKNESS) {
		wireframeStrength = smoothstep(1.0, 0.0, minDistToEdge / WIREFRAME_THICKNESS);
	}

	vec3 color = mix(vec3(0.08, 0.06, 0.9), WIREFRAME_COLOR, wireframeStrength);
	outColor = vec4(color, 1.0);
}