#pragma once
#include "Core/Common.h"
#include <string>
#include <iostream>
#include "Core/VertexArray.h"
#include "Core/IndexBuffer.h"
#include "Core/ShaderProgram.h"
#include "glm/glm.hpp"
#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/matrix_decompose.hpp"
#include "Mesh.h"
#include "Model.h"
#include "SkyBox.h"
#include "Terrain.h"
#include "Grid.h"
#include "Quad.h"
#include "ColoredCube.h"
#include "Batch.h"
#include "Plane.h"

#define ASSERT(x) if (!(x)) __debugbreak();
#define glCall(x) GLClearError();\
	x;\
	ASSERT(!GLPrintErrors(#x, __FILE__, __LINE__))

class VertexArray;
class IndexBuffer;
class ShaderProgram;
class Mesh;
class Model;
class ModelComponent;
class SkyBox;
class Terrain;
class Grid;
class Quad;
class ColoredCube;
class Batch;
//class WaterPlane;
class Plane;

class Renderer {
private:
	glm::mat4 view_matrix = glm::mat4(1.0f);
	glm::mat4 proj_matrix = glm::mat4(1.0f);
	glm::vec4 clip_plane = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
	bool clip_enabled = false;
public:
	Renderer();
	void SetClipPlane(glm::vec4 plane);
	void EnableClipping();
	void DisableClipping();
	void UpdateShaderMatrices(ShaderProgram* shader);
	void UpdateShaderClipPlane(ShaderProgram* shader);
	void ClearScr() const;
public:
	//void DrawTriangle(std::vector<std::pair<float, float>> vertices, std::vector<std::tuple<float, float, float>> vertices_colors, VertexArray& vao, ShaderProgram& shader);
	//void DrawWaterPlane(WaterPlane& wp, VertexArray& vao);
	//void DrawBatch(Batch& batch, VertexArray& vao);
	void DrawTerrain(Terrain& terrain, VertexArray& vao);
	void SetMatrices(glm::mat4 view, glm::mat4 proj);
	void DrawSkyBox(SkyBox& skybox, VertexArray& vao);
	//void DrawModel(Model& object, VertexArray& vao);
	void RotatingCircle(float cx, float cy, float r, VertexArray& vao, ShaderProgram& shader, unsigned int triangle_count = 30);
	void DrawCircle(std::pair<float, float> pos, float r, int triangle_count, std::tuple<float, float, float> color, VertexArray& vao, ShaderProgram& shader);
	void DrawQuad(Quad& quad, VertexArray& vao);
	void DrawPlane(Plane& plane, VertexArray& vao);
	void DrawCube(ColoredCube& cube, VertexArray& vao);
	void DrawCube(glm::vec3 center, float radius, const glm::mat4& model, Texture& texture, VertexArray& vao, ShaderProgram& shader);
	void DrawMesh(glm::mat4 model, glm::mat4 view, glm::mat4 proj, const Mesh& m, VertexArray& vao, ShaderProgram& shader);
	void DrawPlaneGrid(Grid& grid, VertexArray& vao, glm::vec3 lines_color = glm::vec3(0.0f, 0.0f, 1.0f));
private:
	//void DrawModelComponent(ModelComponent* component, VertexArray& vao, ShaderProgram& shader);
	void DrawModelComponentInstanced(int instances_count, ModelComponent* component, VertexArray& vao, ShaderProgram& shader);
	void DrawIndexes(const VertexArray& va, const IndexBuffer& ib, const ShaderProgram& shader, GLenum type = GL_TRIANGLES) const;
	void DrawArrays(unsigned int vertices_count, const VertexArray& va, const ShaderProgram& shader, GLenum type = GL_TRIANGLES) const;
	void DrawInstancedArrays(unsigned int vertices_count, unsigned int instances_count, const VertexArray& va, const ShaderProgram& shader, GLenum type = GL_TRIANGLES) const;
	void DrawInstancedIndexes(unsigned int instances_count, const VertexArray& va, const IndexBuffer& ib, const ShaderProgram& shader, GLenum type = GL_TRIANGLES) const;
	std::pair<float, float> rotate(std::pair<float, float> pos, float angle);
};