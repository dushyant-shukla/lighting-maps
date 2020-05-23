#version 460 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 eyePosition;
uniform Material material;

in vec3 fragPosition;
in vec3 outNormal;
in vec2 textureCoordinates;

out vec4 fragColor;

vec3 PhongLighting(vec3 Ka, vec3 Kd, vec3 Ks)
{
	vec3 lightIntensity;
	float ambientStrength = 0.1;
	vec3 ambientLight = ambientStrength * Ka;

	vec3 N = normalize(outNormal);
	vec3 L = normalize(lightPosition - fragPosition);
	//vec3 L = vec3(0.2f, 1.0f, 0.3f);
	float diffuseStrength = max(dot(N, L), 0.0f);
	vec3 diffuseColor = diffuseStrength * Kd;

	vec3 specular = vec3(0, 0, 0);
	if(dot(N, L) > 0.0)
	{
		vec3 V = normalize(eyePosition - fragPosition);
		vec3 H = normalize(V + L);
		vec3 R = reflect(-L, N);
		float specularStrength = pow(max(dot(V, R), 0.0f), material.shininess);
		//float specularStrength = pow(max(dot(H, N), 0.0f), material.shininess);

		// To invert specular highlights enable the next line and comment out the line after that
		//specular = specularStrength * ( vec3(1.0) - vec3(texture(material.specular, textureCoordinates)));
		specular = specularStrength * Ks;
	}

	// enable next two lines for adding emission map
	//vec3 emissionColor = texture(material.emission, textureCoordinates).rgb;
	//lightIntensity = ambientLight + diffuseColor + specular + emissionColor;

	lightIntensity = (ambientLight + diffuseColor + specular) * lightColor;
	return lightIntensity;
	//return L;
	//return specular;
}

subroutine vec3 CalcuateColor();
subroutine uniform CalcuateColor colorMethod;

subroutine (CalcuateColor)
vec3 PhongLightingWithTexture()
{
	vec3 Ka = vec3(texture(material.diffuse, textureCoordinates));
	vec3 Kd = Ka;
	vec3 Ks = vec3(texture(material.specular, textureCoordinates));
	return PhongLighting(Ka, Kd, Ks);
}

subroutine (CalcuateColor)
vec3 PhongLightingDefault()
{
	vec3 Ka = vec3(0.87f, 0.72f, 0.53f);
	vec3 Kd = Ka;
	vec3 Ks = Ka;
	return PhongLighting(Ka, Kd, Ks);
}

void main() {
	fragColor = vec4(colorMethod(), 1.0);
}