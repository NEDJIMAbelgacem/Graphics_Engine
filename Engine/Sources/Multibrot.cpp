#include "Multibrot.h"

Multibrot::Multibrot(int power, int max_iteration) : multibrot_power(power), max_iteration(max_iteration){
	shader = new ShaderProgram(shader_path);
	shader->FillUniformVec3("u_color", color);
	shader->FillUniform1i("u_multibrot_power", power);
	shader->FillUniform1i("u_max_iteration", max_iteration);
	float vertices[]{
		1.0f, 1.0f,
		-1.0f, 1.0f,
		-1.0f, -1.0f,
		1.0f, -1.0f
	};
	vbo = new VertexBuffer(vertices, 4 * 2 * sizeof(float));
	vao = new VertexArray();
	BufferLayout layout;
	layout.AddElement<float>(2, 0);
	vao->AddBuffer(*vbo, layout);
}

void Multibrot::Draw() {
	shader->Bind();
	vao->Bind();
	glCall(glDrawArrays(GL_TRIANGLE_FAN, 0, 4));
	vao->Unbind();
	shader->Unbind();
}

void Multibrot::SetZoom(float zoom) {
	shader->FillUniform1f("u_zoom", zoom);
}

void Multibrot::SetCenterOffset(glm::vec2 offset) {
	shader->FillUniformVec("u_center_offset", offset);
}

void Multibrot::ZoomInto(glm::vec2 center, float time) {
	SetZoom(time);
	SetCenterOffset(center);
}

VertexBuffer* Multibrot::GetVertexBuffer() {
	return vbo;
}

ShaderProgram* Multibrot::GetShader() {
	return shader;
}

Multibrot::~Multibrot() {
	delete vbo;
}