#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size): mRenderId(0) {
	GLCall(glGenBuffers(1, &mRenderId));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, mRenderId));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer() {
	GLCall(glDeleteBuffers(1, &mRenderId));
}

void VertexBuffer::Bind() const {
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, mRenderId));
}

void VertexBuffer::Unbind() const {
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
