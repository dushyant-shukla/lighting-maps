
#include <glad/glad.h> 

#include <iostream>
#include <string>


static std::string vertexShader = R"glsl(

	#version 330 core

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
	
)glsl";

static std::string pixelShader = R"glsl(

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
			//vec3 specular = specularStrength * lightColor * ( vec3(1.0) - vec3(texture(material.specular, textureCoordinates)));
			vec3 specular = specularStrength * lightColor * vec3(texture(material.specular, textureCoordinates));

			//vec3 emissionColor = texture(material.emission, textureCoordinates).rgb;
			//gl_FragColor = vec4(ambientLight + diffuseColor + specular + emissionColor, 1.0);

			gl_FragColor = vec4(ambientLight + diffuseColor + specular, 1.0);
		}
		else {
			gl_FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);	
		}
		
	}
)glsl";

static std::string lightVertexShader = R"glsl(

	#version 330 core

	layout(location = 0) in vec3 position;

	uniform mat4 projection;
	uniform mat4 view;
	uniform mat4 model;

	void main() {
		gl_Position = projection * view * model * vec4(position, 1.0f);
	}
	
)glsl";

static std::string lightPixelShader = R"glsl(

	#version 330 core

	uniform bool debug;

	void main() {
		if(!debug) {
			gl_FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0);;
		} 
		else {
			gl_FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);	
		}
		
	}
)glsl";

#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
		x;\
		ASSERT(GLCheckError(#x, __FILE__, __LINE__))

static void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

static bool GLCheckError(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()) {
		std::cout << "[OpenGL Error] (" << error << ") : " << function << " " << file << " " << line << " " << std::endl;
		return false;
	}
	return true;
}

static unsigned int CompileShader(unsigned int shaderType, const std::string& shader) {
	unsigned int shaderId = glCreateShader(shaderType);
	const char* source = shader.c_str();
	glShaderSource(shaderId, 1, &source, nullptr);
	glCompileShader(shaderId);

	int compilationStatus;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compilationStatus);
	if (!compilationStatus) {
		int length;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*) _malloca(length * sizeof(char));
		glGetShaderInfoLog(shaderId, length, &length, message);
		std::cout << "Failed to compile " << ((shaderType == GL_VERTEX_SHADER)? "Vertex Shader": "Fragement Shader") << " shader\n" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(shaderId);
		return 0;
	}

	return shaderId;
}

static unsigned int CreateShaderProgram() {
	unsigned int programId = glCreateProgram();
	unsigned int vertexShaderId = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fragmentShaderId = CompileShader(GL_FRAGMENT_SHADER, pixelShader);
	
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);
	glLinkProgram(programId);
	glValidateProgram(programId);

	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);

	return programId;
}

static unsigned int CreateLightShaderProgram() {
	unsigned int programId = glCreateProgram();
	unsigned int vertexShaderId = CompileShader(GL_VERTEX_SHADER, lightVertexShader);
	unsigned int fragmentShaderId = CompileShader(GL_FRAGMENT_SHADER, lightPixelShader);

	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);
	glLinkProgram(programId);
	glValidateProgram(programId);

	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);

	return programId;
}