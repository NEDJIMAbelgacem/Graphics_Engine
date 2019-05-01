#include "Batch.h"

Batch::Batch(const std::vector<glm::vec3>& positions) {
	instances_count = positions.size();
	float* offsets = (float*)alloca(3 * positions.size() * sizeof(float));
	for (unsigned int i = 0; i < positions.size(); ++i) {
		offsets[3 * i] = positions[i][0];
		offsets[3 * i + 1] = positions[i][1];
		offsets[3 * i + 2] = positions[i][2];
	}
	vbo = new VertexBuffer(offsets, 3 * positions.size() * sizeof(float));
}

Batch::~Batch() {
    delete vbo;
}

int Batch::GetInstancesCount() {
	return instances_count;
}

VertexBuffer* Batch::GetOffsetsBuffer() {
	return vbo;
}