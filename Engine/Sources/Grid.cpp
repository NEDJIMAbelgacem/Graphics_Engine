#include "Grid.h"

Grid::Grid(float x, float y, float z, float tile_size, int tiles_per_side, std::string shader_path)
	: pos_x(x), pos_y(y), pos_z(z), tile_size(tile_size), tiles_per_side(tiles_per_side), shader_path(shader_path) {
	shader = new ShaderProgram(shader_path);
	
	float* vertices = (float*)alloca((tiles_per_side + 1) * (tiles_per_side + 1) * 3 * sizeof(float));
	for (int i = 0; i < (tiles_per_side + 1) * (tiles_per_side + 1); ++i) {
		vertices[3 * i] = (float)(i / (tiles_per_side + 1)) * tile_size - x;
		vertices[3 * i + 1] = y;
		vertices[3 * i + 2] = (float)(i % (tiles_per_side + 1)) * tile_size - z;
	}
	unsigned int* indexes = (unsigned int*)alloca(4 * (tiles_per_side + 1) * sizeof(unsigned int));
	for (int i = 0; i < tiles_per_side + 1; ++i) {
		indexes[4 * i] = i * (tiles_per_side + 1);
		indexes[4 * i + 1] = i * (tiles_per_side + 1) + tiles_per_side;
		indexes[4 * i + 2] = i;
		indexes[4 * i + 3] = (tiles_per_side + 1) * tiles_per_side + i;
	}
	vbo = new VertexBuffer(vertices, (tiles_per_side + 1) * (tiles_per_side + 1) * 3 * sizeof(float));
	ibo = new IndexBuffer(indexes, 4 * (tiles_per_side + 1));
}

VertexBuffer* Grid::GetVertexBuffer() {
	return vbo;
}

IndexBuffer* Grid::GetIndexBuffer() {
	return ibo;
}

ShaderProgram* Grid::GetShader() {
	return shader;
}

Grid::~Grid() {
	delete vbo;
	delete ibo;
}