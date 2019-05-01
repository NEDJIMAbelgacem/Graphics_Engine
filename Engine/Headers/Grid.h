#pragma once
#include "Renderer.h"

class Grid {
private:
	float pos_x, pos_y, pos_z;
	float tile_size;
	int tiles_per_side;
	std::string shader_path;
	VertexBuffer* vbo;
	IndexBuffer* ibo;
	ShaderProgram* shader;
public:
	Grid(float x, float y, float z, float tile_size, int tiles_per_side, std::string shader_path);
	VertexBuffer* GetVertexBuffer();
	IndexBuffer* GetIndexBuffer();
	ShaderProgram* GetShader();
	~Grid();
};