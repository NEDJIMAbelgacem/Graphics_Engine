#include "Core/VertexBuffer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size) {
	glCall(glGenBuffers(1, &id));
	glCall(glBindBuffer(GL_ARRAY_BUFFER, id));
	glCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

void VertexBuffer::Bind() const {
	glCall(glBindBuffer(GL_ARRAY_BUFFER, id));
}

void VertexBuffer::Unbind() const {
	glCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

unsigned int VertexBuffer::GetId() const {
	return this->id;
}

VertexBuffer::~VertexBuffer() {
	glCall(glDeleteBuffers(1, &id));
}