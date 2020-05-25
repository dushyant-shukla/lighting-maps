#version 460 core

layout(location = 0) out vec3 gPosition;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec3 gColor;

in vec3 fragPosition;
in vec3 outNormal;
in vec2 textureCoordinates;

void main() {
	vec3 Ka = vec3(0.87f, 0.72f, 0.53f);
	vec3 Kd = Ka;
	vec3 Ks = Ka;
	gPosition = fragPosition;
	gNormal = normalize(outNormal);
	gColor = Kd;
}