#version 450 core

const int MAX_POINT_LIGHTS = 8;
const int MAX_DIRECTIONAL_LIGHTS = 4;

out vec4 outColor;

uniform mat4 viewMatrix;

uniform unsigned int pointLightCount;
uniform vec3 pointLightPositions[MAX_POINT_LIGHTS];
uniform vec3 pointLightColors[MAX_POINT_LIGHTS];
uniform vec3 pointLightAttenuations[MAX_POINT_LIGHTS];

uniform unsigned int directionalLightCount;
uniform vec3 directionalLightDirections[MAX_DIRECTIONAL_LIGHTS];
uniform vec3 directionalLightColors[MAX_DIRECTIONAL_LIGHTS];

layout (binding = 0) uniform sampler2D diffuseTexture;

in VertexShaderOut 
{
	vec3 positionView;
	vec3 normalView;
	vec2 texCoord;
} fs_in;

vec3 computeBlinnPhong(vec3 L, vec3 lightColor)
{
	vec3 N = normalize(fs_in.normalView);
	vec3 V = -normalize(fs_in.positionView);
	vec3 H = normalize(L + V);
	
	float diffuse = max(dot(N, L), 0.0);
	float specular = 0.0;

	if(diffuse > 0)
		specular = pow(max(dot(N, H), 0.0), 100.0);

	return diffuse * lightColor + specular * vec3(1.0);
}

float computeAttenuation(vec3 L, vec3 attenuationFactors)
{
		float lightDistance = length(L);
		return 1.0 / (attenuationFactors.x + 
			attenuationFactors.y * lightDistance + 
			attenuationFactors.z * lightDistance * lightDistance);
}

void main()
{
	vec3 lightSum = vec3(0);

	for(int i = 0; i < pointLightCount; ++i)
	{
		vec4 lightPosView = viewMatrix * vec4(pointLightPositions[i], 1.0f);
        vec3 L = lightPosView.xyz - fs_in.positionView;
		lightSum += computeAttenuation(L, pointLightAttenuations[i]) * computeBlinnPhong(normalize(L), pointLightColors[i]);
	}

	for(int i = 0; i < directionalLightCount; ++i)
	{
		vec4 lightDirView = viewMatrix * vec4(directionalLightDirections[i], 0.0);
		vec3 L = normalize(lightDirView.xyz);
		lightSum += computeBlinnPhong(-L, directionalLightColors[i]);
	}

	vec3 ambient = vec3(0.01);

	outColor = vec4(ambient + lightSum, 1.0) * texture(diffuseTexture, fs_in.texCoord);
	//outColor = vec4(fs_in.texCoord, 0.0, 1.0);
}