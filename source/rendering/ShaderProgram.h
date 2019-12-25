#pragma once

#include <string>

class ShaderProgram {

private:
	unsigned int programId;
	char const* LoadShader(const std::string& filename);

public:

	ShaderProgram();
	~ShaderProgram();

	unsigned int GetProgramId();
	void AddShader(const std::string& filename, unsigned int shaderType);
	void LinkProgram();
	void UseProgram();
	void UnuseProgram();
};

ShaderProgram* LoadShaderProgram(std::string vertexShader, std::string fragmentShader);
