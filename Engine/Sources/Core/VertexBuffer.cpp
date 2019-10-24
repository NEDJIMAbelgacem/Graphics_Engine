#include "Core/VertexBuffer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size, VertexBufferType type) {
	glCall(glGenBuffers(1, &id));
	glCall(glBindBuffer(GL_ARRAY_BUFFER, id));
	switch (type) {
		case VertexBufferType::STATIC_DRAW:
			glCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
			break;
		case VertexBufferType::DYNAMIC_DRAW:
			glCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW));
			break;
	}
}

VertexBuffer::~VertexBuffer() {
	glCall(glDeleteBuffers(1, &id));
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

void VertexBuffer::ModifyData(unsigned int offset_in_bytes, unsigned int size, void* data) {
	glCall(glBindBuffer(GL_ARRAY_BUFFER, id));
	glCall(glBufferSubData(GL_ARRAY_BUFFER, offset_in_bytes, size, data)); 
	glCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}