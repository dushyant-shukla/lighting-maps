#include "Texture2D.h"

#include <stb_image.h>
#include <glad/glad.h>
#include <iostream>

Texture2D::Texture2D(unsigned int textureUnit, std::string texturePath) : mTextureId(0), mTextureUnit(textureUnit), mTexturePath(texturePath) {
	glGenTextures(1, &mTextureId);
}

Texture2D::~Texture2D() {
}

void Texture2D::GenerateJpgTexture() {
	ActivateTexture();
	int width, height, nrChannels;
	unsigned char* data = stbi_load(mTexturePath.c_str(), &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "\nFailed to load texture " << mTexturePath << std::endl;
	}
	stbi_image_free(data);
}

void Texture2D::GeneratePngTexture() {
	ActivateTexture();
	int width, height, nrChannels;
	unsigned char* data = stbi_load(mTexturePath.c_str(), &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "\nFailed to load texture " << mTexturePath << std::endl;
	}
	stbi_image_free(data);
}

void Texture2D::ActivateTexture() {
	glActiveTexture(mTextureUnit);
	glBindTexture(GL_TEXTURE_2D, mTextureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture2D::Bind() {
	glActiveTexture(mTextureUnit);
	glBindTexture(GL_TEXTURE_2D, mTextureId);
}

void Texture2D::Unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}