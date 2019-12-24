#pragma once

#include <vector>

#include "Renderer.h"

struct VertexAttribute {
	unsigned int mLocation;
	unsigned int mSize;
	unsigned int mType;
	unsigned int mNormalized;
	unsigned int mStride;
	unsigned int mOffset;
};

class VertexBufferLayout {

private:

	std::vector<VertexAttribute> mVertexAttributes;
	unsigned int mStride;

public:

	VertexBufferLayout(int stride);
	~VertexBufferLayout();

	const std::vector<VertexAttribute>& GetVertexAttributes() const;

	template <typename T>
	void Push(unsigned int location, unsigned int size, unsigned int type, unsigned int offset) {
		static_assert(false);
	}

	template <>
	void Push<float>(unsigned int location, unsigned int size, unsigned int type, unsigned int offset) {
		mVertexAttributes.push_back({ location, size, GL_FLOAT, GL_FALSE, mStride, offset });
	}

};