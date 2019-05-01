#pragma once
#include "Core/Common.h"

class IndexBuffer {
private:
	unsigned int id;
	unsigned int count;
public:
	IndexBuffer(const unsigned int* data, int count);

	void Bind() const;

	void Unbind() const;

	unsigned int GetCount() const;

	unsigned int GetId() const;

	~IndexBuffer();
};