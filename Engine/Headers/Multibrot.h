#pragma once
#include "Core/Common.h"
#include "Core/VertexArray.h"
#include "Core/VertexBuffer.h"
#include "Core/ShaderProgram.h"

class Multibrot {
private:
	const std::string shader_path = "resources/shaders/simulation_engine/fractals/multibrot.shader";
	VertexBuffer* vbo;
	VertexArray* vao;
	ShaderProgram* shader;
	glm::vec3 color = glm::vec3(0.0f, 1.0f, 0.0f);
	int multibrot_power;
	int max_iteration;

public:
	Multibrot(int power, int max_iteration = 100);
	VertexBuffer* GetVertexBuffer();
	ShaderProgram* GetShader();
	void ZoomInto(glm::vec2 center, float time);
	void SetZoom(float zoom);
	void SetCenterOffset(glm::vec2 offset);
	void Draw();
	~Multibrot();
};