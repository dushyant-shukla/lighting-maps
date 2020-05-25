#version 460 core

struct Material {
	//sampler2D diffuse;
	//sampler2D specular;
	//sampler2D emission;
	float shininess;
};

in vec2 textureCoordinates;

uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 eyePosition;
uniform Material material;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gColor;

out vec4 fragColor;

void main() {

	vec3 fragmentPosition = texture(gPosition, textureCoordinates).rgb;
	vec3 N = texture(gNormal, textureCoordinates).rgb;
	vec3 L = normalize(lightPosition - fragmentPosition);

	vec3 Kd = texture(gColor, textureCoordinates).rgb;
	vec3 Ka = Kd;
	vec3 Ks = Kd;

	float ambientStrength = 0.1;
	vec3 ambientLight = ambientStrength * Ka;

	//vec3 L = vec3(0.2f, 1.0f, 0.3f);
	float diffuseStrength = max(dot(N, L), 0.0f);
	vec3 diffuseColor = diffuseStrength * Kd;

	vec3 specular = vec3(0, 0, 0);
	if(dot(N, L) > 0.0)
	{
		vec3 V = normalize(eyePosition - fragmentPosition);
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

	vec3 lightIntensity = (ambientLight + diffuseColor + specular) * lightColor;

	//fragColor = vec4(lightIntensity, 1.0);
	//fragColor = vec4(fragmentPosition, 1.0);
	fragColor = vec4(N, 1.0);
}