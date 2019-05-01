#pragma once
#include "Renderer.h"

class ColoredCube {
private:
	ShaderProgram* shader;
	VertexArray* vao;
	VertexBuffer* vbo;
	IndexBuffer* ibo;
	std::string shader_path;
	glm::vec3 color;
	glm::vec3 scale;
	glm::vec3 position;
	glm::mat4 proj_matrix, view_matrix;
public:
	ColoredCube(std::string shader_path = "resources/shaders/colored_cube_shader.shader", glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f));
	void SetColor(glm::vec3 color);
	void SetPosition(glm::vec3 position);
	void SetScale(glm::vec3 scale);
	ShaderProgram* GetShader();
	VertexBuffer* GetVertexBuffer();
	IndexBuffer* GetIndexBuffer();
	void Draw(glm::mat4 view_matrix, glm::mat4 proj_matrix);
	~ColoredCube();
};