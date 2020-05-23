#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 inTextureCoordinates;

out vec3 outNormal;
out vec3 fragPosition;
out vec2 textureCoordinates;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat3 normalMatrix;

void main() {
	gl_Position = projection * view * model * vec4(position, 2.0f);
	fragPosition = vec3(model * vec4(position, 2.0f));
	outNormal = normalMatrix * normal;
	textureCoordinates = inTextureCoordinates;
}