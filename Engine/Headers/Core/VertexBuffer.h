#pragma once
#include "Core/Common.h"

enum class VertexBufferType { STATIC_DRAW, DYNAMIC_DRAW };

class VertexBuffer {
private:
	unsigned int id;
public:
	VertexBuffer(const void* data, unsigned int size, VertexBufferType type = VertexBufferType::STATIC_DRAW);
	~VertexBuffer();

	void Bind() const;

	void Unbind() const;

	unsigned int GetId() const;

	void ModifyData(unsigned int offset_in_bytes, unsigned int size, void* data);
};