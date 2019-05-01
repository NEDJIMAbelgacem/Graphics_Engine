#include "Core/VertexArray.h"

VertexArray::VertexArray() {
	glCall(glGenVertexArrays(1, &id));
}

void VertexArray::AddBuffer(VertexBuffer& vbo, BufferLayout& layout) {
	this->Bind();
	vbo.Bind();
	unsigned int stride = layout.GetStride();
	std::vector<LayoutElement>& elements = layout.GetLayoutElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); ++i) {
		LayoutElement& element = elements[i];
		int& location = element.location;
		glCall(glVertexAttribPointer(location, element.count, element.type, element.normalized, stride, (const void*)offset));
		glCall(glEnableVertexAttribArray(location));
		offset += element.count * element.GetSize(element.type);
	}
	vbo.Unbind();
	this->Unbind();
}

void VertexArray::AddInstanceBuffer(VertexBuffer& vbo, BufferLayout& layout, int divisor) {
	this->Bind();
	vbo.Bind();
	unsigned int stride = layout.GetStride();
	std::vector<LayoutElement> elements = layout.GetLayoutElements();
	unsigned int offset = 0;
	for (int i = 0; i < elements.size(); ++i) {
		LayoutElement& element = elements[i];
		int location = element.location;
		glCall(glVertexAttribPointer(location, element.count, element.type, element.normalized, stride, (const void*)offset));
		glCall(glEnableVertexAttribArray(location));
		glCall(glVertexAttribDivisor(location, divisor));
		offset += element.count * element.GetSize(element.type);
	}
	vbo.Unbind();
	this->Unbind();
}

void VertexArray::Bind() const {
	glCall(glBindVertexArray(id));
}

void VertexArray::Unbind() const {
	glCall(glBindVertexArray(0));
}

unsigned int VertexArray::GetId() const {
	return this->id;
}

VertexArray::~VertexArray() {
	glCall(glDeleteVertexArrays(1, &id));
}