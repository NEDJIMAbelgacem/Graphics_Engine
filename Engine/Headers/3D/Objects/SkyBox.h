#pragma once
#include "Core/Common.h"
#include "Core/ShaderProgram.h"
#include "Core/Cubemap.h"
#include "Core/Texture.h"
#include "stb_image.h"
#include "GameObject.h"

class ShaderProgram;
class VertexArray;
class Cubemap;

namespace N3D {

class SkyBox {
private:
	const std::vector<std::string> suffixes{ "rt", "lf", "up", "dn", "bk", "ft" };
private:
	VertexArray* vao;
	Cubemap* cubemap;

	unsigned int slot = -1;
	int width, height, bitsPerPixel;
	std::string skybox_name;
	bool is_hdr = false;
public:
	SkyBox(std::string folder, std::string file) {
		skybox_name = folder;
		PrepareVAO();

		this->cubemap = new Cubemap(folder, file);
	}

	SkyBox(std::string file_path) {
		skybox_name = file_path;
		PrepareVAO();

		is_hdr = true;
		this->cubemap = new Cubemap(file_path);
	}

	~SkyBox() {
		delete cubemap;
	}

    void FillShader(ShaderProgram& prg) {
		this->Bind(1);
		prg.FillUniform1i("u_skybox", 1);
		prg.FillUniform1i("u_is_hdr", this->is_hdr);
		// this->Unbind();
	}

	void Render() {
		vao->Bind();
		this->Bind(1);
		glCall(glDrawArrays(GL_TRIANGLE_FAN, 0, 4));
		this->Unbind();
		vao->Unbind();
	}

	void Bind(unsigned int slot = 0) {
		this->cubemap->Bind(slot);
	}
	void Unbind() {
		this->cubemap->Unbind();
	}

	std::string GetName() {
		return this->skybox_name;
	}
private:
	void PrepareVAO() {
		this->vao = new VertexArray();
		float vertices[]{
			-1.0f, -1.0f, 0.0f,
			1.0f, -1.0f, 0.0f,
			1.0f,  1.0f, 0.0f,
			-1.0f, 1.0f, 0.0f
		};
		VertexBuffer vbo(vertices, 3 * 4 * sizeof(float));
		BufferLayout layout;
		layout.AddElement<float>(3, 0);
		vao->AddBuffer(vbo, layout);
		vao->Unbind();
	}
};

}