#pragma once

class IndexBuffer {

private:

	unsigned int mRenderId;
	unsigned int mCount;

protected:

public:

	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() const {
		return mCount;
	}
};
