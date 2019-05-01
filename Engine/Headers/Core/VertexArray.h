#pragma once
#include "Renderer.h"
#include "VertexBuffer.h"
#include "BufferLayout.h"
#include <vector>

class VertexBuffer;
class BufferLayout;

class VertexArray {
private:
	unsigned int id;
	unsigned int offset = 0;
public:
	VertexArray();

	void AddBuffer(VertexBuffer& vbo, BufferLayout& layout);
	void AddInstanceBuffer(VertexBuffer& vbo, BufferLayout& layout, int divisor);

	void Bind() const;

	void Unbind() const;

	unsigned int GetId() const;

	~VertexArray();
};