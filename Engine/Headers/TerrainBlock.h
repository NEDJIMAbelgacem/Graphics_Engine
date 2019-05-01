#pragma once
#include "Core/Common.h"
#include "Core/ShaderProgram.h"
#include "Core/VertexArray.h"
#include "Core/VertexBuffer.h" 
#include "Core/IndexBuffer.h" 

class TerrainBlock {
private:
    VertexArray* vao;
	VertexBuffer* vbo;
	IndexBuffer* ibo;
	int vertices_count;
public:
	TerrainBlock(int vertices_per_side);
	void SetVerticesPerSide(int vertices_count);
	VertexBuffer* GetVerticesBuffer();
	IndexBuffer* GetIndexesBuffer();
    VertexArray* GetVertexArray();
	int GetVerticesCount();
	~TerrainBlock();
};