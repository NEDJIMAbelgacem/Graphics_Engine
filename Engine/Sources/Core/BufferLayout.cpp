#include "Core/BufferLayout.h"

BufferLayout::BufferLayout() : stride(0) { }

template<typename T>
void BufferLayout::AddElement(unsigned int count, int location) {
	ASSERT(true);
}

template<>
void BufferLayout::AddElement<float>(unsigned int count, int location) {
	layout_vect.push_back(LayoutElement{ GL_FLOAT, count, false, location });
	stride += LayoutElement::GetSize(GL_FLOAT) * count;
}

template<>
void BufferLayout::AddElement<unsigned int>(unsigned int count, int location) {
	layout_vect.push_back(LayoutElement{ GL_UNSIGNED_INT, count, false, location });
	stride += LayoutElement::GetSize(GL_UNSIGNED_INT) * count;
}

template<>
void BufferLayout::AddElement<unsigned char>(unsigned int count, int location) {
	layout_vect.push_back(LayoutElement{ GL_BYTE, count, true, location });
	stride += LayoutElement::GetSize(GL_BYTE) * count;
}


unsigned int BufferLayout::GetStride() const {
	return this->stride;
}

std::vector<LayoutElement> BufferLayout::GetLayoutElements() const {
	return this->layout_vect;
}

BufferLayout::~BufferLayout() { }