#pragma once
#include "Core/Common.h"

class VertexBuffer {
private:
	unsigned int id;
public:
	VertexBuffer(const void* data, unsigned int size);

	void Bind() const;

	void Unbind() const;

	unsigned int GetId() const;

	~VertexBuffer();
};