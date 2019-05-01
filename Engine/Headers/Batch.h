#pragma once
#include "Core/Common.h"
#include "Model.h"

class Model;
class VertexBuffer;

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