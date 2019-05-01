#include "ColoredCube.h"
#include "Renderer.h"

ColoredCube::ColoredCube(std::string shader_path, glm::vec3 color)
	: shader_path(shader_path), color(color) {
	shader = new ShaderProgram(shader_path);
	this->position = glm::vec3(0.0f, 0.0f, 0.0f);
	this->scale = glm::vec3(1.0f, 1.0f, 1.0f);
	shader->FillUniformMat4f("u_model", glm::mat4(1.0f));
	shader->FillUniformVec3("u_color", color);
	float vertices[] = {
		// front 
		-0.5, -0.5, 0.5,
		 0.5, -0.5, 0.5,
		 0.5,  0.5, 0.5,
		-0.5,  0.5, 0.5,
		// back
		-0.5, -0.5, -0.5,
		 0.5, -0.5, -0.5,
		 0.5,  0.5, -0.5,
		-0.5,  0.5, -0.5,
	};
	unsigned int indexes[]{
		// front
		2, 1, 0,
		0, 3, 2,
		// right
		2, 3, 7,
		7, 6, 2,
		// back
		4, 5, 6,
		6, 7, 4,
		// left
		1, 5, 4,
		4, 0, 1,
		// bottom
		3, 0, 4,
		4, 7, 3,
		// top
		6, 5, 1,
		1, 2, 6
	};

	vbo = new VertexBuffer(vertices, sizeof(vertices));
	ibo = new IndexBuffer(indexes, 36);
	vao = new VertexArray();
	BufferLayout layout;
	layout.AddElement<float>(3, 0);
	vao->AddBuffer(*vbo, layout);
}

void ColoredCube::SetColor(glm::vec3 color) {
	if (this->color != color) {
		this->color = color;
		shader->FillUniformVec3("u_color", color);
	}
}

void ColoredCube::SetPosition(glm::vec3 position) {
	if (this->position != position) {
		this->position = position;
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		model = glm::scale(model, scale);
		shader->FillUniformMat4f("u_model", model);
	}
}

void ColoredCube::SetScale(glm::vec3 scale) {
	if (this->scale != scale) {
		this->scale = scale;
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		model = glm::scale(model, scale);
		shader->FillUniformMat4f("u_model", model);
	}
}

ShaderProgram* ColoredCube::GetShader() {
	return shader;
}

VertexBuffer* ColoredCube::GetVertexBuffer() {
	return vbo;
}

IndexBuffer* ColoredCube::GetIndexBuffer() {
	return ibo;
}

void ColoredCube::Draw(glm::mat4 view_matrix, glm::mat4 proj_matrix) {
	if (this->view_matrix != view_matrix) {
		this->view_matrix = view_matrix;
		shader->FillUniformMat4f("u_view", view_matrix);
	}
	if (this->proj_matrix != proj_matrix) {
		this->proj_matrix = proj_matrix;
		shader->FillUniformMat4f("u_proj", proj_matrix);
	}
	vao->Bind();
	shader->Bind();
	ibo->Bind();
	glCall(glDrawElements(GL_TRIANGLES, ibo->GetCount(), GL_UNSIGNED_INT, 0));
	ibo->Unbind();
	shader->Unbind();
	vao->Unbind();
}

ColoredCube::~ColoredCube() {
	delete ibo;
	delete vbo;
	delete shader;
	delete vao;
}
