#pragma once
#include "Core/ShaderProgram.h"
#include "Core/Cubemap.h"
#include "Core/Texture.h"
#include "stb_image.h"
#include <string>
#include <tuple>
#include <fstream>
#include <sstream>
#include <string>

class ShaderProgram;
class VertexArray;
class Cubemap;

class SkyBox {
private:
	const std::vector<std::string> suffixes{ "rt", "lf", "up", "dn", "bk", "ft" };
private:
	ShaderProgram* sky_shader;
	VertexArray* vao;
	Cubemap* cubemap;
	Cubemap* irradiance_map;

	unsigned int slot = -1;
	int width, height, bitsPerPixel;
	std::string skybox_name;
	bool is_hdr = false;

	glm::vec3 light_dir;
	glm::vec3 light_color;
public:
	SkyBox(std::string folder, std::string file, ShaderProgram* shader);
	SkyBox(std::string file_path, ShaderProgram* shader);
	~SkyBox();

	void Draw(glm::mat4 view_matrix, glm::mat4 projection_matrix);

	void Bind(unsigned int slot = 0);
	void BindIrradiance(unsigned int slot = 0);
	void Unbind();

	bool IsHDR();
	std::string GetName();
	ShaderProgram* GetShader();
private:
	void PrepareVAO();
	void LoadLightParameters(std::string folder, glm::vec3& light_dir, glm::vec3& light_color);
	unsigned char* RotateImage(unsigned char* data, int x, int y, int nb_c, bool anti = false);
};