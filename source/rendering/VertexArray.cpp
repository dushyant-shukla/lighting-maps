#include "VertexArray.h"
#include "Renderer.h"

VertexArray::VertexArray(): mRenderId(0) {
	GLCall(glGenVertexArrays(1, &mRenderId));
	GLCall(glBindVertexArray(mRenderId));
}

VertexArray::~VertexArray() {
}

void VertexArray::AddAttributes(const VertexBuffer& vertexBuffer, const VertexBufferLayout& layout) {
	GLCall(vertexBuffer.Bind());
	const std::vector<VertexAttribute> attributes = layout.GetVertexAttributes();
	for (int index = 0; index < attributes.size(); ++index) {
		VertexAttribute attribute = attributes[index];
		GLCall(glVertexAttribPointer(attribute.mLocation, attribute.mSize, attribute.mType, attribute.mNormalized, attribute.mStride, (void*)attribute.mOffset));
		GLCall(glEnableVertexAttribArray(attribute.mLocation));
	}
}

void VertexArray::Bind() const {
	GLCall(glBindVertexArray(mRenderId));
}

void VertexArray::Unbind() const {
	GLCall(glBindVertexArray(0));
}
