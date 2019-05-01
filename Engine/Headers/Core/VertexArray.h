#pragma once
#include "Core/Common.h"
#include "Core/VertexBuffer.h"
#include "Core/BufferLayout.h"
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