#include "TerrainBlock.h"

TerrainBlock::TerrainBlock(int vertices_count) {
    this->vao = new VertexArray();
	this->SetVerticesPerSide(vertices_count);
	float quad[]{
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	};
	unsigned int indexes[]{
		0, 1, 3,
		1, 2, 3
	};
	this->vbo = new VertexBuffer(quad, sizeof(quad));
	this->ibo = new IndexBuffer(indexes, 2 * 3);

    BufferLayout layout;
	layout.AddElement<float>(3, 0);
	layout.AddElement<float>(2, 1);
	vao->AddBuffer(*vbo, layout);
    vao->Unbind();
}

VertexBuffer* TerrainBlock::GetVerticesBuffer() {
	return vbo;
}

IndexBuffer* TerrainBlock::GetIndexesBuffer() {
	return ibo;
}

void TerrainBlock::SetVerticesPerSide(int vertices_count) {
	this->vertices_count = vertices_count;
	//shader->FillUniform1i("u_vertices_per_tile", vertices_count);
}

int TerrainBlock::GetVerticesCount() {
	return this->vertices_count;
}

VertexArray* TerrainBlock::GetVertexArray() {
    return this->vao;
}

TerrainBlock::~TerrainBlock() {
	delete vbo;
	delete ibo;
    delete vao;
}