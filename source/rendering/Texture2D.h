#pragma once

#include <string>

class Texture2D {

private:

	unsigned int mTextureId;
	unsigned int mTextureUnit;
	std::string mTexturePath;

	void ActivateTexture();

public:

	Texture2D(unsigned int textureUnit, std::string texturePath);
	~Texture2D();

	void GenerateJpgTexture();
	void GeneratePngTexture();

	void Bind();
	void Unbind();
};
