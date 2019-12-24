#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"

class VertexArray {

private:

	unsigned int mRenderId;

public:

	VertexArray();
	~VertexArray();

	void AddAttributes(const VertexBuffer& vertexBuffer, const VertexBufferLayout& layout);

	void Bind() const;
	void Unbind() const;
};
