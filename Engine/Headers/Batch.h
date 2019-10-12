#pragma once
#include "Core/Common.h"
#include "Core/VertexBuffer.h"

class Batch {
private:
	VertexBuffer* vbo;
	int instances_count;
public:
	Batch(const std::vector<glm::vec3>& positions);
    ~Batch();

	VertexBuffer* GetOffsetsBuffer();
	int GetInstancesCount();
};