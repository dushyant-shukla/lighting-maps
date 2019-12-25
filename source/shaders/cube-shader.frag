#version 330 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};

uniform bool debug;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 eyePosition;
uniform Material material;

in vec3 fragPosition;
in vec3 outNormal;
in vec2 textureCoordinates;

void main() {
	if(!debug) {
		float ambientStrength = 0.1;
		vec3 ambientLight = ambientStrength * lightColor * vec3(texture(material.diffuse, textureCoordinates));

		vec3 N = normalize(outNormal);
		vec3 L = normalize(lightPosition - fragPosition);
		float diffuseStrength = max(dot(N, L), 0.0f);
		vec3 diffuseColor = diffuseStrength * lightColor * vec3(texture(material.diffuse, textureCoordinates));

		vec3 V = normalize(eyePosition - fragPosition);
		vec3 R = reflect(-L, N);
		float specularStrength = pow(max(dot(V, R), 0.0f), material.shininess);
		
		// To invert specular highlights enable the next line and comment out the line after that
		// vec3 specular = specularStrength * lightColor * ( vec3(1.0) - vec3(texture(material.specular, textureCoordinates)));
		vec3 specular = specularStrength * lightColor * vec3(texture(material.specular, textureCoordinates));

		//vec3 emissionColor = texture(material.emission, textureCoordinates).rgb;
		//gl_FragColor = vec4(ambientLight + diffuseColor + specular + emissionColor, 1.0);

		gl_FragColor = vec4(ambientLight + diffuseColor + specular, 1.0);
	}
	else {
		gl_FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);	
	}
	
}