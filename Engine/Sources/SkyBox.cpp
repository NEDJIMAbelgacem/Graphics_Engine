#include "SkyBox.h"

#define swap(a, b) unsigned char tmp = a; a = b; b = tmp

SkyBox::SkyBox(std::string folder, std::string file, ShaderProgram* shader) {
	this->shader = shader;
	skybox_name = folder;
	PrepareVAO();

	this->cubemap = new Cubemap(folder, file);
	irradiance_map = new Cubemap(this->cubemap);
}

SkyBox::SkyBox(std::string file_path, ShaderProgram* shader) {
	this->shader = shader;
	skybox_name = file_path;
	PrepareVAO();

	is_hdr = true;
	this->cubemap = new Cubemap(file_path);
	irradiance_map = new Cubemap(this->cubemap);
}

SkyBox::~SkyBox() {
	delete cubemap;
}

void SkyBox::PrepareVAO() {
	this->vao = new VertexArray();
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
	vao->Unbind();
}

void SkyBox::Setup(Camera& camera) {
    shader->FillUniform1f("u_near", camera.GetNearPlane());
    shader->FillUniform1f("u_far", camera.GetFarPlane());
}

void SkyBox::Render() {
	this->Bind(0);
	shader->FillUniform1i("u_skybox", 0);

	vao->Bind();
	shader->Bind();
	glCall(glDrawArrays(GL_TRIANGLE_FAN, 0, 4));
	shader->Unbind();
	this->Unbind();
	vao->Unbind();
}

void SkyBox::Bind(unsigned int slot) {	
	this->cubemap->Bind(slot);
}

void SkyBox::BindIrradiance(unsigned int slot) {
	this->irradiance_map->Bind(slot);
}

void SkyBox::Unbind() {
	this->cubemap->Unbind();
}

std::string SkyBox::GetName() {
	return this->skybox_name;
}