#include "Core/IndexBuffer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, int count) : count(count) {
	glCall(glGenBuffers(1, &id));
	glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
	glCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

void IndexBuffer::Bind() const {
	glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
}

void IndexBuffer::Unbind() const {
	glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

unsigned int IndexBuffer::GetCount() const {
	return count;
}

unsigned int IndexBuffer::GetId() const {
	return this->id;
}

IndexBuffer::~IndexBuffer() {
	glCall(glDeleteBuffers(1, &id));
}