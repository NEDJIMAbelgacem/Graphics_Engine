#include "MandelBox.h"

#define epsilon 0.0001f

MandelBox::MandelBox(ShaderProgram* shader) {
	this->shader = shader;
	vao = new VertexArray();
	float vertices[]{
		-1.0f, -1.0f,
		 1.0f, -1.0f,
		 1.0f,  1.0f,
		-1.0f,  1.0f
	};
	VertexBuffer vbo(vertices, 8 * sizeof(float));
	BufferLayout layout;
	layout.AddElement<float>(2, 0);
	vao->AddBuffer(vbo, layout);
}

MandelBox::~MandelBox() {
	delete vao;
}

void MandelBox::SetNearFarPlanes(float near_plane, float far_plane) {
    if (this->near_plane != near_plane) {
		this->near_plane = near_plane;
		this->shader->FillUniform1f("u_near", near_plane);
	}
    if (this->far_plane != far_plane) {
		this->far_plane = far_plane;
        this->shader->FillUniform1f("u_far", far_plane);
	}
}

void MandelBox::SetProjectionMatrix(glm::mat4 projection) {
	if (this->projection_matrix != projection) {
		this->projection_matrix = projection;
		shader->FillUniformMat4f("u_proj_matrix", projection);
	}
}

void MandelBox::SetViewMatrix(glm::mat4 view) {
	if (this->view_matrix != view) {
		this->view_matrix = view;
		shader->FillUniformMat4f("u_view_matrix", view);
	}
}

void MandelBox::SetBailout(float bailout) {
    shader->FillUniform1f("u_bailout", bailout);
}

void MandelBox::SetIterations(int iterations){
    shader->FillUniform1i("u_iterations", iterations);
}
void MandelBox::SetMinRad2(float MinRad2){
    shader->FillUniform1f("u_MinRad2", MinRad2);
}
void MandelBox::SetScale(float scale) {
    shader->FillUniform1f("u_scale", scale);
}

void MandelBox::SetRaymarchingParams(float min_distance, float max_distance, int max_steps) {
    shader->FillUniform1f("u_raymarching_min_distance", min_distance);
    shader->FillUniform1f("u_raymarching_max_distance", max_distance);
    shader->FillUniform1i("u_raymarching_max_steps", max_steps);
}

void MandelBox::Render() {
	vao->Bind();
	shader->Bind();
	glCall(glDrawArrays(GL_TRIANGLE_FAN, 0, 4));
	shader->Unbind();
	vao->Unbind();
}