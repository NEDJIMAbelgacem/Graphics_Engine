#pragma once
#include "Core/Common.h"
#include <vector>

struct LayoutElement;

class BufferLayout {
private:
	std::vector<LayoutElement> layout_vect;
	unsigned int stride;
public:
	BufferLayout();

	template<typename T>
	void AddElement(unsigned int count, int location);

	unsigned int GetStride() const;

	std::vector<LayoutElement> GetLayoutElements() const;

	~BufferLayout();
};

struct LayoutElement {
	GLenum type;
	unsigned int count;
	bool normalized;
	int location;
	static unsigned int GetSize(GLenum type) {
		switch (type) {
		case GL_BYTE:
			return sizeof(char);
		case GL_FLOAT:
			return sizeof(float);
		case GL_UNSIGNED_INT:
			return sizeof(unsigned int);
		}
		return 0;
	}
};
