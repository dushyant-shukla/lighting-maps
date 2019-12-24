#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count): mRenderId(0), mCount(count) {

	ASSERT(sizeof(unsigned int) == sizeof(GLuint));

	GLCall(glGenBuffers(1, &mRenderId));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRenderId));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer() {
	GLCall(glDeleteBuffers(1, &mRenderId));
}

void IndexBuffer::Bind() const {
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, mRenderId));
}

void IndexBuffer::Unbind() const {
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
