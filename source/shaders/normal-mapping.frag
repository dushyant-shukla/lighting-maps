#version 460 core

struct Material {
	sampler2D diffuse;
	sampler2D normalMap;
	//sampler2D depthMap;
	float shininess;
};

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform Material material;
uniform mat3 normalMatrix;

in vec3 outNormal;
in vec2 textureCoordinates;
in vec3 outLightPosition;
in vec3 outEyePosition;
in vec3 outFragPosition;

subroutine vec3 FS_ShadingTechnique();
layout(location = 0) subroutine uniform FS_ShadingTechnique fs_shadingTechnique;

out vec4 fragColor;

vec3 CalculateSpecularComponent(const in vec3 V, const in vec3 L, const in vec3 N, const in vec3 Ks)
{
	vec3 specular = vec3(0, 0, 0);
	//vec3 H = normalize(V + L);
	if(dot(N, L) > 0.0)
	{
		vec3 R = reflect(-L, N);
		float specularStrength = pow(max(dot(V, R), 0.0f), material.shininess);
	
		// To invert specular highlights enable the next line and comment out the line after that
		//specular = specularStrength * ( vec3(1.0) - vec3(texture(material.specular, textureCoordinates)));
		specular = specularStrength * Ks;
	}
	return specular;
}

vec3 CalculateDiffuseComponent(const in vec3 L, const in vec3 N, const in vec3 Kd)
{
	vec3 diffuseColor = vec3(0, 0, 0);
	float diffuseStrength = max(dot(N, L), 0.0f);
	diffuseColor = diffuseStrength * Kd;
	return diffuseColor;
}


vec3 PhongLighting(vec3 Ka, vec3 Kd, vec3 Ks, vec3 L, vec3 N, vec3 V)
{
	float ambientStrength = 0.1;
	vec3 ambientLight = ambientStrength * Ka;

	vec3 diffuseColor = CalculateDiffuseComponent(L, N, Kd);
	vec3 specular = CalculateSpecularComponent(V, L, N, Ks);

	vec3 lightIntensity = (ambientLight + diffuseColor + specular) * lightColor;
	return lightIntensity;
}

subroutine (FS_ShadingTechnique)
vec3 DefaultShading()
{
	vec3 Ka = vec3(0.87f, 0.72f, 0.53f);
	vec3 Kd = Ka;
	vec3 Ks = Ka;

	vec3 V = normalize(outEyePosition - outFragPosition); // view vector
	vec3 N = normalize(outNormal);
	vec3 L = normalize(outLightPosition - outFragPosition);
	return PhongLighting(Ka, Kd, Ks, L, N, V);
}

subroutine (FS_ShadingTechnique)
vec3 TexturedShading()
{
	vec3 Ka = vec3(texture(material.diffuse, textureCoordinates));
	vec3 Kd = Ka;
	vec3 Ks = Kd;
	
	vec3 V = normalize(outEyePosition - outFragPosition); // view vector
	vec3 N = normalize(outNormal);
	vec3 L = normalize(outLightPosition - outFragPosition);
	return PhongLighting(Ka, Kd, Ks, L, N, V);
}

subroutine (FS_ShadingTechnique)
vec3 NormalMapping()
{
	vec3 Ka = vec3(texture(material.diffuse, textureCoordinates));
	vec3 Kd = Ka;
	vec3 Ks = Kd;

	vec3 V = normalize(outEyePosition - outFragPosition); // view vector
	vec3 N = texture(material.normalMap, textureCoordinates).rgb;
	N = normalize(N * 2.0 - 1.0); // converting normal values from [0, 1] to [1, +1]
	vec3 L = normalize(outLightPosition - outFragPosition);
	return PhongLighting(Ka, Kd, Ks, L, N, V);
}

subroutine (FS_ShadingTechnique)
vec3 ParallexMapping()
{
	vec3 finalColor;
	//return finalColor;
	return vec3(1, 1, 0);
}

void main() {
	fragColor = vec4(fs_shadingTechnique(), 1.0);
	//fragColor = vec4(colorTechnique(), 1.0);
}