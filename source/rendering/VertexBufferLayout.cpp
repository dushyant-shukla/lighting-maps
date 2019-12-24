#include "VertexBufferLayout.h"

VertexBufferLayout::VertexBufferLayout(int stride) : mStride(stride) {
}

VertexBufferLayout::~VertexBufferLayout() {
}

const std::vector<VertexAttribute>& VertexBufferLayout::GetVertexAttributes() const {
	return mVertexAttributes;
}
