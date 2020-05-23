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

subroutine vec3 ColorTechnique();
subroutine vec3 ShadingTechnique();

layout(location = 0) subroutine uniform ColorTechnique colorTechnique;
layout(location = 1) subroutine uniform ShadingTechnique shadingTechnique;

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

//vec2 GetTextureCoordinatesWithParallexOffset(const in vec2 textureCoordinates, const in vec3 viewDirection)
//{
//	float height = texture(material.depthMap, textureCoordinates).r;
//	//vec2 p = (viewDirection.xy / viewDirection.z) * height * 0.05;
//	vec2 p = viewDirection.xy * height * 0.05;
//	return textureCoordinates - p;
//}

vec3 PhongLighting(vec3 Ka, vec3 Kd, vec3 Ks)
{
	vec3 lightIntensity;
	vec3 specular = vec3(0, 0, 0);
	vec3 diffuseColor = vec3(0, 0, 0);
	vec3 N;
	vec3 L; // light direction

	vec2 textureCoords = textureCoordinates;
	vec3 V = normalize(outEyePosition - outFragPosition); // view vector
	//if(parallexMapping)
	//{
	//	textureCoords = GetTextureCoordinatesWithParallexOffset(textureCoordinates, V);
	//	if(textureCoords.x > 1.0 || textureCoords.y > 1.0 || textureCoords.x < 0.0 || textureCoords.y < 0.0)
	//	{
	//		discard;
	//	} 
	//	Ka = vec3(texture(material.diffuse, textureCoords));
	//	Kd = Ka;
	//	Ks = Kd;
	//}
	//else
	//{
	//	textureCoords = textureCoordinates;
	//}

	float ambientStrength = 0.1;
	vec3 ambientLight = ambientStrength * Ka;

	//if(normalMapping)
	//{
	//	N = texture(material.normalMap, textureCoords).rgb;
	//	//N = texture(material.normalMap, textureCoords).rgb;
	//	N = normalize(N * 2.0 - 1.0); // converting normal values from [0, 1] to [1, +1]
	//	L = normalize(outLightPosition - outFragPosition);
	//}
	//else
	//{
		N = normalize(outNormal);
		L = normalize(outLightPosition - outFragPosition);
	//}

	diffuseColor = CalculateDiffuseComponent(L, N, Kd);
	specular = CalculateSpecularComponent(V, L, N, Ks);

	//lightIntensity = (ambientLight + diffuseColor + specular) * lightColor;
	lightIntensity = (ambientLight + diffuseColor) * lightColor;
	return lightIntensity;

	//return L;
	//return normalMatrix * N;
}


subroutine (ColorTechnique)
vec3 PhongWithTexture()
{
	vec3 Ka = vec3(texture(material.diffuse, textureCoordinates));
	vec3 Kd = Ka;
	vec3 Ks = Kd;
	return PhongLighting(Ka, Kd, Ks);
}

subroutine (ColorTechnique)
vec3 PhongWithColor()
{
	vec3 Ka = vec3(0.87f, 0.72f, 0.53f);
	vec3 Kd = Ka;
	vec3 Ks = Ka;
	return PhongLighting(Ka, Kd, Ks);
}

subroutine (ShadingTechnique)
vec3 DefaultShading()
{
	vec3 finalColor, specularComponent, diffuseComponent, ambientComponent;
	vec3 L, N, V;

	return colorTechnique();

	//return finalColor;
	//return vec3(1, 0, 0);
}

subroutine (ShadingTechnique)
vec3 NormalMapping()
{
	vec3 finalColor;
	//return finalColor;
	return vec3(0, 0, 1);
}

subroutine (ShadingTechnique)
vec3 ParallexMapping()
{
	vec3 finalColor;
	//return finalColor;
	return vec3(1, 1, 0);
}

void main() {
	fragColor = vec4(shadingTechnique(), 1.0);
	//fragColor = vec4(colorMethod(), 1.0);
}