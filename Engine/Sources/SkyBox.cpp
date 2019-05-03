#include "SkyBox.h"

#define swap(a, b) unsigned char tmp = a; a = b; b = tmp

SkyBox::SkyBox(std::string folder, std::string file, ShaderProgram* shader) {
	sky_shader = shader;
	skybox_name = folder;
	PrepareVAO();

	this->cubemap = new Cubemap(folder, file);
	irradiance_map = new Cubemap(this->cubemap);
}

SkyBox::SkyBox(std::string file_path, ShaderProgram* shader) {
	sky_shader = shader;
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

void SkyBox::Draw(glm::mat4 view_matrix, glm::mat4 projection_matrix) {
	glm::mat4 invprojview = glm::inverse(projection_matrix * view_matrix);
	sky_shader->FillUniformMat4f("u_invprojview", invprojview);
	this->Bind(0);
	sky_shader->FillUniform1i("u_skybox", 0);

	vao->Bind();
	sky_shader->Bind();
	glCall(glDrawArrays(GL_TRIANGLE_FAN, 0, 4));
	sky_shader->Unbind();
	this->Unbind();
	vao->Unbind();
}

ShaderProgram* SkyBox::GetShader() {
	return sky_shader;
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

bool SkyBox::IsHDR() {
	return this->is_hdr;
}

void SkyBox::LoadLightParameters(std::string folder, glm::vec3& light_dir, glm::vec3& light_color) {
	std::ifstream input(std::string() + "resources/skyboxes/" + folder + "/" + folder + ".shader");
	std::string line;

	while (std::getline(input, line)) {
		std::stringstream ss(line);
		std::string first_token;
		ss >> first_token;
		if (first_token == "q3map_sun") {
			float x, y, z;
			int r, g, b;
			ss >> x >> y >> z;
			ss >> r >> g >> b;
			light_dir = glm::vec3(x, y, z);
			light_color = glm::vec3(r / 256.0f, g / 256.0f, b / 256.0f);
			break;
		}
	}
}

unsigned char* SkyBox::RotateImage(unsigned char* data, int x, int y, int nb_c, bool anti) {
	if (x != y) N3D_LOG_WARN("cube map face not a square and I haven't implemented that path yet :3");
	unsigned char* data2 = new unsigned char[x * y * nb_c];
	for (int i = 0; i < x; ++i) {
		for (int j = 0; j < y; ++j) {
			int index1 = i * y + j;
			int index2 = (y - 1 - index1 % y) * x + index1 / y;
			for (int k = 0; k < nb_c; ++k) {
				if (anti) data2[nb_c * index1 + k] = data[nb_c * index2 + k];
				else data2[nb_c * index2 + k] = data[nb_c * index1 + k];
			}
		}
	}
	stbi_image_free(data);
	return data2;
}