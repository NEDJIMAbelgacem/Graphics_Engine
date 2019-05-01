#pragma once
#include "Renderer.h"

class Plane {
private:
	std::string shader_path;
	VertexBuffer* vbo;
	ShaderProgram* shader;

	glm::vec4 equation;
	glm::vec3 center;
	float size;
	int tiles_per_side;
	glm::vec3 color;
public:
	Plane(glm::vec4 equation, glm::vec3 center, float size, int tiles_per_side, 
		std::string shader_path, glm::vec3 color = glm::vec3(0.0f, 0.0f, 1.0f));
	VertexBuffer* GetVertexBuffer();
	ShaderProgram* GetShader();
	void SetZoom(float zoom);
	void SetCenterOffset(glm::vec3 offset);
	~Plane();
};