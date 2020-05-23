#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 inTextureCoordinates;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 biTangent;

out vec3 outNormal;
out vec2 textureCoordinates;

out vec3 outLightPosition;
out vec3 outEyePosition;
out vec3 outFragPosition;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat3 normalMatrix;

uniform vec3 lightPosition;
uniform vec3 eyePosition;

uniform int shadeTeq;

subroutine void VS_ShadingTechnique();
subroutine uniform VS_ShadingTechnique vs_shadingTechnique;

void TangentSpaceCalculations()
{
	vec3 eyeSpaceNormal = normalize(normalMatrix * normal);
	vec3 eyeSpaceTangent = normalize(normalMatrix * tangent);
	eyeSpaceTangent = normalize(eyeSpaceTangent - dot(eyeSpaceTangent, eyeSpaceNormal) * eyeSpaceNormal);
	vec3 eyeSpaceoutBiTangent = cross(eyeSpaceNormal, eyeSpaceTangent);
	mat3 tangentSpaceMatrix = transpose(mat3(eyeSpaceTangent, eyeSpaceoutBiTangent, eyeSpaceNormal)); // c1, c2, c3
	outFragPosition = tangentSpaceMatrix * vec3(model * vec4(position, 2.0f));
	outLightPosition = tangentSpaceMatrix * lightPosition;
	outEyePosition = tangentSpaceMatrix * eyePosition;
	textureCoordinates = inTextureCoordinates;
	return;
}

subroutine (VS_ShadingTechnique)
void VS_DefaultShading()
{
	outNormal = normalize(normalMatrix * normal);
	outFragPosition = vec3(model * vec4(position, 2.0f));
	outLightPosition = lightPosition;
	outEyePosition = eyePosition;
	textureCoordinates = inTextureCoordinates;
	return;
}

subroutine (VS_ShadingTechnique)
void VS_NormalMapping()
{
	TangentSpaceCalculations();
	return;
}

subroutine (VS_ShadingTechnique)
void VS_ParallexMapping()
{
	TangentSpaceCalculations();
	return;
}

void main() {
	gl_Position = projection * view * model * vec4(position, 2.0f);
	vs_shadingTechnique();
}